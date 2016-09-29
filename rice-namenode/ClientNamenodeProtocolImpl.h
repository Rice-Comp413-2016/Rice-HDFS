#include <iostream>
#include "ClientNamenodeProtocol.pb.h"

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
}; // class
} // namespace
