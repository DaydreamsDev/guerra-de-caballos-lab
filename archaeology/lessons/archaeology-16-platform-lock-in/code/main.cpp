#include "platform.h"

#include <cstdio>

int main()
{
    set_console_color();
    clear_console();
    std::printf("portable=yes\n");
    return 0;
}
