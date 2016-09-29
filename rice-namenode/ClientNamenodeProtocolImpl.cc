#include <iostream>

#include "ClientNamenodeProtocol.pb.h"
#include "ClientNamenodeProtocolImpl.h"

/**
 * The implementation of the rpc calls. 
 */
namespace client_namenode_translator {

// the .proto file implementation's namespace, used for messages
using namespace hadoop::hdfs;


// TODO - this will probably take some zookeeper object
ClientNamenodeTranslator::ClientNamenodeTranslator() {
	std::cout << "Created client namenode translator." << std::endl;
}

std::string ClientNamenodeTranslator::getFileInfo(std::string input) {
	GetFileInfoRequestProto req;
	req.ParseFromString(input);
    const std::string& src = req.src();
	// from here, we would ask zoo-keeper something, we should check
	// the response, and either return the response or return some 
	// void response...for now we will just return			
	std::string out; 
	GetFileInfoResponseProto res;
	if (!res.SerializeToString(&out)) {
		// TODO handle error
	}
	return out;
}

} //namespace
