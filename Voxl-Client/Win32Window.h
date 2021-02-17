#pragma once
#include <IWindow.h>
namespace voxl
{
    class Win32Window : public IWindow
    {
    public:
        void Init(const WindowSettings& a_Settings) override;
        void SetSettings(const WindowSettings& a_Settings) override;
        void SetResizeCallback(std::function<void(int a_X, int a_Y, bool a_FullScreen)>& a_Function) override;
        glm::ivec2 GetDimensions() override;
        void Resize(const glm::ivec2& a_Dimensions) override;
        void SetFullScreen(bool a_FullScreen) override;
        void SetHideCursor(bool a_HideCursor) override;
        void SetLockCursor(bool a_CaptureCursor) override;
        void SetAllowManualResize(bool a_AllowResize) override;
        utilities::InputData GetInputData() override;
    };
}