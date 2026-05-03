#include "chat.h"

namespace ai_npc {
    Chat::Chat() {
        show = false;
        focusInput = false;
        playerInput[0] = '\0';
    }

    void Chat::showChat(const std::set<std::string> &participants, const std::string playerName) {
        Conversation *conversation = getConversation(participants);

        ImGui::SetNextWindowSize(ImVec2(520, 160), ImGuiCond_FirstUseEver);
        ImGui::Begin("Conversation", &show);
        for (auto& message : conversation->messages) {
            ImGui::TextWrapped("%s: %s", message.sender.c_str(), message.text.c_str());
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
                conversation->messages.push_back({playerName, std::string(playerInput)});
                playerInput[0] = '\0';
            } else {
                show = false;  // Enter on empty input closes the chat
            }
        }
        ImGui::End();
    }

    Conversation* Chat::getConversation(const std::set<std::string>& participants) {
        std::string k = Conversation::key(participants);
        // Create new conversation if it doesn't exists
        if (conversations.find(k) == conversations.end()) {
            Conversation conv;
            conv.participants = participants;
            conversations[k] = conv;
        }
        return &conversations[k];
    }
}
