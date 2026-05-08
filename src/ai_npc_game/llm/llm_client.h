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

            LLMResponse doRequest(LLMRequest request);

        private:
            std::string host;
            int port;
            std::string model;
    };
}
