#pragma once
#include <IWindow.h>
#include <windows.h>

namespace voxl
{
    class IRenderer;

    /*
     * Window for windows operating systems.
     * Contains underlying HWND handle and such.
     * Works with the WindowProc callback system.
     */
    class Window_Win32 : public IWindow
    {
    public:
        Window_Win32(IRenderer& a_Renderer);

        HWND GetHwnd();

    protected:

        static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    public:
        void Resize(glm::vec2 a_Dimensions) override;
        glm::vec2 GetDimensions() override;
        void SetFullScreen(bool a_FullScreen) override;
        bool IsFullScreen() override;
        void SetNativeMousePosition(const glm::vec2& a_MousePosition) override;
        glm::vec2 GetNativeMousePosition() const override;
        utilities::InputData PollInput() override;
        bool IsClosed() const override;
        void Close() override;

    private:
        RECT GetClientCoordinates() const;
        RECT GetWindowCoordinates() const;
        void CenterCursor(RECT a_ClientRect);
        void OnMoveResize();
        void ObtainFocus();

    public:
        void SetResizeCallback(std::function<void(int a_Width, int a_Height)> a_Function) override;
        bool Initialize(const WindowSettings& a_Settings) override;
    private:
        //Window settings
        WindowSettings m_Settings;

        //Event queue
        utilities::InputQueue m_InputQueue;

        //Screen dimensions
        glm::vec2 m_Dimensions;

        //Old window rect for switching back from fullscreen.
        RECT m_WindowedRect;

        //Mouse position
        glm::vec2 m_MousePos;

        //True if the window has closed
        bool m_Closed;

        //Whether or not the screen is currently full-screen.
        bool m_IsFullScreen;

        //Window handle
        HWND m_Hwnd;

        //Flag settings
        bool m_HasFocus;

        //Cursor restore options.
        HCURSOR m_OldCursor;
        bool m_CursorHidden;

        static LONG m_WindowStyle;

        //Callback function when resizing happens.
        std::function<void(int, int)> m_ResizeCallback;

        //Reference to owning renderer instance.
        IRenderer& m_Renderer;
    };
}