#include "WaveformGenerator.h"
#include <gtest/gtest.h>

class WaveformGeneratorTestFixture : public ::testing::Test {
protected:
    void SetUp() override { };
    void TearDown() override { };
public:
    WaveformGenerator wf_gen;
};

TEST_F(WaveformGeneratorTestFixture, CanCreateObject)
{
    WaveformGenerator local_wfgen;
}

TEST_F(WaveformGeneratorTestFixture, CanGenerateSine)
{

}
