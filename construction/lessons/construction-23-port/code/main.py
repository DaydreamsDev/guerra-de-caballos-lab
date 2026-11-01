import argparse

from game import run_game


def parse_arguments() -> argparse.Namespace:
    argument_parser = argparse.ArgumentParser(
        description="Produce the deterministic transcript for one game."
    )
    argument_parser.add_argument(
        "--seed",
        type=int,
        required=True,
        metavar="INTEGER",
        help="Integer seed for the deterministic random generator.",
    )
    argument_parser.add_argument(
        "--name",
        type=str,
        required=True,
        metavar="TEXT",
        help="Player name written to the transcript.",
    )
    return argument_parser.parse_args()


def main() -> int:
    arguments = parse_arguments()
    print(run_game(arguments.seed, arguments.name), end="")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
