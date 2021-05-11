#include "core/common.h"

int main()
{
    Lunar::print("Hello, {}!", "world");
    Lunar::error("I'd rather be {1} than {0}", "right", "happy");
    return 1;
}