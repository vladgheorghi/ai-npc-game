#pragma once

#include <string>
#include <vector>
#include <cstdint>

#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>

namespace ai_npc {
    struct LLMTurn {
        std::string role;
        std::string message;
    };

    struct LLMRequest {
        uint64_t id = 0;
        uint32_t npcId = 0;            // For which NPC is the request
        std::vector<LLMTurn> messages; 
        bool jsonMode = false;         // LLM is constrained to respond in JSON format if true
    };

    struct LLMResponse {
        uint64_t id = 0;
        uint32_t npcId = 0;  // For which NPC is the response
        bool ok = false;     // true if any errors
        std::string message; // Response message is stored here on success
        std::string error;   // If `ok` is true, error message is stored here
    };

    class LLMClient {
        public:
            LLMClient(std::string host = "localhost", int port = 11434,
                    std::string model = "llama3.1:8b");
            ~LLMClient();

            // No copying or moving — owning a thread makes both dangerous.
            LLMClient(const LLMClient&) = delete;
            LLMClient& operator=(const LLMClient&) = delete;
            
            // Pass request to LLM. Returns ID used for response
            // Non-blocking method
            uint64_t submit(LLMRequest request);

            // Polls for responses. Returns false if there is none
            // Called once per frame
            bool poll(LLMResponse& out);

        private:
            // Runs until game is stopped
            void workerLoop();

            LLMResponse doRequest(LLMRequest request);

            std::string host;
            int port;
            std::string model;
            
            // Worker thread (has its dedicated method in workerLoop)
            std::thread worker;
            // Atomic flag for stopping execution
            std::atomic<bool> stop{false};
            // Next request's ID
            // Atomic because incremented by main thread and read by worker thread
            std::atomic<uint64_t> nextId{1};
            
            // Mutex for requests queue
            std::mutex reqMu;
            // Puts thread in a conditioned sleep
            std::condition_variable reqCv;
            // Request queue: main thread pushes, worker pops.
            std::queue<LLMRequest> reqQ;

            // Mutex for responses queue
            std::mutex respMu;
            // Response queue: worker pushes, main thread pops
            std::queue<LLMResponse> respQ;
    };
}
