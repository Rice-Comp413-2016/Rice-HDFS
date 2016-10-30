#include "zkwrapper.h"
#include "zk_nn_client.h"

#include <gtest/gtest.h>

#include <cstring>
#include <easylogging++.h>

INITIALIZE_EASYLOGGINGPP

namespace {

    class NamenodeTest : public ::testing::Test {

    protected:
        virtual void SetUp() {
            system("sudo ~/zookeeper/bin/zkServer.sh stop");
            system("sudo ~/zookeeper/bin/zkServer.sh start");
            int error_code;
            auto zk_shared = std::make_shared<ZKWrapper>("localhost:2181", error_code, "/testing");
            assert(error_code == 0); // Z_OK
            client = new zkclient::ZkNnClient(zk_shared);
            zk = new ZKWrapper("localhost:2181", error_code, "/testing");
        }

        virtual void TearDown() {

            // Code here will be called immediately after each test (right
            // before the destructor).
            std::string command("sudo ~/zookeeper/bin/zkCli.sh rmr /testing");
            // system(command.data());
            system("sudo ~/zookeeper/bin/zkServer.sh stop");
        }

        // Objects declared here can be used by all tests in the test case for Foo.
        ZKWrapper *zk;
        zkclient::ZkNnClient *client;
    };


    TEST_F(NamenodeTest, create){
        // ASSERT_EQ("ZCLOSING", zk->translate_error(-116));
    }

    TEST_F(NamenodeTest, findDataNodes){

        int error;
        zk->create("/health/localhost:2181", ZKWrapper::EMPTY_VECTOR, error);
        zk->create("/health/localhost:2181/health", ZKWrapper::EMPTY_VECTOR, error);
        zk->create("/health/localhost:2182", ZKWrapper::EMPTY_VECTOR, error);
        zk->create("/health/localhost:2182/health", ZKWrapper::EMPTY_VECTOR, error);

        auto datanodes = std::vector<std::string>();
        u_int64_t block_id;
        LOG(INFO) << "Finding dn's for block " << block_id;
        client->generate_block_UUID(block_id);
        client->find_datanode_for_block(datanodes, block_id, 1, true);

        for (auto datanode : datanodes) {
            LOG(INFO) << "Returned datanode " << datanode;
        }
        ASSERT_EQ(1, datanodes.size());
    }

    TEST_F(NamenodeTest, findDataNodesWithReplicas){
        // Check if we can find datanodes, without overlapping with ones that already contain a replica
    }

    TEST_F(NamenodeTest, checkAcks){
        // Check if check_acks works as intended
    }

}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
