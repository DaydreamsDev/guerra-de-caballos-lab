# Formato de transcripción

> [English](transcript-format.md) | [Español](transcript-format-es.md)

La transcripción es el comportamiento observable de una ejecución. Es texto plano, UTF-8, con un
registro por línea.

## Encabezado

```text
seed=<integer>
player=<name>
roll_player=<1..50>
roll_machine=<1..50>
start=<player|machine>
initial_player=<row>,<column>
initial_machine=<row>,<column>
```

## Turno

```text
turn=<move number> who=<player|machine> at=<row>,<column> order=<n,n,n,n,n,n,n,n>
```

El campo `order` enumera los ocho números de movimiento en el orden en que se probarán.

## Intentos

Una línea por cada movimiento intentado:

```text
  try=<move> to=<row>,<column> legal
  try=<move> to=<row>,<column> illegal=outside
  try=<move> to=<row>,<column> illegal=visited
  try=<move> to=<row>,<column> capture=<player|machine>
```

Los dos espacios iniciales forman parte del formato.

## Movimiento exitoso

Tras un movimiento legal, incluida una captura:

```text
move=<move number> <player|machine> <from-row>,<from-column>-><to-row>,<to-column>
```

Para una captura, añadir:

```text
move=<move number> <player|machine> <from-row>,<from-column>-><to-row>,<to-column> capture
```

## Trampa

Cuando el caballo activo no tiene ningún movimiento legal:

```text
  no_legal_move trapped=<player|machine>
```

No se registra ningún movimiento exitoso.

## Resultado

```text
winner=<player|machine|draw>
moves=<number of successful moves>
```

## Ejemplo

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

El ejemplo coincide con `common/traces/seed-0017.txt`. Las trazas bajo `common/traces/` son la
autoridad para sus semillas.
