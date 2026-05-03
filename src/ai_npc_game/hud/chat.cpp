#include "chat.h"

namespace ai_npc {
    Chat::Chat() {
        show = false;
        focusInput = false;
        playerInput[0] = '\0';
    }

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

        // Focus on the chat input bar
        if (focusInput) {
            ImGui::SetKeyboardFocusHere();  // focuses the NEXT widget, this frame only
            focusInput = false;
        }

        bool submitted = ImGui::InputText("##reply", playerInput, IM_ARRAYSIZE(playerInput),
                                          ImGuiInputTextFlags_EnterReturnsTrue);
        ImGui::SameLine();
        // If Send button is pressed or message is submitted by pressing Enter
        if (ImGui::Button("Send") || submitted)
        {
            if (playerInput[0] != '\0') {
                chatHistory.push_back(std::string("Player: ") + playerInput);
                playerInput[0] = '\0';
            } else {
                show = false;  // Enter on empty input closes the chat
            }
        }
        ImGui::End();
    }
}
