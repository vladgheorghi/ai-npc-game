#pragma once

class WindowObject;

namespace gfxc
{
    class ImGuiLayer
    {
     public:
        ImGuiLayer();
        ~ImGuiLayer();

        void Init(WindowObject *window);
        void Shutdown();

        void BeginFrame();
        void EndFrame();

        bool WantCaptureMouse() const;
        bool WantCaptureKeyboard() const;

     private:
        bool initialized;
    };
}