#include "chat.h"

namespace ai_npc {
    void Chat::ShowChat() {
        if (!show) {
            return;
        }

        ImGui::SetNextWindowSize(ImVec2(520, 160), ImGuiCond_FirstUseEver);
        ImGui::Begin("Conversation", &show);
        ImGui::TextWrapped("NPC: Hello traveler, what brings you here?");
        for (auto& chatEntry : chatHistory) {
            ImGui::TextWrapped(chatEntry.c_str());
        }
        ImGui::Separator();
        ImGui::InputText("##reply", playerInput, IM_ARRAYSIZE(playerInput));
        ImGui::SameLine();
        if (ImGui::Button("Send") && playerInput[0] != '\0')
        {
            // TODO: forward playerInput to LLM
            chatHistory.push_back(std::string("Player: ") + playerInput);
            playerInput[0] = '\0';
        }
        ImGui::End();
    }
}
