#define ELPP_FRESH_LOG_FILE
#define ELPP_THREAD_SAFE

#include <cstdlib>
#include <thread>
#include <iostream>
#include <asio.hpp>
#include <rpcserver.h>
#include <easylogging++.h>
#include "ClientDatanodeProtocolImpl.h"
#include "data_transfer_server.h"
#include "native_filesystem.h"
#include "zk_dn_client.h"
#include "DaemonFactory.h"

// initialize the logging library (only do this once!)
INITIALIZE_EASYLOGGINGPP

#define LOG_CONFIG_FILE "dn-log-conf.conf"

using namespace client_datanode_translator;

int main(int argc, char* argv[]) {
	el::Configurations conf(LOG_CONFIG_FILE);
	el::Loggers::reconfigureAllLoggers(conf);

	int error_code = 0;

	asio::io_service io_service;
	unsigned short xferPort = 50010;
	unsigned short ipcPort = 50020;
	std::string backingStore("/var/DISK");

	// usage: namenode ip ip ip [port], optional
	//short port = 2181;
	std::string ip_port_pairs("");

	if (argc < 5) {
		LOG(INFO) << "Bad arguments supplied, exiting";
		return 1;
	}

	short port = std::atoi(argv[4]);
	std::string comma(",");
	std::string colon(":");
	std::string pstr(std::to_string(port));
	ip_port_pairs += argv[1] + colon + pstr + comma + argv[2] + colon + pstr + comma + argv[3] + colon + pstr;
	LOG(INFO) << "IP and port pairs are " << ip_port_pairs;

	if (argc >= 6) {
		xferPort = std::atoi(argv[5]);
	}
	if (argc >= 7) {
		ipcPort = std::atoi(argv[6]);
	}
	if (argc >= 8) {
		backingStore = argv[7];
	}
	LOG(INFO) << "my backingstore is " << backingStore;
	auto fs = std::make_shared<nativefs::NativeFS>(backingStore);
    if (fs == nullptr){
        LOG(INFO) << "Failed to create filesystem!";
        return -1;
    }
	uint64_t total_disk_space = fs->getTotalSpace();

	auto dncli = std::make_shared<zkclient::ZkClientDn>("54.196.213.186", ip_port_pairs, total_disk_space, ipcPort, xferPort);
	ClientDatanodeTranslator translator(ipcPort);
	auto transfer_server = std::make_shared<TransferServer>(xferPort, fs, dncli);
    dncli->setTransferServer(transfer_server);
	daemon_thread::DaemonThreadFactory factory;
	factory.create_daemon_thread(&TransferServer::sendStats, transfer_server.get(), 3);
	factory.create_daemon_thread(&TransferServer::poll_replicate, transfer_server.get(), 2);
	factory.create_daemon_thread(&TransferServer::poll_delete, transfer_server.get(), 5);
	std::thread(&TransferServer::serve, transfer_server.get(), std::ref(io_service)).detach();
	translator.getRPCServer().serve(io_service);
}
