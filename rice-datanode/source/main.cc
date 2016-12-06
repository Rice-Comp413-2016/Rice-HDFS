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

	asio::io_service io_service;
	unsigned short xferPort = 50010;
	unsigned short ipcPort = 50020;
	std::string backingStore("/dev/sdb");

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
	if (argc >= 7) {
		zk_ip_port_1 = argv[4];
		zk_ip_port_2 = argv[5];
		zk_ip_port_3 = argv[6];
	}
	auto fs = std::make_shared<nativefs::NativeFS>(backingStore);
	uint64_t total_disk_space = fs->getTotalSpace();
	// TODO: Change the datanode id
	auto dncli = std::make_shared<zkclient::ZkClientDn>("127.0.0.1", ip_port_pairs, total_disk_space, ipcPort, xferPort);
	ClientDatanodeTranslator translator(ipcPort);
	TransferServer transfer_server(xferPort, fs, dncli);
	daemon_thread::DaemonThreadFactory factory;
	factory.create_daemon_thread(&TransferServer::sendStats, &transfer_server, 3);
	std::thread(&TransferServer::serve, &transfer_server, std::ref(io_service)).detach();
	translator.getRPCServer().serve(io_service);
}
