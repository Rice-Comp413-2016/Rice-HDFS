#include <iostream>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>

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
	std::cout << "Got getFileInfo request with input " << input << std::endl;
	GetFileInfoRequestProto req;
	req.ParseFromString(input);
	const std::string& src = req.src();
	// from here, we would ask zoo-keeper something, we should check
	// the response, and either return the response or return some 
	// void response...for now we will just return			
	std::string out; 
	GetFileInfoResponseProto res;
	return Serialize(&out, res);
}

std::string ClientNamenodeTranslator::mkdir(std::string input) {
	std::cout << "Got mkdir request with input " << input << std::endl;
	MkdirsRequestProto req;
	req.ParseFromString(input);
	const std::string& src = req.src();
	const hadoop::hdfs::FsPermissionProto& permission_msg = req.masked();
	bool create_parent = req.createparent();
	std::string out;
	MkdirsResponseProto res;
	// TODO for now, just say the mkdir command failed
	res.set_result(false);
	return Serialize(&out, res);
}

std::string ClientNamenodeTranslator::append(std::string input) {
	std::cout << "Got append request with input " << input << std::endl;
	AppendRequestProto req;
	req.ParseFromString(input);
	const std::string& src = req.src();
	const std::string& clientName = req.clientname();
	std::string out;
	AppendResponseProto res;
	// TODO We don't support this operation, so we need to return some
	// kind of failure status. I've looked around and I'm not sure 
	// how to do this since this message only contains an optional
	// LocatedBlockProto. No LocatedBlockProto might be failure
	return Serialize(&out, res);
}

// TODO delete is a keyword in C++. Come up with a better replacement name than
// deleteCmd
std::string ClientNamenodeTranslator::deleteCmd(std::string input) {
	std::cout << "Got delete request with input " << input << std::endl;
	DeleteRequestProto req;
	req.ParseFromString(input);
	const std::string& src = req.src();
	const bool recursive = req.recursive();
	std::string out;
	DeleteResponseProto res;
	// TODO for now, just say the delete command failed
	res.set_result(false);
	return Serialize(&out, res);
}

std::string ClientNamenodeTranslator::create(std::string input) {
	std::cout << "Got create request with input " << input << std::endl;
	CreateRequestProto req;
	req.ParseFromString(input);
	const std::string& src = req.src();
	const hadoop::hdfs::FsPermissionProto& masked = req.masked();
	std::string out;
	CreateResponseProto res;
	// TODO for now, just say the create command failed. Not entirely sure
	// how to do that, but I think you just don't include an
	// HDFSFileStatusProto
	return Serialize(&out, res);
}

std::string ClientNamenodeTranslator::Serialize(std::string* out, google::protobuf::Message& res) {
	if (!res.SerializeToString(out)) {
		// TODO handle error
	}
	return *out;
} 

} //namespace
