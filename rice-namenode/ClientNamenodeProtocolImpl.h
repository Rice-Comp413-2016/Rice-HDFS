#include <iostream>
#include "ClientNamenodeProtocol.pb.h"
#include <google/protobuf/message.h>
#include <rpcserver.h>

#pragma once

/**
 * The implementation of the rpc calls. 
 */
namespace client_namenode_translator {

// the .proto file implementation's namespace, used for messages
using namespace hadoop::hdfs;

class ClientNamenodeTranslator {
	public:
		ClientNamenodeTranslator(int port); 
		std::string getFileInfo(std::string);
		std::string mkdir(std::string);
		std::string append(std::string);
		std::string deleteCmd(std::string);
		std::string create(std::string);
		std::string getBlockLocations(std::string);

		RPCServer getRPCServer();
	private:
		std::string Serialize(std::string*, google::protobuf::Message&);
		void InitServer();
		void RegisterClientRPCHandlers();

		int port;
		RPCServer server;
}; // class
} // namespace
