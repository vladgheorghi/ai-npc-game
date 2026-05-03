#pragma once

#include <vector>

#include "imgui.h"

namespace ai_npc {
    class Chat {
        public:
            Chat();
            ~Chat();
            void ShowChat();
        private:
        protected:

        public:
            bool show;
        private:
            std::vector<std::string> chatHistory;
            char playerInput[256];
        protected:
    };
}
