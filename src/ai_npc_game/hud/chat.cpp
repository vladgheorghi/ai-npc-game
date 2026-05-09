#include "chat.h"

namespace ai_npc {
    Chat::Chat(NameResolver resolver, LLMClient* llm, uint32_t playerId): show(false), focusInput(false), resolver(std::move(resolver)), llm(llm), playerId(playerId)
    {
        playerInput[0] = '\0';
    }

    void Chat::showChat(uint32_t npcId) {
        Conversation *conversation = getConversation({npcId, playerId});

        ImGui::SetNextWindowSize(ImVec2(520, 160), ImGuiCond_FirstUseEver);

        std::string title = "Conversation with " + resolver(npcId);

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
                llm->submit(createLLMRequest(conversation));

            } else {
                show = false;  // Enter on empty input closes the chat
            }
        }
        ImGui::End();
    }

    LLMRequest Chat::createLLMRequest(Conversation* conversation)
    {
        LLMRequest req;
        for (const auto& message : conversation->messages) {
            LLMTurn turn;
            if (message.senderId == playerId) {
                turn.role = std::string("user");
            } else {
                turn.role = std::string("assistant");
            }
            turn.message = std::string(message.text);
            req.messages.push_back(turn);
        }
        
        for (const auto& id : conversation->participantIds) {
            if (id != playerId) {
                req.npcId = id;
                break;
            }
        }

        return req;
    }

    void Chat::updateConversation(LLMResponse resp)
    {
        Conversation *conversation = getConversation({playerId, resp.npcId});

        conversation->messages.push_back({resp.npcId, std::string(resp.message)});
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
