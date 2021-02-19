#include "Renderer.h"


#include <iostream>

#include "Win32Window.h"

namespace voxl
{
    Renderer::Renderer() : m_Initialized(false)
    {

    }

    Renderer::~Renderer()
    {

    }

    bool Renderer::Init(const RenderSettings& a_Settings)
    {
        //Create the window from the given window settings.
        m_Settings = a_Settings;
        m_Window = std::make_shared<Window_Win32>(*this);

        if(!m_Window->Initialize(m_Settings.windowSettings))
        {
            std::cout << "Could not initialize window." << std::endl;
            return false;
        }
        
        //TODO init D3D12.

        //Success.
        m_Initialized = true;
        return true;
    }

    std::shared_ptr<IWindow> Renderer::GetWindow()
    {
        assert(m_Initialized);
        return m_Window;
    }

    std::shared_ptr<IChunkMesh> Renderer::CreateChunkMesh(const ChunkMeshSettings& a_Data)
    {
        assert(m_Initialized);
        //TODO
        return nullptr;
    }

    std::shared_ptr<IStaticMesh> Renderer::CreateStaticMesh(const StaticMeshDrawData& a_Data)
    {
        assert(m_Initialized);
        //TODO
        return nullptr;
    }

    std::shared_ptr<ISkeletalMesh> Renderer::CreateSkeletalMesh(const SkeletalMeshDrawData& a_Data)
    {
        assert(m_Initialized);
        //TODO
        return nullptr;
    }

    void Renderer::PrepareForFrame()
    {
        assert(m_Initialized);
    }

    void Renderer::FrameData(const ChunkMeshDrawData& a_Data)
    {
        assert(m_Initialized);
    }

    void Renderer::FrameData(const StaticMeshDrawData& a_Data)
    {
        assert(m_Initialized);
    }

    void Renderer::FrameData(const SkeletalMeshDrawData& a_Data)
    {
        assert(m_Initialized);
    }

    void Renderer::DrawFrame()
    {
        assert(m_Initialized);
    }

    void Renderer::ResizeSwapChain(const glm::ivec2& a_NewSize)
    {
        assert(m_Initialized);
        //TODO
    }
}
