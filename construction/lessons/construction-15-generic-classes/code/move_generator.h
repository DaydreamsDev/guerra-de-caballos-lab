#pragma once

#include <array>

constexpr int moveCandidateCount = 8;

constexpr int knightRowChangePerMove[moveCandidateCount] = {-1, -2, -2, -1, 1, 2, 2, 1};
constexpr int knightColumnChangePerMove[moveCandidateCount] = {2, 1, -1, -2, -2, -1, 1, 2};

struct MoveCandidate
{
    int moveNumber;
    int destinationRow;
    int destinationColumn;
};

template <typename PieceType>
std::array<MoveCandidate, moveCandidateCount> generate_move_candidates(const PieceType &piece)
{
    std::array<MoveCandidate, moveCandidateCount> candidates = {};
    for (int index = 0; index < moveCandidateCount; index += 1)
    {
        candidates[index] = {
            index + 1,
            piece.row() + knightRowChangePerMove[index],
            piece.column() + knightColumnChangePerMove[index],
        };
    }
    return candidates;
}
