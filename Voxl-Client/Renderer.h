#pragma once
#include <IRenderer.h>

namespace voxl
{
    class Window_Win32;

    class Renderer : public IRenderer
    {
    public:
        Renderer();
        ~Renderer() override;

        bool Init(const RenderSettings& a_Settings) override;
        std::shared_ptr<IWindow> GetWindow() override;
        std::shared_ptr<IChunkMesh> CreateChunkMesh(const ChunkMeshSettings& a_Data) override;
        std::shared_ptr<IStaticMesh> CreateStaticMesh(const StaticMeshDrawData& a_Data) override;
        std::shared_ptr<ISkeletalMesh> CreateSkeletalMesh(const SkeletalMeshDrawData& a_Data) override;
        void PrepareForFrame() override;
        void FrameData(const ChunkMeshDrawData& a_Data) override;
        void FrameData(const StaticMeshDrawData& a_Data) override;
        void FrameData(const SkeletalMeshDrawData& a_Data) override;
        void DrawFrame() override;
        void ResizeSwapChain(const glm::ivec2& a_NewSize) override;

    private:
        bool m_Initialized;
        std::shared_ptr<Window_Win32> m_Window;
        RenderSettings m_Settings;
    };

}

