#pragma once
#include "IEntityController.h"

namespace voxl
{
    class IEntity;
    class Player;

    class PlayerController : IEntityController
    {
    public:
        PlayerController();

        void Tick(float a_DeltaTime) override;
        IEntity* GetEntity() override;
        void SetEntity(IEntity* a_Entity) override;

    public:
        std::shared_ptr<IGameMode> GetGameMode();

    private:
        Player* m_Player;
    };
}
