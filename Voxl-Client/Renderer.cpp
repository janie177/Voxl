#include "Renderer.h"

namespace voxl
{
    void Renderer::Init(const RenderSettings& a_Settings)
    {
    }

    void Renderer::Resize(IWindow* a_Window)
    {
    }

    std::shared_ptr<IWindow> Renderer::GetWindow()
    {
        //TODO
        return nullptr;
    }

    std::shared_ptr<IChunkMesh> Renderer::CreateChunkMesh(const ChunkMeshSettings& a_Data)
    {
        //TODO
        return nullptr;
    }

    std::shared_ptr<IStaticMesh> Renderer::CreateStaticMesh(const StaticMeshDrawData& a_Data)
    {
        //TODO
        return nullptr;
    }

    std::shared_ptr<ISkeletalMesh> Renderer::CreateSkeletalMesh(const SkeletalMeshDrawData& a_Data)
    {
        //TODO
        return nullptr;
    }

    void Renderer::PrepareForFrame()
    {
    }

    void Renderer::FrameData(const ChunkMeshDrawData& a_Data)
    {
    }

    void Renderer::FrameData(const StaticMeshDrawData& a_Data)
    {
    }

    void Renderer::FrameData(const SkeletalMeshDrawData& a_Data)
    {
    }

    void Renderer::DrawFrame()
    {
    }
}
