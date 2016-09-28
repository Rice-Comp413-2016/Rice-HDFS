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

	// TODO - this will probably take some zookeeper object
	ClientNamenodeTranslator() {
		std::cout << "Created client namenode translator." << std::endl;
		initLog();
	}

	GetFileInfoResponseProto getFileInfo(GetFileInfoRequestProto& req) {
        const std::string& src = req.src();
		// from here, we would ask zoo-keeper something, we should check
		// the response, and either return the response or return some 
		// void response...for now we will just return			
		return GetFileInfoResponseProto();
	}

	MkdirsResponseProto mkdirs(const MkdirsRequestProto& req) {
		const std::string& src = req.src();
		const FsPermissionProto& masked = req.masked();
		bool createparent = req.createparent();
		return MkdirsResponseProto();
	}

private:
	void initLog() {
	}
}; // class 
} // namespace
