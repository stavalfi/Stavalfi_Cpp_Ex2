
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */


int main(int argc, char *argv[]) {
    srand (time(NULL));
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}