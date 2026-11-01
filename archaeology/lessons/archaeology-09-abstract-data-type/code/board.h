#pragma once

#include <cstddef>

struct Board;

Board *board_create();
void board_destroy(Board *board);
void board_reset(Board *board);
void board_mark(Board *board, int row, int column);
bool board_is_visited(const Board *board, int row, int column);
std::size_t board_visited_count(const Board *board);
