#include <array>
#include <cstdio>
#include <memory>

struct Position
{
    int row;
    int column;

    bool operator==(const Position &other) const = default;
};

struct Delta
{
    int rowChange;
    int columnChange;
};

enum class PieceKind
{
    knight,
    mirror
};

constexpr std::array<Delta, 8> knightDeltas = {
    Delta{-1, 2}, Delta{-2, 1}, Delta{-2, -1}, Delta{-1, -2},
    Delta{1, -2}, Delta{2, -1}, Delta{2, 1},   Delta{1, 2},
};

constexpr std::array<Delta, 8> mirrorDeltas = {
    Delta{1, -2}, Delta{2, -1}, Delta{2, 1},   Delta{1, 2},
    Delta{-1, 2}, Delta{-2, 1}, Delta{-2, -1}, Delta{-1, -2},
};

class Piece
{
public:
    virtual ~Piece() = default;
    virtual Position move(int moveNumber, Position position) const = 0;
    virtual const char *role_name() const = 0;
};

class Knight : public Piece
{
public:
    Position move(int moveNumber, Position position) const override
    {
        const Delta &delta = knightDeltas[moveNumber - 1];
        return Position{position.row + delta.rowChange, position.column + delta.columnChange};
    }

    const char *role_name() const override
    {
        return "knight";
    }
};

class MirrorKnight : public Piece
{
public:
    Position move(int moveNumber, Position position) const override
    {
        const Delta &delta = mirrorDeltas[moveNumber - 1];
        return Position{position.row + delta.rowChange, position.column + delta.columnChange};
    }

    const char *role_name() const override
    {
        return "mirror";
    }
};

Delta delta_with_switch(PieceKind kind, int moveNumber)
{
    switch (kind)
    {
    case PieceKind::knight:
        switch (moveNumber)
        {
        case 1:
            return Delta{-1, 2};
        case 2:
            return Delta{-2, 1};
        case 3:
            return Delta{-2, -1};
        case 4:
            return Delta{-1, -2};
        case 5:
            return Delta{1, -2};
        case 6:
            return Delta{2, -1};
        case 7:
            return Delta{2, 1};
        case 8:
            return Delta{1, 2};
        default:
            return Delta{0, 0};
        }
    case PieceKind::mirror:
        switch (moveNumber)
        {
        case 1:
            return Delta{1, -2};
        case 2:
            return Delta{2, -1};
        case 3:
            return Delta{2, 1};
        case 4:
            return Delta{1, 2};
        case 5:
            return Delta{-1, 2};
        case 6:
            return Delta{-2, 1};
        case 7:
            return Delta{-2, -1};
        case 8:
            return Delta{-1, -2};
        default:
            return Delta{0, 0};
        }
    }
    return Delta{0, 0};
}

Position move_with_switch(PieceKind kind, int moveNumber, Position position)
{
    Delta delta = delta_with_switch(kind, moveNumber);
    return Position{position.row + delta.rowChange, position.column + delta.columnChange};
}

int main()
{
    std::unique_ptr<Piece> knight = std::make_unique<Knight>();
    std::unique_ptr<Piece> mirror = std::make_unique<MirrorKnight>();
    std::array<Piece *, 2> pieces = {knight.get(), mirror.get()};
    std::array<PieceKind, 2> kinds = {PieceKind::knight, PieceKind::mirror};

    int comparisonCount = 0;
    int mismatchCount = 0;
    for (int pieceIndex = 0; pieceIndex < 2; pieceIndex += 1)
    {
        for (int moveNumber = 1; moveNumber <= 8; moveNumber += 1)
        {
            for (int row = -2; row <= 2; row += 1)
            {
                for (int column = -2; column <= 2; column += 1)
                {
                    Position position{row, column};
                    Position expected = move_with_switch(kinds[pieceIndex], moveNumber,
                                                         position);
                    Position actual = pieces[pieceIndex]->move(moveNumber, position);
                    comparisonCount += 1;
                    if (!(expected == actual))
                    {
                        mismatchCount += 1;
                    }
                }
            }
        }
    }
    std::printf("comparisons=%d mismatches=%d\n", comparisonCount, mismatchCount);
    return mismatchCount == 0 ? 0 : 1;
}
