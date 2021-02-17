#pragma once
#include <IClient.h>

namespace voxl
{
    class Client : public IClient
    {
    public:
        IChunkStore& GetChunkStore() override;
        IRenderer& GetRenderer() override;
        IServerConnection& GetConnection() override;
        void Run() override;
    };
}