#pragma once

struct Board;

Board *board_create();
void board_destroy(Board *board);
int board_row_count(const Board *board);
int board_column_count(const Board *board);
bool board_is_valid(const Board *board);
void board_reset(Board *board);
bool board_was_visited(const Board *board, int row, int column);
void board_mark_visited(Board *board, int row, int column);
