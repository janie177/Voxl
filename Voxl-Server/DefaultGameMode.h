#pragma once
#include <IGameMode.h>

namespace voxl
{
    class DefaultGameMode : public IGameMode
    {
    public:
        void Tick(double a_DeltaTime) override;
        void Save() override;
        void Load() override;
        std::string GetName() const override;
        void RegisterWorld(IWorld* a_World) override;
        void UnregisterWorld(IWorld* a_World) override;
    };
}