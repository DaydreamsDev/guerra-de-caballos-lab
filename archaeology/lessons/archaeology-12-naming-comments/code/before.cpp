#include <cstdio>

// calcula el indice del tablero
int calc(int f, int c)
{
    return f * 8 + c;
}

int main()
{
    std::printf("index=%d\n", calc(2, 3));
    return 0;
}
