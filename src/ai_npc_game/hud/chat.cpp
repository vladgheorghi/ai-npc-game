#include "chat.h"

namespace ai_npc {
    Chat::Chat(NameResolver resolver): show(false), focusInput(false)
    {
        playerInput[0] = '\0';
    }

    void Chat::showChat(const std::set<uint32_t>& ids, uint32_t playerId) {
        Conversation *conversation = getConversation(ids);

        ImGui::SetNextWindowSize(ImVec2(520, 160), ImGuiCond_FirstUseEver);

        std::string title = "Conversation with ";
        for (const auto& id : ids) {
            if (id != playerId) {
                title += resolver(id);
            }
        }
        ImGui::Begin(title.c_str(), &show);
        for (auto& message : conversation->messages) {
            ImGui::TextWrapped("%s: %s", resolver(message.senderId).c_str(), message.text.c_str());
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
                conversation->messages.push_back({playerId, std::string(playerInput)});
                playerInput[0] = '\0';
            } else {
                show = false;  // Enter on empty input closes the chat
            }
        }
        ImGui::End();
    }

    Conversation* Chat::getConversation(const std::set<uint32_t>& ids) {
        std::string k = Conversation::key(ids);
        // Create new conversation if it doesn't exists
        if (conversations.find(k) == conversations.end()) {
            Conversation conv;
            conv.participantIds = ids;
            conversations[k] = conv;
        }
        return &conversations[k];
    }
}
