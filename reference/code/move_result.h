#pragma once

enum class MoveOutcome
{
    outsideBoard,
    visited,
    capture,
    legal
};

struct MoveResult
{
    MoveOutcome outcome;
    int destinationRow;
    int destinationColumn;
};
