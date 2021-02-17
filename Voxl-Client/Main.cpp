#include "Client.h"

int main()
{
    std::unique_ptr<voxl::Client> client = std::make_unique<voxl::Client>();
    client->Start();

    return 0;
}