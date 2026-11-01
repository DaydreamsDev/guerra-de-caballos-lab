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

MOVE_NUMBER_MINIMUM = 1
MOVE_NUMBER_MAXIMUM = 8
ROLL_MINIMUM = 1
ROLL_MAXIMUM = 50
DEFAULT_MAXIMUM_MOVE_COUNT = 200

RANDOM_MULTIPLIER = 6364136223846793005
RANDOM_INCREMENT = 1442695040888963407
RANDOM_MASK = (1 << 64) - 1


class RandomSequence:
    state: int

    def __init__(self, seed: int) -> None:
        self.state = seed & RANDOM_MASK

    def below(self, bound: int) -> int:
        self.state = (
            self.state * RANDOM_MULTIPLIER + RANDOM_INCREMENT
        ) & RANDOM_MASK
        return self.state % bound

    def integer(self, minimum: int, maximum: int) -> int:
        return minimum + self.below(maximum - minimum + 1)


def format_position(position: tuple[int, int]) -> str:
    return f"{position[0]},{position[1]}"


def is_inside_board(position: tuple[int, int]) -> bool:
    row, column = position
    return (
        1 <= row <= BOARD_ROW_COUNT
        and 1 <= column <= BOARD_COLUMN_COUNT
    )


def convert_cell_number_to_position(cell_number: int) -> tuple[int, int]:
    row = cell_number // BOARD_COLUMN_COUNT + 1
    column = cell_number % BOARD_COLUMN_COUNT + 1
    return (row, column)


def create_initial_positions(
    random_sequence: RandomSequence,
) -> tuple[tuple[int, int], tuple[int, int]]:
    player_cell_number = random_sequence.below(BOARD_CELL_COUNT)
    machine_cell_number = random_sequence.below(BOARD_CELL_COUNT)
    while machine_cell_number == player_cell_number:
        machine_cell_number = random_sequence.below(BOARD_CELL_COUNT)
    return (
        convert_cell_number_to_position(player_cell_number),
        convert_cell_number_to_position(machine_cell_number),
    )


def create_initial_rolls(
    random_sequence: RandomSequence,
) -> tuple[int, int]:
    player_roll = random_sequence.integer(ROLL_MINIMUM, ROLL_MAXIMUM)
    machine_roll = random_sequence.integer(ROLL_MINIMUM, ROLL_MAXIMUM)
    while machine_roll == player_roll:
        machine_roll = random_sequence.integer(ROLL_MINIMUM, ROLL_MAXIMUM)
    return (player_roll, machine_roll)


def build_move_order(starting_move_number: int) -> list[int]:
    return [
        (starting_move_number - 1 + offset) % MOVE_NUMBER_MAXIMUM + 1
        for offset in range(MOVE_NUMBER_MAXIMUM)
    ]


def other_knight(active_knight: str) -> str:
    return "machine" if active_knight == "player" else "player"


def run_game(
    seed: int,
    player_name: str,
    maximum_move_count: int = DEFAULT_MAXIMUM_MOVE_COUNT,
) -> str:
    random_sequence = RandomSequence(seed)
    player_position, machine_position = create_initial_positions(random_sequence)
    player_roll, machine_roll = create_initial_rolls(random_sequence)

    starting_knight = "player" if player_roll > machine_roll else "machine"
    active_knight = starting_knight
    visited_positions: set[tuple[int, int]] = set()
    successful_move_count = 0
    winner_knight: str | None = None

    transcript_lines = [
        f"seed={seed}",
        f"player={player_name}",
        f"roll_player={player_roll}",
        f"roll_machine={machine_roll}",
        f"start={starting_knight}",
        f"initial_player={format_position(player_position)}",
        f"initial_machine={format_position(machine_position)}",
    ]

    while (
        winner_knight is None
        and successful_move_count < maximum_move_count
    ):
        turn_number = successful_move_count + 1
        starting_move_number = random_sequence.integer(
            MOVE_NUMBER_MINIMUM, MOVE_NUMBER_MAXIMUM
        )
        move_order = build_move_order(starting_move_number)
        other = other_knight(active_knight)
        active_position = (
            player_position if active_knight == "player" else machine_position
        )
        other_position = (
            machine_position if active_knight == "player" else player_position
        )
        order_text = ",".join(str(move_number) for move_number in move_order)
        transcript_lines.append(
            f"turn={turn_number} who={active_knight} "
            f"at={format_position(active_position)} order={order_text}"
        )

        move_was_accepted = False
        for attempted_move_number in move_order:
            row_change, column_change = KNIGHT_MOVE_CHANGES[
                attempted_move_number - 1
            ]
            destination = (
                active_position[0] + row_change,
                active_position[1] + column_change,
            )
            destination_text = format_position(destination)

            if not is_inside_board(destination):
                transcript_lines.append(
                    f"  try={attempted_move_number} to={destination_text} "
                    "illegal=outside"
                )
                continue

            if destination == other_position:
                transcript_lines.append(
                    f"  try={attempted_move_number} to={destination_text} "
                    f"capture={other}"
                )
                successful_move_count += 1
                transcript_lines.append(
                    f"move={successful_move_count} {active_knight} "
                    f"{format_position(active_position)}->{destination_text} "
                    "capture"
                )
                visited_positions.add(destination)
                winner_knight = active_knight
                move_was_accepted = True
                break

            if destination in visited_positions:
                transcript_lines.append(
                    f"  try={attempted_move_number} to={destination_text} "
                    "illegal=visited"
                )
                continue

            transcript_lines.append(
                f"  try={attempted_move_number} to={destination_text} legal"
            )
            successful_move_count += 1
            transcript_lines.append(
                f"move={successful_move_count} {active_knight} "
                f"{format_position(active_position)}->{destination_text}"
            )
            visited_positions.add(destination)
            if active_knight == "player":
                player_position = destination
            else:
                machine_position = destination
            active_knight = other
            move_was_accepted = True
            break

        if not move_was_accepted and winner_knight is None:
            transcript_lines.append(
                f"  no_legal_move trapped={active_knight}"
            )
            winner_knight = other

    if winner_knight is None:
        winner_knight = "draw"

    transcript_lines.append(f"winner={winner_knight}")
    transcript_lines.append(f"moves={successful_move_count}")
    return "\n".join(transcript_lines) + "\n"
