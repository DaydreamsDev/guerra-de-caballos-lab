#include "board.h"

#include <cstdio>

int main()
{
    Board board;
    board.mark(1, 1);
    board.mark(2, 3);
    std::printf("visited=%zu first=%d second=%d\n", board.visited_count(),
                board.is_visited(1, 1) ? 1 : 0, board.is_visited(2, 3) ? 1 : 0);
    return 0;
}
