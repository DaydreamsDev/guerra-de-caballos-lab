#include "platform.h"

#include <cstdio>

void set_console_color()
{
    std::printf("\033[1;37;44m");
}

void clear_console()
{
    std::printf("\033[2J\033[H");
}

int read_console_key()
{
    return std::getchar();
}
