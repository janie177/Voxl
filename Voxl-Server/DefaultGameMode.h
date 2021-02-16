#pragma once
#include <IGameMode.h>

namespace voxl
{
    class DefaultGameMode : public IGameMode
    {
    public:
        void Tick(double a_DeltaTime, IWorld& a_World) override;
        void Save(IWorld& a_World) override;
        void Load(IWorld& a_World) override;
        std::unique_ptr<IGameMode> Clone() override;
        std::string GetName() const override;
    };
}