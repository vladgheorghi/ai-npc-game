#include "ai_npc_game/llm/llm_client.h"

#include <httplib.h>
#include <nlohmann/json.hpp>

namespace ai_npc {
    LLMClient::LLMClient(std::string host, int port, std::string model) :
    host(std::move(host)), port(port), model(std::move(model)) {
        // Create worker thread and pass its dedicated method and args
        worker = std::thread(&LLMClient::workerLoop, this);
    }

    LLMClient::~LLMClient() {
        {
            std::lock_guard<std::mutex> lock(reqMu);
            stop = true;
        }
        reqCv.notify_all();
        if (worker.joinable()) worker.join();
    }

    uint64_t LLMClient::submit(LLMRequest req) {
        req.id = nextId.fetch_add(1);
        const uint64_t id = req.id;

        {
            std::lock_guard<std::mutex> lock(reqMu);
            reqQ.push(std::move(req));
        }
        reqCv.notify_one();

        return id;
    }

    bool LLMClient::poll(LLMResponse& out) {
        std::lock_guard<std::mutex> lock(respMu);

        if (respQ.empty())
            return false;

        out = std::move(respQ.front());
        respQ.pop();

        return true;
    }

    void LLMClient::workerLoop() {
        while (true) {
            LLMRequest req;
            {
                std::unique_lock<std::mutex> lock(reqMu);
                reqCv.wait(lock, [this] { return stop || !reqQ.empty(); });
                if (stop && reqQ.empty()) return;
                req = std::move(reqQ.front());
                reqQ.pop();
            }

            LLMResponse resp = doRequest(req);

            {
                std::lock_guard<std::mutex> lock(respMu);
                respQ.push(std::move(resp));
            }
        }
    }

    LLMResponse LLMClient::doRequest(LLMRequest request) {
        LLMResponse response;
        response.id = request.id;
        response.npcId = request.npcId;

        // Create JSON body content of the HTTP request
        nlohmann::json body;
        body["model"] = model;
        body["stream"] = false;
        body["messages"] = nlohmann::json::array();
        for (const auto& turn : request.messages) {
            body["messages"].push_back({
                {"role", turn.role},
                {"content", turn.message}
            });
        }
        if (request.jsonMode) {
            body["format"] = "json";
        }

        // POST request to Ollama
        httplib::Client cli(host, port);
        cli.set_read_timeout(60, 0);
        cli.set_connection_timeout(5, 0);

        auto resp = cli.Post("/api/chat", body.dump(), "application/json");

        // Network-level error, no response object
        if (!resp) {
            response.ok = false;
            response.error = "http error: " + httplib::to_string(resp.error());
            return response;
        }

        // HTTP-level error
        if (resp->status != 200) {
            response.ok = false;
            response.error = "http " + std::to_string(resp->status) + ": " + resp->body;
            return response;
        }

        // Parse JSON body
        try {
            auto parsed = nlohmann::json::parse(resp->body);
            response.message = parsed.at("message").at("content").get<std::string>();
            response.ok = true;
        } catch (const std::exception& e) {
            response.ok = false;
            response.error = std::string("json parse: ") + e.what();
        }

        return response;
    }
}
