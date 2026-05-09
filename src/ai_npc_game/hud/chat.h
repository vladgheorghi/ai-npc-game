#pragma once

#include <vector>
#include <string>
#include <set>
#include <unordered_map>

#include "imgui.h"

#include "ai_npc_game/llm/llm_client.h"

namespace ai_npc {
    struct Message {
        uint32_t senderId;
        std::string text;
    };

    struct Conversation {
        std::set<uint32_t> participantIds;
        std::vector<Message> messages;

        static std::string key(const std::set<uint32_t>& ids) {
            std::string k;
            for (const auto& id : ids)
                k += std::to_string(id) + ":";
            return k;
        }
    };

    class Chat {
        public:
            // Resolve character/NPC's IDs to names
            using NameResolver = std::function<std::string(uint32_t)>;
            Chat(NameResolver resolver, LLMClient* llm, uint32_t playerId);
            void showChat(uint32_t npcId);
            Conversation* getConversation(const std::set<uint32_t>& ids);
            void updateConversation(LLMResponse response);

        public:
            bool show;
            bool focusInput;
        private:
            LLMRequest createLLMRequest(Conversation* conversation);

            NameResolver resolver;
            char playerInput[256];
            std::unordered_map<std::string, Conversation> conversations;
            LLMClient* llm;
            uint32_t playerId;
    };
}
