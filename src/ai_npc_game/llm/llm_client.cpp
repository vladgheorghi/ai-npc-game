#include "ai_npc_game/llm/llm_client.h"

#include <httplib.h>
#include <nlohmann/json.hpp>

namespace ai_npc {
    LLMClient::LLMClient(std::string host, int port, std::string model) :
    host(std::move(host)), port(port), model(std::move(model)) {}


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
