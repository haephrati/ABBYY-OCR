#include <cstdio>
#include <cstdlib>
#include <string>
using std::string;
#include "utils.h"

static void Fail(const char* msg)
{
    std::printf("FAIL %s\n", msg);
    std::exit(1);
}

int main()
{
    if (LOG_COLOR_GREEN != 10)
        Fail("LOG_COLOR_GREEN");
    if (LOG_COLOR_YELLOW != 14)
        Fail("LOG_COLOR_YELLOW");
    if (LOG_COLOR_WHITE != 7)
        Fail("LOG_COLOR_WHITE");
    std::printf("OK AbbyyOcrTests\n");
    return 0;
}
