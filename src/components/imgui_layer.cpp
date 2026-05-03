#include "components/imgui_layer.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "core/window/window_object.h"

using namespace gfxc;


ImGuiLayer::ImGuiLayer()
    : initialized(false)
{
}


ImGuiLayer::~ImGuiLayer()
{
    Shutdown();
}


void ImGuiLayer::Init(WindowObject *window)
{
    if (initialized || !window)
        return;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window->GetGLFWWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 410 core");

    initialized = true;
}


void ImGuiLayer::Shutdown()
{
    if (!initialized)
        return;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    initialized = false;
}


void ImGuiLayer::BeginFrame()
{
    if (!initialized)
        return;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}


void ImGuiLayer::EndFrame()
{
    if (!initialized)
        return;

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


bool ImGuiLayer::WantCaptureMouse() const
{
    return initialized && ImGui::GetIO().WantCaptureMouse;
}


bool ImGuiLayer::WantCaptureKeyboard() const
{
    return initialized && ImGui::GetIO().WantCaptureKeyboard;
}