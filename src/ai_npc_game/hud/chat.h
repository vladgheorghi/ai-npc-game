#pragma once

#include <vector>
#include <string>
#include <set>
#include <unordered_map>

#include "imgui.h"

namespace ai_npc {
    struct Message {
        std::string sender;
        std::string text;
    };

    struct Conversation {
        std::set<std::string> participants;
        std::vector<Message> messages;

        static std::string key(const std::set<std::string>& participants) {
            std::string k;
            for (const auto& p : participants)
                k += p + ":";
            return k;
        }
    };

    class Chat {
        public:
            Chat();
            ~Chat();
            void showChat(const std::set<std::string> &participants, const std::string playerName);
            Conversation* getConversation(const std::set<std::string>& participants);
        private:
        protected:

        public:
            bool show;
            bool focusInput;
            bool canOpen;
        private:
            char playerInput[256];
            std::unordered_map<std::string, Conversation> conversations;
        protected:
    };
}
