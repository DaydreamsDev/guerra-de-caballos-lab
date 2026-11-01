#include <cstdio>

int board_cell_index(int row, int column)
{
    return row * 8 + column;
}

int main()
{
    std::printf("index=%d\n", board_cell_index(2, 3));
    return 0;
}
