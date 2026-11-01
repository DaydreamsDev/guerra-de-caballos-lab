#include "board.h"

#include <cstdio>

int main()
{
    Board *board = board_create();
    board_mark(board, 1, 1);
    board_mark(board, 2, 3);
    std::printf("visited=%zu first=%d second=%d\n", board_visited_count(board),
                board_is_visited(board, 1, 1) ? 1 : 0,
                board_is_visited(board, 2, 3) ? 1 : 0);
    board_destroy(board);
    return 0;
}
