#include <gtest/gtest.h>
#include "TestCommand.h"

TEST(TestCommandTest, name_is_init_succeeded)
{
    Scat::TestCommand testCommand("start");

    auto name = testCommand.getName();

    ASSERT_EQ("start", name);
}

TEST(TestCommandTest, arguments_are_init_succeeded)
{
    Scat::TestCommand testCommand("include", {"filename"});

    const auto& arguments = testCommand.getArguments();

    ASSERT_EQ(std::vector<std::string> { "filename" }, arguments);
}
