# failure-006 — La lectura sin límite del nombre desborda `char nombre[250]`

> [English](failure-006-unbounded-name.md) | [Español](failure-006-unbounded-name-es.md)

- **Clase:** corrección
- **Severidad:** crítica
- **Concepto violado:** la longitud de la entrada es parte del contrato del búfer
- **Archivo:** etiqueta `v1.0.0`,
  `original/variant-02-records-2008/source/guerra-de-caballos-records.cpp`

## Evidencia

- **Variante y línea:** `variant-02:46`, `variant-03:56`, `variant-04:62`.
- **Código:**

```cpp
struct movimiento {
    char nombre[250];       // variant-02, línea 32
};
struct movimiento nom;
// ...
cin>>nom.nombre;            // variant-02, línea 46
```

`operator>>` para `char *` lee hasta un espacio en blanco y no tiene límite de ancho. El tamaño del
arreglo no forma parte de la extracción.

## Síntoma

Un nombre de más de 249 caracteres escribe después del final de `nombre`. El programa no comprueba
la longitud, no avisa y no trunca. Los bytes que siguen al arreglo se sobrescriben con el resto de
la entrada y un NUL de terminación.

## Causa raíz

El programa usa un arreglo C de tamaño fijo y la extracción formateada por defecto. `cin >> arreglo`
no puede conocer el tamaño del arreglo; quien programa debe indicar el ancho. El código confía en
que la persona usuaria respete un límite que el código nunca declara. El mismo registro guarda el
nombre para mostrarlo, así que la entrada no se valida en ningún otro lugar.

## Consecuencia

Desbordamiento de búfer. Se pueden sobrescribir miembros adyacentes, el marco de pila o los
metadatos del montón. El resultado depende del diseño y de las mitigaciones: un bloqueo, estado de
juego corrupto o ejecución de código. Como mínimo, el comportamiento es indefinido para toda entrada
larga. El nombre es lo primero que pide el programa, así que el fallo es alcanzable antes de
cualquier lógica del juego.

## Prueba que lo detecta

El tercer bloque de `tests/run_failure_tests.sh`. Busca `cin>>` en `variant-02` e imprime
`failure-006=detected`. La comprobación confirma que la extracción sin límite existe; no introduce
un nombre largo ni observa el desbordamiento.

Una prueba más fuerte llamaría a la función de entrada con una cadena de 300 caracteres y
comprobaría que el nombre se trunca y que el canario de alrededor sigue intacto. Esa prueba no está
en este arnés porque la función original está atada a `cin` y a la consola.

## Arreglo mínimo

Limita la extracción al tamaño del búfer, reservando un byte para el terminador:

```cpp
#include <iomanip>
cin >> std::setw(sizeof(nom.nombre)) >> nom.nombre;
```

`setw(250)` lee como máximo 249 caracteres y añade el NUL. La alternativa es
`cin.getline(nom.nombre, sizeof nom.nombre);`, que también se detiene en el tamaño del búfer.

El arreglo mínimo evita el desbordamiento. No valida el contenido, no rechaza nombres vacíos ni
maneja nombres con espacios.

## Arreglo estructural

Usa `std::string` y `std::getline`, y luego valida la longitud y el contenido de forma explícita:

```cpp
std::string name;
std::getline(std::cin, name);
if (name.empty() || name.size() > 249)
{
    // rechazar o truncar con una política declarada
}
```

Peldaño de la pista A: [`construction-17-stl`][c17] (cadenas y contenedores estándar) y
[`construction-22-error-contracts`][c22] (validación de entrada y contratos de error).

[c17]: ../../construction/lessons/construction-17-stl/lesson.md
[c22]: ../../construction/lessons/construction-22-error-contracts/lesson.md

## Confianza

Alta en que la extracción no tiene límite y en que el búfer tiene 250 bytes. Media en la
explotabilidad práctica, que depende del compilador, del diseño de la pila y de las mitigaciones de
la plataforma. El comportamiento indefinido no está en duda; el camino de ataque sí.

## Nota sobre la prueba

La prueba es una reproducción aislada del patrón del fallo. No es una prueba del programa original.
La búsqueda demuestra el patrón; no demuestra un desbordamiento en ejecución.
