#pragma once

#include <vector>

#include "imgui.h"

namespace ai_npc {
    class Chat {
        public:
            Chat();
            ~Chat();
            void ShowChat();
            void SendMessage(std::string message); 
        private:
        protected:

        public:
            bool show;
            bool focusInput;
        private:
            std::vector<std::string> chatHistory;
            char playerInput[256];
        protected:
    };
}
