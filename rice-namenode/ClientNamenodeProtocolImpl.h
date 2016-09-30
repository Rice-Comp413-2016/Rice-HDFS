#include <iostream>
#include "ClientNamenodeProtocol.pb.h"
#include <google/protobuf/message.h>

/**
 * The implementation of the rpc calls. 
 */
namespace client_namenode_translator {

// the .proto file implementation's namespace, used for messages
using namespace hadoop::hdfs;

class ClientNamenodeTranslator {
	public:
		ClientNamenodeTranslator(); 
		static std::string getFileInfo(std::string);
		static std::string mkdir(std::string);
		static std::string append(std::string);
		static std::string deleteCmd(std::string);
		static std::string create(std::string);
	private:
		static std::string Serialize(std::string*, google::protobuf::Message&);
}; // class
} // namespace
