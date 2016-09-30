#include <cstdlib>
#include <iostream>
#include <asio.hpp>
#include <rpcserver.h>

int main(int argc, char* argv[]) {
    asio::io_service io_service;
    short port = 5351;
    if (argc == 2) {
        port = std::atoi(argv[1]);
    }
    RPCServer server(port);
    server.register_handlers();
    server.serve(io_service);
}
