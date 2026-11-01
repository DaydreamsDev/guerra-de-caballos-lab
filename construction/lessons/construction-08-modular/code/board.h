#pragma once

int board_row_count();
int board_column_count();
void board_reset();
bool board_was_visited(int row, int column);
void board_mark_visited(int row, int column);
