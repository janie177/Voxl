#pragma once
#include <IRenderer.h>

namespace voxl
{
    class Renderer : public IRenderer
    {
    public:
        void Init(const RenderSettings& a_Settings) override;
        void Resize(IWindow* a_Window) override;
        std::shared_ptr<IWindow> GetWindow() override;
        std::shared_ptr<IChunkMesh> CreateChunkMesh(const ChunkMeshSettings& a_Data) override;
        std::shared_ptr<IStaticMesh> CreateStaticMesh(const StaticMeshDrawData& a_Data) override;
        std::shared_ptr<ISkeletalMesh> CreateSkeletalMesh(const SkeletalMeshDrawData& a_Data) override;
        void PrepareForFrame() override;
        void FrameData(const ChunkMeshDrawData& a_Data) override;
        void FrameData(const StaticMeshDrawData& a_Data) override;
        void FrameData(const SkeletalMeshDrawData& a_Data) override;
        void DrawFrame() override;
    };

}

