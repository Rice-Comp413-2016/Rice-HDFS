
#include "zk_dn_client.h"
#include "zk_client_common.h"
#include "zkwrapper.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <easylogging++.h>
INITIALIZE_EASYLOGGINGPP
using ::testing::AtLeast;

using namespace zkclient;
class ZKDNClientTest : public ::testing::Test{
protected:
        virtual void SetUp(){
                block_id = 12345;
                block_size = 54321;
                xferPort = 50010;
                ipcPort = 50020;
                int error_code = 0;
                zk = std::make_shared<ZKWrapper>("localhost:2181", error_code, "/testing");
                ASSERT_EQ("ZOK", zk->translate_error(error_code)); // Z_OK
                client = new ZkClientDn("127.0.0.1", zk, block_size * 10, ipcPort, xferPort);
                dn_id = "127.0.0.1:50020";
        }
        virtual void TearDown() {
                system("sudo ~/zookeeper/bin/zkCli.sh rmr /testing");
        }
        uint64_t block_id;
        uint64_t block_size;
        unsigned short xferPort;
        unsigned short ipcPort;
        std::shared_ptr <ZKWrapper> zk;
        std::string dn_id;
        ZkClientDn *client;

};

TEST_F(ZKDNClientTest, RegisterMakesWorkQueues){
    bool exists;
    int error_code;
    uint64_t size;

    std::vector<std::uint8_t> data(sizeof(BlockZNode));
    std::string path = "/work_queues/replicate/" + dn_id;

        ASSERT_TRUE(zk->get(path, data, error_code));
 }


TEST_F(ZKDNClientTest, CanReadBlockSize) {
        bool exists;
        int error_code;
        uint64_t size;

        std::string path = "/block_locations/" + std::to_string(block_id);
        std::vector<std::uint8_t> data(sizeof(BlockZNode));
        zk->create(path, ZKWrapper::EMPTY_VECTOR, error_code);

        client->blockReceived(block_id, block_size);

        ASSERT_TRUE(zk->get(path, data, error_code));
        memcpy(&size, &data[0], sizeof(data));
        ASSERT_EQ(block_size, size);
}

TEST_F(ZKDNClientTest, CanDeleteBlock) {
        bool exists;
        int error_code;
        uint64_t size;

        std::string path = "/block_locations/" + std::to_string(block_id);
        std::vector<std::uint8_t> data(sizeof(BlockZNode));

        zk->create(path, ZKWrapper::EMPTY_VECTOR, error_code);

        client->blockReceived(block_id, block_size);
        ASSERT_TRUE(zk->get(path + "/" + dn_id, data, error_code));

        client->blockDeleted(block_id);
        ASSERT_FALSE(zk->get(path + "/" + dn_id, data, error_code));

        // Check that /block_reports/ znode also deleted if no children
        ASSERT_FALSE(zk->get(path, data, error_code));
}
