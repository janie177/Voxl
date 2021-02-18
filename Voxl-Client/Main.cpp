#include "Client.h"

int main()
{
    std::unique_ptr<voxl::Client> client = std::make_unique<voxl::Client>();
    client->Start();

    getchar();

    return 0;
}