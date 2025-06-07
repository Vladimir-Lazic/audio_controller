#include "Logger.h"
#include "TimeUtils.h"
#include "gtest/gtest.h"

class UtilsTestFixture : public ::testing::Test {
protected:
    void SetUp() override { };
    void TearDown() override { };
};

TEST_F(UtilsTestFixture, TestLogger)
{
    Logger logger("./test_log.txt");
    logger.log("INFO", "Test Message");

    std::ifstream logFile("./test_log.txt");

    std::string line;
    bool found = false;
    while (std::getline(logFile, line)) {
        if (line.find("Test Message") != std::string::npos) {
            found = true;
            break;
        }
    }

    ASSERT_TRUE(found);
}

TEST_F(UtilsTestFixture, TestTimeUtils)
{
    std::string time = TimeUtils::getCurrentTimeStamp();
    ASSERT_TRUE(!time.empty());
}