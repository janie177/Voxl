#include "DefaultGameMode.h"

namespace voxl
{
    void DefaultGameMode::Tick(double a_DeltaTime, IWorld& a_World)
    {
        //TODO
    }

    void DefaultGameMode::Save(IWorld& a_World)
    {
        //TODO
    }

    void DefaultGameMode::Load(IWorld& a_World)
    {
        //TODO
    }

    std::unique_ptr<IGameMode> DefaultGameMode::Clone()
    {
        //TODO
        return std::make_unique<DefaultGameMode>();
    }

    std::string DefaultGameMode::GetName() const
    {
        //TODO
        return "";
    }
}
