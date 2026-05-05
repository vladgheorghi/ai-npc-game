#pragma once

#include <vector>
#include <string>
#include <set>
#include <unordered_map>

#include "imgui.h"

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
            Chat(NameResolver resolver);
            void showChat(const std::set<uint32_t>& ids, uint32_t playerId);
            Conversation* getConversation(const std::set<uint32_t>& ids);

        public:
            bool show;
            bool focusInput;
        private:
            NameResolver resolver;
            char playerInput[256];
            std::unordered_map<std::string, Conversation> conversations;
    };
}
