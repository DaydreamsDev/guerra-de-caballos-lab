# Transcript format

> [English](transcript-format.md) | [Español](transcript-format-es.md)

The transcript is the observable behavior of one run. It is plain text, UTF-8, one record per line.

## Header

```text
seed=<integer>
player=<name>
roll_player=<1..50>
roll_machine=<1..50>
start=<player|machine>
initial_player=<row>,<column>
initial_machine=<row>,<column>
```

## Turn

```text
turn=<move number> who=<player|machine> at=<row>,<column> order=<n,n,n,n,n,n,n,n>
```

The `order` field lists the eight move numbers in the order they will be tried.

## Attempts

One line per attempted move:

```text
  try=<move> to=<row>,<column> legal
  try=<move> to=<row>,<column> illegal=outside
  try=<move> to=<row>,<column> illegal=visited
  try=<move> to=<row>,<column> capture=<player|machine>
```

The two leading spaces are part of the format.

## Successful move

After a legal move, including a capture:

```text
move=<move number> <player|machine> <from-row>,<from-column>-><to-row>,<to-column>
```

For a capture, append:

```text
move=<move number> <player|machine> <from-row>,<from-column>-><to-row>,<to-column> capture
```

## Trap

When the active knight has no legal move:

```text
  no_legal_move trapped=<player|machine>
```

No successful move is recorded.

## Result

```text
winner=<player|machine|draw>
moves=<number of successful moves>
```

## Example

```text
seed=17
player=Learner
roll_player=17
roll_machine=34
start=machine
initial_player=2,5
initial_machine=6,4
turn=1 who=machine at=6,4 order=1,2,3,4,5,6,7,8
  try=1 to=5,6 legal
move=1 machine 6,4->5,6
turn=2 who=player at=2,5 order=8,1,2,3,4,5,6,7
  try=8 to=3,7 legal
move=2 player 2,5->3,7
turn=3 who=machine at=5,6 order=3,4,5,6,7,8,1,2
  try=3 to=3,5 legal
move=3 machine 5,6->3,5
turn=4 who=player at=3,7 order=2,3,4,5,6,7,8,1
  try=2 to=1,8 legal
move=4 player 3,7->1,8
turn=5 who=machine at=3,5 order=5,6,7,8,1,2,3,4
  try=5 to=4,3 legal
move=5 machine 3,5->4,3
turn=6 who=player at=1,8 order=4,5,6,7,8,1,2,3
  try=4 to=0,6 illegal=outside
  try=5 to=2,6 legal
move=6 player 1,8->2,6
turn=7 who=machine at=4,3 order=7,8,1,2,3,4,5,6
  try=7 to=6,4 legal
move=7 machine 4,3->6,4
turn=8 who=player at=2,6 order=6,7,8,1,2,3,4,5
  try=6 to=4,5 legal
move=8 player 2,6->4,5
turn=9 who=machine at=6,4 order=1,2,3,4,5,6,7,8
  try=1 to=5,6 illegal=visited
  try=2 to=4,5 capture=player
move=9 machine 6,4->4,5 capture
winner=machine
moves=9
```

The example matches `common/traces/seed-0017.txt`. The traces under `common/traces/` are
authoritative for their seeds.
