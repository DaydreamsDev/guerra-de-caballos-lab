#pragma once

#include <algorithm>
#include <array>
#include <ranges>

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
    std::ranges::transform(
        std::views::iota(0, moveCandidateCount), candidates.begin(),
        [&piece](int index)
        {
            return MoveCandidate{
                index + 1,
                piece.row() + knightRowChangePerMove[index],
                piece.column() + knightColumnChangePerMove[index],
            };
        });
    return candidates;
}
