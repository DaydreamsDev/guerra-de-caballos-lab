import argparse
import sys

BOARD_ROW_COUNT = 8
BOARD_COLUMN_COUNT = 8
BOARD_CELL_COUNT = BOARD_ROW_COUNT * BOARD_COLUMN_COUNT

KNIGHT_MOVE_CHANGES = (
    (-1, 2),
    (-2, 1),
    (-2, -1),
    (-1, -2),
    (1, -2),
    (2, -1),
    (2, 1),
    (1, 2),
)

MOVE_ORDER_MINIMUM = 1
MOVE_ORDER_MAXIMUM = 8

ROLL_MINIMUM = 1
ROLL_MAXIMUM = 50

DEFAULT_MAXIMUM_MOVE_COUNT = 200

RANDOM_MULTIPLIER = 6364136223846793005
RANDOM_INCREMENT = 1442695040888963407
RANDOM_MASK = (1 << 64) - 1


class DeterministicRandom:
    def __init__(self, seed):
        self.state = seed & RANDOM_MASK

    def next_value(self):
        self.state = (
            self.state * RANDOM_MULTIPLIER + RANDOM_INCREMENT
        ) & RANDOM_MASK
        return self.state

    def random_below(self, bound):
        return self.next_value() % bound

    def random_integer(self, minimum, maximum):
        return minimum + self.random_below(maximum - minimum + 1)


def convert_cell_number_to_position(cellNumber):
    row = cellNumber // BOARD_COLUMN_COUNT + 1
    column = cellNumber % BOARD_COLUMN_COUNT + 1
    return (row, column)


def format_position(position):
    return f"{position[0]},{position[1]}"


def position_is_inside_board(position):
    row, column = position
    return (
        1 <= row <= BOARD_ROW_COUNT
        and 1 <= column <= BOARD_COLUMN_COUNT
    )


def create_initial_positions(randomGenerator):
    playerCellNumber = randomGenerator.random_below(BOARD_CELL_COUNT)
    machineCellNumber = randomGenerator.random_below(BOARD_CELL_COUNT)
    while machineCellNumber == playerCellNumber:
        machineCellNumber = randomGenerator.random_below(BOARD_CELL_COUNT)
    playerPosition = convert_cell_number_to_position(playerCellNumber)
    machinePosition = convert_cell_number_to_position(machineCellNumber)
    return (playerPosition, machinePosition)


def create_initial_rolls(randomGenerator):
    playerRoll = randomGenerator.random_integer(ROLL_MINIMUM, ROLL_MAXIMUM)
    machineRoll = randomGenerator.random_integer(ROLL_MINIMUM, ROLL_MAXIMUM)
    while machineRoll == playerRoll:
        machineRoll = randomGenerator.random_integer(ROLL_MINIMUM, ROLL_MAXIMUM)
    return (playerRoll, machineRoll)


def build_move_order(startingMoveNumber):
    return [
        (startingMoveNumber - 1 + offset) % MOVE_ORDER_MAXIMUM + 1
        for offset in range(MOVE_ORDER_MAXIMUM)
    ]


def determine_other_knight(activeKnight):
    return "machine" if activeKnight == "player" else "player"


def build_transcript(seed, playerName, maximumMoveCount):
    randomGenerator = DeterministicRandom(seed)

    playerPosition, machinePosition = create_initial_positions(randomGenerator)
    playerRoll, machineRoll = create_initial_rolls(randomGenerator)

    startingKnight = "player" if playerRoll > machineRoll else "machine"
    activeKnight = startingKnight
    visitedPositions = set()
    successfulMoveCount = 0
    winnerKnight = None

    transcriptLines = [
        f"seed={seed}",
        f"player={playerName}",
        f"roll_player={playerRoll}",
        f"roll_machine={machineRoll}",
        f"start={startingKnight}",
        f"initial_player={format_position(playerPosition)}",
        f"initial_machine={format_position(machinePosition)}",
    ]

    while winnerKnight is None and successfulMoveCount < maximumMoveCount:
        turnNumber = successfulMoveCount + 1
        startingMoveNumber = randomGenerator.random_integer(
            MOVE_ORDER_MINIMUM, MOVE_ORDER_MAXIMUM
        )
        moveOrder = build_move_order(startingMoveNumber)

        otherKnight = determine_other_knight(activeKnight)
        activePosition = (
            playerPosition if activeKnight == "player" else machinePosition
        )
        otherPosition = (
            machinePosition if activeKnight == "player" else playerPosition
        )

        orderText = ",".join(str(moveNumber) for moveNumber in moveOrder)
        transcriptLines.append(
            f"turn={turnNumber} who={activeKnight} "
            f"at={format_position(activePosition)} order={orderText}"
        )

        successfulMoveThisTurn = False

        for attemptedMoveNumber in moveOrder:
            rowChange, columnChange = KNIGHT_MOVE_CHANGES[attemptedMoveNumber - 1]
            destinationPosition = (
                activePosition[0] + rowChange,
                activePosition[1] + columnChange,
            )
            destinationText = format_position(destinationPosition)

            if not position_is_inside_board(destinationPosition):
                transcriptLines.append(
                    f"  try={attemptedMoveNumber} to={destinationText} illegal=outside"
                )
                continue

            if destinationPosition == otherPosition:
                transcriptLines.append(
                    f"  try={attemptedMoveNumber} to={destinationText} capture={otherKnight}"
                )
                successfulMoveCount += 1
                transcriptLines.append(
                    f"move={successfulMoveCount} {activeKnight} "
                    f"{format_position(activePosition)}->{destinationText} capture"
                )
                visitedPositions.add(destinationPosition)
                winnerKnight = activeKnight
                successfulMoveThisTurn = True
                break

            if destinationPosition in visitedPositions:
                transcriptLines.append(
                    f"  try={attemptedMoveNumber} to={destinationText} illegal=visited"
                )
                continue

            transcriptLines.append(
                f"  try={attemptedMoveNumber} to={destinationText} legal"
            )
            successfulMoveCount += 1
            transcriptLines.append(
                f"move={successfulMoveCount} {activeKnight} "
                f"{format_position(activePosition)}->{destinationText}"
            )
            visitedPositions.add(destinationPosition)

            if activeKnight == "player":
                playerPosition = destinationPosition
            else:
                machinePosition = destinationPosition

            activeKnight = otherKnight
            successfulMoveThisTurn = True
            break

        if not successfulMoveThisTurn and winnerKnight is None:
            transcriptLines.append(f"  no_legal_move trapped={activeKnight}")
            winnerKnight = otherKnight

    if winnerKnight is None:
        winnerKnight = "draw"

    transcriptLines.append(f"winner={winnerKnight}")
    transcriptLines.append(f"moves={successfulMoveCount}")

    return "\n".join(transcriptLines) + "\n"


def parse_arguments(arguments):
    argumentParser = argparse.ArgumentParser(
        description="Produce the deterministic transcript for one game."
    )
    argumentParser.add_argument(
        "--seed",
        type=int,
        required=True,
        metavar="INTEGER",
        help="Integer seed for the deterministic random generator.",
    )
    argumentParser.add_argument(
        "--name",
        type=str,
        required=True,
        metavar="TEXT",
        help="Player name written to the transcript header.",
    )
    argumentParser.add_argument(
        "--max-moves",
        dest="maximumMoveCount",
        type=int,
        default=DEFAULT_MAXIMUM_MOVE_COUNT,
        metavar="INTEGER",
        help="Maximum number of successful moves before a draw.",
    )
    parsedArguments = argumentParser.parse_args(arguments)
    if parsedArguments.maximumMoveCount < 0:
        argumentParser.error("--max-moves must be zero or greater")
    return parsedArguments


def main(arguments=None):
    parsedArguments = parse_arguments(arguments)
    transcript = build_transcript(
        parsedArguments.seed,
        parsedArguments.name,
        parsedArguments.maximumMoveCount,
    )
    sys.stdout.write(transcript)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
