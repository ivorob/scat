#include <gtest/gtest.h>

#if defined(_WIN32) || defined(_WIN64)
#include <winsock2.h>
#endif

int
main(int argc, char *argv[])
{
#if defined(_WIN32) || defined(_WIN64)
    WSAData data;
    WSAStartup(MAKEWORD(2, 2), &data);
#endif

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
