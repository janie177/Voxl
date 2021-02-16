#include "time/GameLoop.h"
#include "Server.h"

int main()
{
    std::unique_ptr<voxl::Server> server = std::make_unique<voxl::Server>();
    server->Start();

    auto& settings = server->GetServerSettings();

    utilities::GameLoop gameLoop(1, settings.tps, false);
    long long tick = 0;

    while(true)
    {
        const auto data = gameLoop.update();

        //Tick
        if(data.tick)
        {
            server->Tick(data.deltaTick);
            ++tick;
        }

        //Remove this but for now it's nice to see TPS.
        if(tick == 300)
        {
            server->GetLogger().log(utilities::Severity::Info, "Tps: " + std::to_string(data.tps) + ".");
            tick = 0;
        }

        if(server->HasShutDown())
        {
            break;
        }
    }

    return 0;
}
