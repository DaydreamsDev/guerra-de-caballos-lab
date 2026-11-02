# archaeology-09 — Tipo abstracto de datos ausente

> [English](lesson.md) | [Español](lesson-es.md)

> **Estado:** borrador · **Concepto:** tipo abstracto de datos · **Prerrequisito:** archaeology-08 ·
> **Tiempo estimado:** 90 minutos
> **Lectura:** ~18 minutos
> **Rol de la lección:** tronco

## Por qué existe esta lección

`archaeology-08` terminó con una cabecera. Un llamador ya no podía nombrar `visited[0]`, y el
enlazador unió dos unidades de traducción. Pero la cabecera seguía conteniendo esta línea:

```cpp
bool visited[cellCount];
```

La representación seguía en la compilación del llamador. Cambiarla por una máscara de 64 bits
obligaba a recompilar a todos los llamadores, porque el tamaño y la disposición del objeto
cambiaban. La frontera compró control de acceso; no compró independencia de representación.

El archivo intentó arreglar eso con un tipo. En `variant-03`:

```text
34:  struct stablero {
35:
36:      int  mvectab;
37:  };
...
42:  struct stablero vectab[64];
44:  struct stablero *pvectab,*pvectab1,*pvectab2;
```

`variant-04` hace lo mismo con otro nombre:

```text
44:  struct  tab {
45:      int  vectab;
46:
47:  };
48:  struct tab tablero[64];
49:  struct tab *ptablero,*ptablero1,*ptablero2;
```

Eso es una estructura con un entero dentro. El compilador la acepta como tipo. El diseño no: la
estructura no tiene operaciones, ni invariante, ni representación oculta. Un llamador sigue
escribiendo `pvectab->mvectab = 0;` y sigue sabiendo que una celda es un `int`. En `variant-03`,
102 líneas mencionan `mvectab`. El envoltorio renombró el tipo y no cambió nada del acoplamiento.

La traducción mecánica se ve en el código. `vectab[i]=0;` pasó a `pvectab->mvectab=0;`
(variant-03, línea 86; variant-04, línea 92). El puntero se movió; el acoplamiento se quedó.

Un **tipo abstracto de datos** (TAD) es un conjunto de valores con un conjunto de operaciones,
donde la representación no forma parte del contrato. La única prueba honesta es un intercambio:
reemplazar la representación, conservar la interfaz y ejecutar el mismo llamador. El código de
2008 no puede pasar esa prueba, porque no hay interfaz que conservar. El campo es la interfaz.

Esta lección construye la interfaz. El módulo incluido es la referencia: una cabecera de 12 líneas
que declara un tipo incompleto y seis operaciones, dos implementaciones de unas 60 líneas cada una,
y un llamador de 15 líneas que incluye solo la cabecera.

La lección también es honesta sobre el precio. La API estilo C de `create`/`destroy` compra
independencia de representación y paga con ciclo de vida manual. Ese intercambio es la segunda
mitad de la lección.

## Qué podrás hacer

Al terminar esta lección puedes:

- explicar la diferencia entre un tipo y su representación;
- leer una declaración adelantada y decir qué sabe el compilador de un tipo incompleto;
- escribir un `Board` opaco cuya definición vive en un solo archivo de implementación;
- mostrar que el mismo llamador y la misma cabecera compilan contra dos representaciones;
- capturar al compilador negándose a medir, reservar o inspeccionar un tipo incompleto;
- explicar por qué una estructura con un campo no es un tipo abstracto de datos;
- listar los fallos de ciclo de vida que una API create/destroy deja al llamador;
- comparar la API opaca de funciones libres con una clase de C++ y un pimpl, con costes;
- enunciar qué prueba el intercambio y qué no prueba;
- mantener el archivo de solo lectura mientras trabajas.

El tipo está terminado cuando un llamador puede compilarse
solo con la cabecera y enlazarse con cualquiera de las dos implementaciones sin una edición.

## El concepto: tipo abstracto de datos

### Un tipo son operaciones, no disposición

Un tipo responde dos preguntas: qué valores existen y qué se puede hacer con ellos. Una
representación responde una tercera: cómo se almacenan esos valores. Un TAD mantiene la tercera
respuesta fuera del contrato.

| Pregunta | Quién la responde | Dónde pertenece |
|---|---|---|
| ¿Qué valores existen? | El tipo | La cabecera, en prosa o en las operaciones. |
| ¿Qué puedo hacer con un valor? | Las operaciones | La cabecera. |
| ¿Cómo se almacena el valor? | La implementación | El `.cpp`. |
| ¿Qué debe ser siempre cierto? | El invariante | Las operaciones, y más tarde la clase. |

En el archivo, la única forma de tocar una celda del tablero es nombrar `mvectab`. El nombre del
campo es la interfaz, así que la representación es el contrato. Ese es el defecto, y es un defecto
de diseño: el programa funciona.

### El tipo opaco

C++ permite que una cabecera nombre un tipo sin definirlo:

```cpp
struct Board;
```

Esto es una **declaración adelantada**. Crea un **tipo incompleto**: el compilador conoce el
nombre, que es una estructura y que vive en alguna unidad de traducción. No conoce el tamaño, los
miembros ni la disposición.

La definición va en exactamente un archivo de implementación:

```cpp
struct Board
{
    bool visited[64];
};
```

El llamador incluye solo la cabecera. El resultado es un tipo que el llamador puede sostener, pasar
y devolver por puntero, y nunca inspeccionar.

| El llamador puede | El llamador no puede | Qué dice el compilador |
|---|---|---|
| Declarar `Board *` | `sizeof(Board)` | `invalid application of 'sizeof' to incomplete type 'Board'` |
| Pasar `Board *` a una operación | Declarar `Board board;` | `aggregate 'Board board' has incomplete type and cannot be defined` |
| Comparar dos valores `Board *` | Leer `board->visited` | `invalid use of incomplete type 'struct Board'` |
| Guardar el puntero | `delete board;` | aviso `-Wdelete-incomplete`; no se llama al destructor |

La cuarta fila es la que sorprende. `delete` sobre un tipo incompleto es un aviso, no un error. Con
`-Werror` detiene la compilación; sin `-Werror` compila y se salta el destructor. Eso es
comportamiento indefinido con un mensaje amable.

### La prueba del intercambio

El criterio de aceptación es un cambio, no una forma:

1. conserva `board.h` idéntico byte a byte;
2. conserva `main.cpp` idéntico byte a byte;
3. enlaza `main.cpp` con `board_array.cpp`, ejecútalo y guarda la salida;
4. enlaza el mismo `main.cpp` con `board_bitmask.cpp`, ejecútalo y compara.

Si las salidas coinciden, el llamador depende de las operaciones y no de la representación. Eso es
lo que compra un TAD, y es la única afirmación que hace la lección.

| Qué cambia el intercambio | Qué no cambia |
|---|---|
| El archivo de implementación en la línea de enlace | La cabecera |
| El tamaño y la disposición del objeto | La fuente del llamador |
| El algoritmo dentro de cada operación | La salida del llamador |
| El número de bytes reservados | Las firmas de las operaciones |

### Qué le falta al envoltorio del archivo

Una `struct` con un campo es un **envoltorio transparente**. Es un tipo a los ojos del compilador y
un renombrado en el diseño. Tres pruebas lo separan de un TAD:

| Prueba | `struct stablero { int mvectab; }` | Un `Board` opaco |
|---|---|---|
| ¿Puede un llamador nombrar el almacenamiento? | Sí: `pvectab->mvectab` | No: `board_mark(board, 1, 1)` |
| ¿Puede cambiar la representación sin tocar llamadores? | No: 102 líneas mencionan `mvectab` | Sí: editas un `.cpp` |
| ¿Tiene operaciones el tipo? | No, solo un campo | Sí, seis funciones |

`struct svecjugadas { int mjugadas; }` falla las mismas tres pruebas para la lista de jugadas. El
archivo tiene dos envoltorios y cero abstracciones.

### Tres formas de ocultar una representación en C++

| Forma | ¿Representación oculta? | Ciclo de vida | Coste principal |
|---|---|---|---|
| Funciones libres sobre una estructura opaca (esta lección) | Sí | Manual `create` / `destroy` | Las fugas y dobles liberaciones son del llamador |
| Clase con miembros privados (`archaeology-08`) | No; la disposición está en la cabecera | Automático, por valor | El llamador recompila cuando cambia la disposición |
| Clase con pimpl | Sí | Automático, RAII | Almacenamiento en montículo para `Impl`, indirección, destructor fuera de línea |

La primera fila es el manejador estilo C. La segunda es lo que construyó la lección 08. La tercera
es la respuesta habitual de C++, y es la comparación que esta lección te debe.

### El coste de la API create/destroy

Seis funciones, sin estado oculto, ABI estable, usable desde C. Eso es valor real. El precio es que
toda regla sobre el ciclo de vida pasa del compilador al llamador:

| Fallo | Qué ocurre | Quién lo detecta |
|---|---|---|
| Olvidar `board_destroy` | La reserva se fuga | Nadie, salvo un sanitizador o un perfilador |
| Llamar `board_destroy` dos veces | Doble liberación; comportamiento indefinido | Nadie |
| Copiar el manejador: `Board *b2 = b;` | Dos dueños, una reserva | Nadie |
| Volver antes o lanzar entre create y destroy | Fuga en ese camino | Nadie |
| Pasar `nullptr` a `board_mark` | Desreferencia de nulo | Nadie; la API no lo dice |
| Usar el manejador tras destruirlo | Uso después de liberar | Nadie |
| Comparar dos manejadores | Identidad, no igualdad de tableros | Nadie; la igualdad no está en el contrato |

Solo `board_destroy(nullptr)` es seguro por accidente, porque `delete nullptr` no hace nada. Nada
en la cabecera lo dice. La cabecera tampoco dice si `board_create` puede fallar. No puede devolver
nulo, porque `new` lanza `std::bad_alloc`; pero el llamador tiene que saberlo por la
implementación, no por el contrato.

Hay un segundo coste fácil de pasar por alto: **no hay semántica de valor**. Un llamador no puede
devolver un `Board` por valor, meter uno en un `std::vector` sin un envoltorio, comparar dos
tableros ni copiar uno. El manejador es una identidad, no un valor. El tablero es un valor en el
dominio del problema.

### El intercambio del pimpl

La respuesta de C++ al mismo problema es una clase cuyo estado es un puntero a una implementación
incompleta:

```cpp
// board.h
#pragma once
#include <cstddef>
#include <memory>

class Board
{
public:
    Board();
    ~Board();
    Board(const Board &other);
    Board &operator=(const Board &other);
    Board(Board &&other) noexcept;
    Board &operator=(Board &&other) noexcept;

    void reset();
    void mark(int row, int column);
    bool is_visited(int row, int column) const;
    std::size_t visited_count() const;

private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};
```

El destructor, las operaciones de copia y las de movimiento deben declararse y definirse **fuera de
línea**, en el `.cpp`, donde `Impl` está completo. Esa es la regla de cinco llegando como factura.

| Criterio | Manejador opaco | Clase + pimpl |
|---|---|---|
| Representación oculta al llamador | Sí | Sí |
| El llamador recompila cuando cambia la representación | No | No |
| El ciclo de vida es automático | No | Sí |
| Copiable como valor | No | Sí, si escribes la copia |
| Reservas en montículo por tablero | Una | Una para `Impl`; el `Board` puede ser un valor |
| Indirección por operación | Una | Dos: la llamada y el acceso al `Impl` |
| Tamaño de la cabecera | 12 líneas | 25 líneas, cinco de ellas miembros especiales |
| Interoperabilidad con C | Sí | No |
| Estabilidad de ABI | Solo las operaciones | Solo las operaciones |

Ninguna opción gana. El manejador es más pequeño y compatible con C; el pimpl es más seguro y más
caro. Elige por lo que el llamador debe poder hacer, no por lo que parece moderno.

> **Una trampa que conviene conocer.** Una fábrica que devuelve `std::unique_ptr<Board>` sobre un
> `Board` incompleto no compila en el llamador. El destructor de `unique_ptr` necesita
> `sizeof(Board)`, y el llamador no lo tiene. El error de referencia es
> `invalid application of 'sizeof' to incomplete type 'Board'` dentro de `std::default_delete`.
> La solución es un borrador personalizado o el pimpl de arriba, no un cast.

## Antes de empezar

Lee esto completo:

- [`README-es.md`](README-es.md) — la ficha de la lección y el criterio de aceptación.
- [archaeology-08](../archaeology-08-module-boundary/lesson-es.md) — el prerrequisito: la frontera
  que esta lección vuelve opaca.
- [`../../../docs/guide/archaeology-track-es.md`](../../../docs/guide/archaeology-track-es.md) — §3,
  la taxonomía de fallos; §8, la secuencia de lecciones; §9, el mapeo a la pista A.
- [`../../../docs/guide/lesson-catalog-es.md`](../../../docs/guide/lesson-catalog-es.md) — la fila
  `archaeology-09` y su criterio de aceptación.
- [`../../../docs/archive-es.md`](../../../docs/archive-es.md) — el archivo fijado, etiqueta y
  commit.

También necesitas:

- un clon local del archivo en la etiqueta `v1.0.0`;
- un compilador de C++20 (`c++`, `g++` o `clang++`);
- un directorio de trabajo temporal fuera del archivo y fuera de los archivos versionados de este
  repositorio.

> **Regla:** el archivo es de solo lectura. Copia lo que necesites al directorio temporal; nunca
> edites `original/`.

## El camino de 90 minutos

| Min | Paso | Qué haces | Evidencia |
|---:|---|---|---|
| 0–15 | Predecir | Predices qué arregla un tipo opaco y qué cuesta | Tabla de predicción |
| 15–35 | Inspeccionar | Lees los envoltorios del archivo y el módulo incluido | Conteos y salida |
| 35–55 | Ocultar | Reconstruyes la cabecera como declaración adelantada más operaciones | Tu `board.h` |
| 55–75 | Intercambiar | Compilas el mismo llamador contra dos implementaciones | Dos ejecuciones, una salida |
| 75–85 | Explicar | Pones precio a la API C y la comparas con un pimpl | Tabla de intercambio |
| 85–90 | Registrar | Rellenas `EVIDENCE-es.md`; confirmas que el archivo está limpio | Entrada de portafolio |

Si el intercambio tarda diez minutos en enlazar, eso es información
sobre tu cadena de herramientas, no un fallo.

### La comprobación de ingeniería en esta lección

La pista B usa el mismo hábito que la pista A con otro verbo: el módulo de referencia existe, así
que lo **reconstruyes** y después lo **intercambias**.

| Paso | Dónde ocurre | Qué produce |
|---|---|---|
| **Predecir** | Paso 1 | Una afirmación falsable sobre qué arregla un tipo opaco. |
| **Inspeccionar** | Paso 2 | Los conteos de los envoltorios y la superficie incluida. |
| **Ocultar** | Paso 3 | Tu propia cabecera opaca y una implementación. |
| **Intercambiar** | Paso 4 | Dos líneas de enlace, un llamador, salida idéntica. |
| **Explicar** | Paso 5 | La tabla de conocimiento del llamador y la de costes. |
| **Registrar** | Paso 6 | La nota de TAD y `EVIDENCE-es.md`. |

## Paso 1 — Predecir (15 min)

Antes de leer `code/board.h`, escribe cinco frases. Cada una debe ser falsable: debes poder decir
qué observación demostraría que está equivocada.

1. En `variant-03`, ¿cuántas líneas mencionan el campo `mvectab` del tablero? Da un número.
2. Si `Board` pasa a ser un tipo opaco, ¿cuántas líneas de `main.cpp` deben cambiar?
3. ¿Qué rechazará el compilador que la versión con clase de `archaeology-08` permitía?
4. ¿Imprimirán la misma línea las implementaciones con arreglo y con máscara de bits?
5. ¿De qué hace responsable al llamador la API create/destroy que una clase no?

### Una predicción trabajada

> "`mvectab` se menciona en unas cien líneas, porque cada cuerpo de caso lo lee o lo escribe.
> Hacer `Board` opaco no cambia ninguna línea de `main.cpp`, porque el llamador ya usa solo
> funciones. El compilador rechazará `sizeof(Board)` y `Board board;`, que la versión con clase
> permitía. Ambas implementaciones imprimirán `visited=2 first=1 second=1`, porque el escenario
> marca dos celdas. La API create/destroy hará responsable al llamador de llamar a `board_destroy`
> exactamente una vez en cada camino, incluidos los que vuelven antes."

Esa predicción es útil incluso donde se equivoca. Si el llamador tuviera que cambiar, la cabecera
seguía filtrando algo. Si las salidas difirieran, las dos implementaciones no implementaban el
mismo tipo.

## Paso 2 — Inspeccionar (20 min)

### Contar el acoplamiento del archivo

Desde la raíz del archivo:

```bash
grep -c "mvectab" original/variant-03-pointers-dev-2008/source/*.cpp
grep -c "tablero" original/variant-04-pointers-final-2008/source/*.cpp
grep -n "struct" original/variant-02-records-2008/source/*.cpp
```

Una ejecución de referencia da:

```text
102
146
31:struct movimiento {
```

Los números exactos no dependen de mi máquina; son hechos sobre el archivo fijado. Lo que importa es
el orden de magnitud: un campo, alcanzado desde unas cien líneas, en cada variante con punteros.

| Variante | Declaración | Campo | Líneas que mencionan el almacenamiento |
|---|---|---|---|
| `variant-01` | `int vectab[64]` (línea 17) | ninguno; es un arreglo | 80 líneas mencionan `vectab` |
| `variant-02` | `int vectab[64]` (línea 17) | ninguno; es un arreglo | 102 líneas mencionan `vectab` |
| `variant-03` | `struct stablero vectab[64]` (línea 42) | `int mvectab` (línea 36) | 102 líneas mencionan `mvectab` |
| `variant-04` | `struct tab tablero[64]` (línea 48) | `int vectab` (línea 45) | 146 líneas mencionan `tablero` |

`variant-02` merece una segunda mirada. Es la variante de "registros", y su única estructura es
`struct movimiento { char nombre[250]; }`: un nombre, no un tablero. La variante que debía
demostrar registros nunca le dio uno al tablero.

### La escritura que prueba que el envoltorio es transparente

```text
variant-03, línea 86:   pvectab->mvectab=0;
variant-04, línea 92:   ptablero->vectab=0;
```

Ambas líneas son la traducción mecánica de `vectab[i]=0;`. Se introdujo el puntero, se renombró el
campo y el llamador sigue escribiendo el almacenamiento. Renombra `mvectab` a `visited` y 102 líneas
dejan de compilar. Cambia solo el tipo y la mayoría sigue compilando, porque C++ convierte enteros a
`bool`; eso es peor, porque el acoplamiento sobrevive al cambio de tipo. Esa es la prueba que el
envoltorio falla.

### Leer el módulo incluido

La lección trae la versión opaca ya hecha. Lee los cuatro archivos en este orden, y lee `main.cpp`
primero:

1. [`code/main.cpp`](code/main.cpp) — 15 líneas. Toda la vista del llamador.
2. [`code/board.h`](code/board.h) — 12 líneas. El contrato.
3. [`code/board_array.cpp`](code/board_array.cpp) — 65 líneas. Una representación.
4. [`code/board_bitmask.cpp`](code/board_bitmask.cpp) — 59 líneas. La otra representación.

Después ejecútalo:

```bash
./run.sh
```

Salida esperada:

```text
array=visited=2 first=1 second=1
bitmask=visited=2 first=1 second=1
match=yes
```

La línea `match=yes` la produce `run.sh` solo cuando las dos cadenas de salida son iguales.

### La superficie pública

| Declaración | Línea de la cabecera | Qué promete la firma |
|---|---|---|
| `struct Board;` | 5 | Existe un tipo; su disposición no. |
| `Board *board_create()` | 7 | Existe un tablero nuevo. |
| `void board_destroy(Board *)` | 8 | El tablero deja de existir. |
| `void board_reset(Board *)` | 9 | Toda celda pasa a no visitada. |
| `void board_mark(Board *, int, int)` | 10 | La celda pasa a visitada. |
| `bool board_is_visited(const Board *, int, int)` | 11 | Responde si la celda está visitada. |
| `std::size_t board_visited_count(const Board *)` | 12 | Cuenta las celdas visitadas. |

Siete líneas, seis operaciones, un tipo incompleto. Compáralo con el archivo: una estructura con un
campo, ninguna operación y cien sitios de llamada que conocen el nombre del campo.

> **La brecha que la cabecera no cierra.** `board_mark(nullptr, 1, 1)` compila.
> `board_mark(board, -5, 900)` también. La cabecera no dice nada sobre nulo, coordenadas ni entrada
> fuera de rango. El cuerpo ignora la entrada fuera de rango y desreferencia nulo. El
> comportamiento es real, no está documentado y, por tanto, no forma parte de un contrato que un
> revisor pueda comprobar. Regístralo; `archaeology-13` es donde se escribe.

### Una fuga que la cabecera no controla

`board_array.cpp` define dos ayudantes:

```cpp
bool board_is_inside(int row, int column);
int board_cell_index(int row, int column);
```

No están en la cabecera, pero no son `static` y no están en un espacio de nombres anónimo. El
enlazador los exporta:

```bash
nm -C build/array-representation | grep " T board_"
```

```text
00000000004005c0 T board_mark(Board*, int, int)
0000000000400570 T board_reset(Board*)
0000000000400520 T board_create()
0000000000400550 T board_destroy(Board*)
0000000000400590 T board_is_inside(int, int)
00000000004005b0 T board_cell_index(int, int)
00000000004005e0 T board_is_visited(Board const*, int, int)
0000000000400610 T board_visited_count(Board const*)
```

Dos detalles de implementación forman parte de la tabla de símbolos del binario. La cabecera los
ocultó del compilador; el enlazador todavía los anuncia. Por eso el paso 3 los pone en un espacio
de nombres anónimo. Las direcciones cambiarán en tu máquina; la `T` y los nombres son la evidencia.

## Paso 3 — Ocultar (20 min)

Esta es la prueba de cambio. Vas a reconstruir el tipo para que el `main.cpp` incluido compile y se
ejecute **sin ser editado**.

### Preparar la copia temporal

```bash
mkdir -p /tmp/board-adt
cp archaeology/lessons/archaeology-09-abstract-data-type/code/main.cpp /tmp/board-adt/
cd /tmp/board-adt
```

`main.cpp` es ahora tu único punto fijo. No lo toques. Todo lo demás lo escribes tú.

### Escribir la cabecera

Tres reglas deciden qué entra:

1. **Declara el tipo, no su disposición.** Una línea: `struct Board;`. Sin miembros, sin tamaño,
   sin un `#include` que los revele.
2. **Declara operaciones, no ayudantes.** Seis funciones. `board_is_inside` y `board_cell_index`
   pertenecen al `.cpp`, en un espacio de nombres anónimo.
3. **No declares la estrategia de reserva.** La cabecera dice `Board *`, no `new`, no `malloc`, no
   un pool. Eso es asunto de la implementación.

La cabecera de referencia tiene 12 líneas. Si la tuya es más larga, cada línea extra necesita un
llamador que la use.

### Escribir la implementación

Mueve las definiciones, conserva el comportamiento. El cuerpo que escribas debe cumplir las seis
promesas de arriba, incluidas las dos que la cabecera no enuncia:

- las filas y las columnas son **base 1**, `1..8`;
- la entrada fuera de rango la **ignora** `board_mark` y es **falsa** para `board_is_visited`.

Pon los ayudantes en un espacio de nombres anónimo:

```cpp
namespace
{
    bool board_is_inside(int row, int column);
    int board_cell_index(int row, int column);
}
```

Compila y ejecuta:

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 board_array.cpp main.cpp -o board-array
./board-array
```

| Comprobación | Esperado |
|---|---|
| Compilación | Sin diagnósticos; `-Werror` convierte cualquier aviso en fallo. |
| Salida | `visited=2 first=1 second=1` |
| Ediciones en `main.cpp` | Cero. |
| `sizeof(Board)` en el llamador | No compila. |
| Ayudantes en `nm` | Ausentes. |

### Romper el tipo a propósito

No es vandalismo; es la única forma de ver qué compra la opacidad. Añade cada línea a una copia
temporal de `main.cpp`, de una en una.

**Tamaño:**

```cpp
std::printf("%zu\n", sizeof(Board));
```

```text
main.cpp:3:35: error: invalid application of 'sizeof' to incomplete type 'Board'
    3 | int main() { std::printf("%zu\n", sizeof(Board)); }
      |                                   ^~~~~~~~~~~~~
```

**Reserva en pila:**

```cpp
Board board;
```

```text
main.cpp:2:20: error: aggregate 'Board board' has incomplete type and cannot be defined
    2 | int main() { Board board; (void)board; }
      |                    ^~~~~
```

**Acceso a miembro:**

```cpp
Board *b = board_create();
b->visited = 0;
```

```text
main.cpp:2:42: error: invalid use of incomplete type 'struct Board'
    2 | int main() { Board *b = board_create(); b->visited = 0; board_destroy(b); }
      |                                          ^~
In file included from main.cpp:1:
board.h:5:8: note: forward declaration of 'struct Board'
    5 | struct Board;
      |        ^~~~~
```

**Delete:**

```cpp
delete b;
```

```text
main.cpp:2:41: warning: possible problem detected in invocation of 'operator delete'
              [-Wdelete-incomplete]
main.cpp:2:21: warning: 'b' has incomplete type
board.h:5:8: note: forward declaration of 'struct Board'
main.cpp:2:41: note: neither the destructor nor the class-specific 'operator delete'
              will be called, even if they are declared when the class is defined
```

El último mensaje está partido por anchura; el compilador lo imprime en una línea. Los tres primeros
diagnósticos son errores. El cuarto es un aviso, y `-Werror` lo convierte en fallo. Sin `-Werror`
compila y se salta el destructor: comportamiento indefinido con un mensaje amable. Por eso las
banderas de compilación de `run.sh` son parte de la lección, no decoración.

### Qué impone ahora el compilador

| Pregunta | La impone | Modo de fallo |
|---|---|---|
| ¿Puede un llamador medir o reservar el tipo? | El compilador | `incomplete type` |
| ¿Puede un llamador nombrar un miembro? | El compilador | `invalid use of incomplete type` |
| ¿Existe cada operación? | El enlazador | `undefined reference to ...` |
| ¿Hace la operación lo que dice su nombre? | Nadie | Comportamiento equivocado y silencioso |
| ¿Sigue vivo el manejador? | Nadie | Uso después de liberar |

Las dos últimas filas son las que el TAD no arregla. Mantenlas visibles.

## Paso 4 — Intercambiar (20 min)

### Compilar el mismo llamador dos veces

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 main.cpp board_array.cpp -o board-array
c++ -std=c++20 -Wall -Wextra -Werror -O2 main.cpp board_bitmask.cpp -o board-bitmask
./board-array
./board-bitmask
```

Ambos imprimen:

```text
visited=2 first=1 second=1
```

### Probar que nada más cambió

```bash
sha256sum main.cpp board.h > before.txt
# compila y ejecuta ambas
sha256sum main.cpp board.h > after.txt
diff before.txt after.txt && echo "caller and header unchanged"
```

Lo único que cambió entre las dos compilaciones es el `.cpp` de la línea de enlace. Ese es todo el
criterio de aceptación: tipo opaco, intercambio de representación, misma salida del llamador.

### El intercambio es excluyente

Enlaza las dos implementaciones a la vez y el enlazador se niega:

```bash
c++ -std=c++20 -O2 main.cpp board_array.cpp board_bitmask.cpp -o both
```

```text
/usr/bin/ld: board_bitmask.cpp:(.text+0x0): multiple definition of `board_create()';
board_array.cpp:(.text+0x0): first defined here
```

El error se repite para cada operación. Dos definiciones de un tipo no pueden coexistir en un
programa. No es una limitación que haya que rodear; es la definición de una elección de
representación.

### Medir la representación desde dentro

El llamador no puede imprimir `sizeof(Board)`. La implementación sí. Una sonda que incluye la
implementación está permitida, porque está dentro del módulo:

```cpp
// probe.cpp — una sonda, no un llamador
#include "board_array.cpp"
#include <cstdio>
int main() { std::printf("array_sizeof=%zu\n", sizeof(Board)); }
```

```text
array_sizeof=64
```

La misma sonda sobre `board_bitmask.cpp` imprime `bitmask_sizeof=8` en la cadena de herramientas de
referencia. Un tablero ocupa 64 bytes; el otro, 8. `sizeof(bool)` está definido por la
implementación, así que mídela en lugar de suponer que vale 1.

### Qué significa "sin recompilar"

Sé preciso, porque la frase es fácil de exagerar:

| Forma de compilación | Qué cambia cuando cambia la representación |
|---|---|
| Enlace estático, un binario | El `.cpp` se recompila; el objeto del llamador se reutiliza; el programa se reenlaza. |
| Biblioteca compartida | Se reemplaza el `.so`; el llamador ni se recompila ni se reenlaza. |
| Solo cabecera | No aplica: no hay una representación separada que intercambiar. |

La **fuente** del llamador nunca cambia. El **archivo objeto** del llamador se reconstruye solo si
la cabecera cambió, y no cambió. Esa es la diferencia con `archaeology-08`, donde `bool visited[64]`
en la cabecera obligaba a reconstruir.

### Qué no prueba el intercambio

| Afirmación | ¿Probada? | Por qué |
|---|---|---|
| Ambas implementaciones coinciden para toda secuencia de marcas. | No | Un escenario, dos marcas. |
| Las dos implementaciones tienen la misma complejidad. | No | No se midió. |
| El comportamiento del tablero coincide con el archivo. | No | La base no es el archivo. |
| `board_mark` rechaza coordenadas fuera de rango. | No | Las ignora. |
| El manejador no puede fugarse. | No | `destroy` es manual. |
| La ABI es estable si cambia la firma de una operación. | No | La cabecera cambia y los llamadores recompilan. |

## Paso 5 — Explicar (10 min)

### Qué sabe ahora el llamador

Rellena esto con tu propio módulo, no con el incluido.

| Pregunta | ¿Lo sabe el llamador? | Cómo lo aprende |
|---|---|---|
| Qué operaciones existen | Sí | La cabecera. |
| Tipos de parámetros y de retorno | Sí | La cabecera. |
| Qué operaciones mutan | Sí | El `const` en las consultas. |
| Cuánto mide un tablero | No | La implementación. |
| Cómo se almacena un tablero | No | La implementación. |
| Si `board_create` puede fallar | No | La implementación: `new` lanza. |
| Quién es dueño del manejador | En parte | El nombre `destroy` lo sugiere; nada lo afirma. |
| Que las filas y columnas son base 1 | No | La implementación o las pruebas. |
| Que la entrada fuera de rango se ignora | No | La implementación. |
| Que `board_reset` es redundante tras crear | No | La implementación: `new Board{}` pone ceros. |

Dos filas merecen una segunda mirada. "Quién es dueño del manejador" es el precio de la API C: el
nombre sugiere propiedad, el compilador no la impone. "`board_reset` es redundante" es la misma
ambigüedad de contrato que en `archaeology-08`: la fábrica ya produce un tablero vacío, así que un
llamador no puede saber si `reset` es obligatorio antes de usar.

### La prueba de cambio

Una lección de diseño se juzga por un cambio, no por una forma. Dos listas, ambas tomadas del módulo
que construiste.

**Cambios que quedan dentro del módulo:**

| Cambio | Fuente del llamador | Recompilación | Salida |
|---|---|---|---|
| `bool visited[64]` → una máscara de 64 bits | Sin cambios | No | Idéntica. |
| `bool[64]` → `std::array<bool, 64>` | Sin cambios | No | Idéntica. |
| Fórmula `(row-1)*8+(col-1)` → `(row-1)*8+col-1` | Sin cambios | No | Idéntica. |
| Bucle de `visited_count` → `std::popcount` | Sin cambios | No | Idéntica. |
| Ayudantes movidos a un espacio de nombres anónimo | Sin cambios | No | Idéntica. |
| Añadir una caché del conteo dentro de la implementación | Sin cambios | No | Idéntica. |

**Cambios que alcanzan al llamador:**

| Cambio | Por qué alcanza al llamador |
|---|---|
| Renombrar `board_mark` a `board_set_visited` | El nombre está en la cabecera; cada llamada se edita. |
| Cambiar `board_mark` para recibir una estructura `Cell` | La firma cambia; cada llamada se edita. |
| Pasar a coordenadas base 0 | Compila, se ejecuta y significa otra cosa en silencio. |
| Hacer que `board_mark` lance fuera de rango | Compila, se ejecuta y cambia el comportamiento en el borde. |
| Quitar `board_reset` | Cualquier llamador que la use deja de compilar. |
| Cambiar el tipo de retorno de `board_visited_count` | La declaración cambia; los llamadores recompilan. |

Compara las dos tablas. La primera tabla es "editar un archivo, reconstruir un archivo". En la
lección 08 la misma tabla era "no editar nada, reconstruirlo todo". Esa diferencia es el
producto.

### El precio, en un párrafo

Escribe esto con tus palabras en el registro. La referencia:

> El manejador opaco da al llamador independencia de representación y un conjunto estable de
> operaciones. Le quita al compilador la capacidad de comprobar el ciclo de vida. Cada
> `board_create` debe emparejarse con exactamente un `board_destroy` en cada camino, incluidos los
> que vuelven antes o lanzan. El compilador no ve el emparejamiento, el enlazador no lo ve y las
> pruebas solo ven los caminos que ejecutan. Un pimpl conserva la independencia y devuelve la
> comprobación del ciclo de vida al compilador, a cambio de una indirección por operación y cinco
> miembros especiales fuera de línea.

### Dos arreglos, dos tamaños

**Arreglo mínimo.** El manejador opaco que acabas de escribir. Es mínimo en el sentido que importa
aquí: `main.cpp` queda intacto, el comportamiento es idéntico y la representación puede
reemplazarse sin tocar al llamador. Eso basta para el criterio de aceptación.

**Arreglo estructural.** Una clase con pimpl, o una fábrica que devuelve un tipo de valor con
destructor fuera de línea. Entonces el llamador obtiene RAII, copia y movimiento, y la
representación sigue oculta. Enlaces con la pista A: `construction-09` (tipo abstracto de datos)
para el intercambio, `construction-10` (invariantes) para la validez, `construction-11`
(encapsulación) para la clase, `construction-16` (propiedad) para el ciclo de vida. Esta lección se
detiene a propósito en el manejador; `construction-16` y `archaeology-10` dan los pasos siguientes.

### Qué no arregla el TAD

- No hace correcto el tablero. El error de límite pertenece a `archaeology-04`.
- No le da dueño al tablero en el llamador. El mapa de propiedad pertenece a `archaeology-05`.
- No enuncia el contrato. Las coordenadas y la política de fuera de rango siguen sin documentar;
  eso es `archaeology-13`.
- No valida. `board_mark(board, -5, 900)` sigue compilando y no hace nada.
- No elige entre dos representaciones. Solo abarata la elección.
- No reemplaza al polimorfismo. Un `switch` sobre el tipo de movimiento es `archaeology-10`.

## Paso 6 — Registrar (5 min)

Rellena [`EVIDENCE-es.md`](EVIDENCE-es.md):

- configuración y fijación del archivo;
- la tabla de predicción, escrita antes del paso 2;
- los conteos del archivo y las dos declaraciones de envoltorio;
- la cabecera opaca que escribiste, con conteos de líneas;
- los errores exactos del compilador para tamaño, pila y acceso a miembro;
- el error exacto de enlace para dos implementaciones;
- las dos salidas y la comparación de hashes;
- los tamaños medidos;
- la tabla de conocimiento del llamador;
- las dos listas de cambios;
- la tabla de costes de ciclo de vida;
- la comparación con el pimpl;
- la frontera de evidencia: verificado, inferido, desconocido;
- la comprobación de integridad del archivo.

Una frase como "el tipo es opaco" es una opinión. Un error del compilador sobre `sizeof(Board)` es
evidencia.

## El original en detalle

Hechos que citarás repetidamente, con números de línea del archivo fijado:

| Ubicación | Qué hay |
|---|---|
| `variant-01`, línea 17 | `int vectab[64],vecjugadas[8];` — ningún tipo. |
| `variant-02`, línea 17 | El mismo arreglo; la única estructura guarda un nombre. |
| `variant-03`, línea 34 | `struct stablero {` — abre el envoltorio. |
| `variant-03`, línea 36 | `int mvectab;` — un entero, público. |
| `variant-03`, línea 42 | `struct stablero vectab[64];` — el arreglo de envoltorios. |
| `variant-03`, línea 44 | `struct stablero *pvectab,*pvectab1,*pvectab2;` — tres cursores. |
| `variant-03`, línea 86 | `pvectab->mvectab=0;` — la traducción de `vectab[i]=0;`. |
| `variant-04`, línea 44 | `struct tab {` — la misma idea, otro nombre. |
| `variant-04`, línea 45 | `int vectab;` — un entero, público. |
| `variant-04`, línea 48 | `struct tab tablero[64];` — el arreglo de envoltorios. |
| `variant-04`, línea 49 | `struct tab *ptablero,*ptablero1,*ptablero2;` — tres cursores. |
| `variant-04`, línea 92 | `ptablero->vectab=0;` — la misma traducción. |

Conteos, de `grep -c` sobre los archivos fijados:

| Archivo | Líneas | Líneas que mencionan el almacenamiento del tablero |
|---|---:|---:|
| `variant-01` base | 930 | 80 mencionan `vectab` |
| `variant-02` registros | 1021 | 102 mencionan `vectab` |
| `variant-03` punteros dev | 1058 | 102 mencionan `mvectab` |
| `variant-04` punteros final | 1079 | 146 mencionan `tablero` |

El archivo contiene **cero** archivos `.h` o `.hpp` en todas las variantes. Eso es un hecho sobre el
artefacto, no una inferencia.

> **Una corrección de nombres.** La guía de la pista dice que `struct stablero` aparece en
> "variant-03 y variant-04". El archivo muestra `stablero` en variant-03 y `struct tab` en
> variant-04. La clase de fallo es idéntica; solo cambia el nombre. Cita el archivo que leíste.

## Qué prueba y qué no prueba el tipo opaco

| Afirmación | ¿Respaldada? | Por qué |
|---|---|---|
| El llamador no puede medir ni reservar un `Board`. | Sí | El compilador lo rechaza; capturaste el mensaje. |
| El llamador no puede nombrar un miembro. | Sí | `invalid use of incomplete type`. |
| El mismo llamador compila contra dos representaciones. | Sí | Dos compilaciones, un `main.cpp`. |
| La salida es idéntica para el escenario. | Sí | Dos ejecuciones, la misma línea. |
| La fuente del llamador sobrevive a un cambio de representación. | Sí | No hizo falta editar. |
| El objeto del llamador sobrevive a un cambio de representación. | Sí, si la cabecera no cambió | La cabecera es la dependencia. |
| Ambas implementaciones coinciden para toda entrada. | No | Un escenario. |
| El manejador está libre de fugas. | No | `destroy` es manual. |
| El tablero rechaza coordenadas inválidas. | No | Las ignora. |
| El tipo documenta su contrato. | No | La cabecera no tiene prosa. |
| El programa de 2008 compilaría hoy sin cambios. | No | `iostream.h`, `conio.h`, `void main`; ver `archaeology-16`. |

Esta tabla es la comprobación de honestidad. La afirmación más fuerte de esta lección es un mensaje
del compilador; la más débil es "el diseño es mejor". Mantenlas separadas.

## Autocomprobación (binaria)

- [ ] Escribí cinco predicciones falsables antes de leer `board.h`.
- [ ] Conté las menciones del almacenamiento del tablero en `variant-03` y `variant-04`.
- [ ] Leí `main.cpp` antes que `board.h`.
- [ ] Ejecuté el módulo incluido y obtuve `match=yes`.
- [ ] Reconstruí `board.h` y una implementación en temporal sin editar `main.cpp`.
- [ ] Mi cabecera declara `struct Board;` y seis operaciones, y nada más.
- [ ] Mis ayudantes están en un espacio de nombres anónimo y ausentes de `nm`.
- [ ] Capturé el error del compilador para `sizeof(Board)`.
- [ ] Capturé el error del compilador para un `Board` en pila.
- [ ] Capturé el error del compilador para `board->visited`.
- [ ] Capturé el aviso `-Wdelete-incomplete`.
- [ ] Compilé el mismo llamador contra ambas implementaciones y comparé la salida.
- [ ] Capturé el error de enlace al enlazar ambas implementaciones.
- [ ] Medí 64 bytes y 8 bytes desde dentro del módulo.
- [ ] Rellené la tabla de conocimiento del llamador, incluidas las filas que la cabecera no
  responde.
- [ ] Listé tres cambios que quedan dentro del módulo y tres que alcanzan al llamador.
- [ ] Listé los fallos de ciclo de vida que la API create/destroy deja al llamador.
- [ ] Comparé el manejador con un pimpl y nombré el coste de cada uno.
- [ ] Enuncié qué prueba el intercambio y qué no.
- [ ] El archivo está sin modificar (`git status` en el archivo está limpio).

Si una casilla no está marcada, la casilla sin marcar nombra el paso siguiente. Repetir uno no tiene
penalización.

## Misconcepciones que esta lección refuta

| Idea equivocada | Por qué es falsa | Qué hacer en su lugar |
|---|---|---|
| "Una estructura es un tipo abstracto de datos." | Una estructura con campos públicos no tiene operaciones ni representación oculta. | Declara operaciones; oculta los campos. |
| "Envolver un entero en una estructura lo convierte en tipo." | El llamador sigue escribiendo `pvectab->mvectab`. | Cambia los sitios de llamada, no la ortografía. |
| "Una clase es un TAD." | Una clase con miembros públicos filtra la representación igual. | Oculta el estado y expón operaciones. |
| "La cabecera oculta el tipo." | Oculta la disposición; la definición sigue en un `.cpp`. | Di "tipo incompleto", no "secreto". |
| "Los tipos opacos siempre son mejores." | Cuestan un ciclo de vida manual y una reserva. | Elige por lo que el llamador debe hacer. |
| "`delete` sobre un tipo incompleto es un error." | Es un aviso; sin `-Werror` compila y se salta el destructor. | Compila con `-Werror` y destruye por la API. |
| "El llamador no recompila, así que nada se reconstruye." | El objeto de la implementación se reconstruye y el programa se reenlaza. | Di "sin editar el llamador, sin recompilar el llamador". |
| "El intercambio prueba que las implementaciones son equivalentes." | Prueba un escenario, una línea. | Añade una prueba diferencial sobre muchas secuencias. |
| "El TAD valida la entrada." | Ignora las coordenadas fuera de rango. | Añade un invariante; eso es `construction-10`. |
| "El TAD elimina la necesidad de pruebas." | Elimina una clase de acoplamiento, no el comportamiento equivocado. | Conserva la traza de referencia. |
| "Esto es la misma lección que la frontera de módulo." | La frontera oculta nombres; el TAD oculta la representación. | Compara las dos tablas de cambio. |
| "Un pimpl es gratis." | Cuesta una indirección por operación y cinco miembros especiales fuera de línea. | Pone precio antes de elegirlo. |

## Términos usados aquí

| Término | Significado en una línea |
|---|---|
| Tipo abstracto de datos | Un conjunto de valores y operaciones cuya representación no está en el contrato. |
| Representación | Cómo se almacena el estado, frente a lo que significa. |
| Independencia de representación | La capacidad de cambiar el almacenamiento sin editar llamadores. |
| Tipo opaco | Un tipo cuya definición el llamador nunca ve. |
| Declaración adelantada | Una declaración que nombra un tipo sin definirlo. |
| Tipo incompleto | Un tipo que el compilador conoce por nombre, no por tamaño ni disposición. |
| Envoltorio transparente | Una estructura cuyo único contenido es un campo que el llamador aún nombra. |
| Manejador | Un valor que identifica un objeto que el llamador no puede inspeccionar. |
| Prueba de intercambio | Reemplazar la representación y volver a ejecutar el mismo llamador. |
| RAII | Atar el ciclo de vida de un recurso al de un objeto. |
| pimpl | Una clase que guarda un puntero a una implementación incompleta. |
| Regla de cinco | Destructor, constructor de copia, asignación de copia, constructor de movimiento, asignación de movimiento. |
| Semántica de valor | Copiar un valor copia su estado, no una referencia a él. |
| Prueba diferencial | Ejecutar las mismas entradas en dos implementaciones y comparar. |

## Qué significa «terminado» aquí

La **Autocomprobación (binaria)** de arriba es la lista autoritativa. Si todas las casillas están
marcadas, la lección está terminada para ti. `meta.yml` refleja los mismos criterios para las
herramientas.
