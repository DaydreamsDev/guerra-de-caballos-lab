# failure-002 — `void main()` no es un punto de entrada estándar

> [English](failure-002-void-main.md) | [Español](failure-002-void-main-es.md)

- **Clase:** corrección
- **Severidad:** alta
- **Concepto violado:** el punto de entrada es un contrato con el sistema de ejecución
- **Archivo:** etiqueta `v1.0.0`,
  `original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp`

## Evidencia

- **Variante y línea:** `variant-01:32`, `variant-02:37`, `variant-04:52`.
- **Contraste:** `variant-03:47` usa `int main()`.
- **Código:**

```cpp
void  main()             // variant-01, línea 32
{
    system("color 1f");
    inicializacion();
    cabubicacion();
    // ...
}
```

La declaración devuelve `void`. El estándar de C++ exige que `main` devuelva `int`. Microsoft Visual
C++ 6.0 aceptaba `void main` como extensión, y por eso el código sobrevivió a una compilación real
en 2008.

## Síntoma

El punto de entrada no tiene tipo de retorno. Un compilador conforme rechaza el programa, y una
compilación con `-Werror` falla antes de que empiece el juego. Cuando un compilador permisivo lo
acepta, el estado de salida del proceso no está definido por el código.

## Causa raíz

El programa se escribió para un compilador que permitía la extensión, y el punto de entrada nunca se
trató como un contrato. El código original no necesitaba un código de estado, así que eligió la
declaración por comodidad. El código registra una convención de compilación local,
no la regla del lenguaje.

## Consecuencia

Fallo de portabilidad. El mismo archivo que compilaba en 2008 falla en una cadena de herramientas
moderna o en integración continua. Un ejecutor de pruebas, un script de shell o un proceso padre no
pueden leer un estado de salida con significado. El defecto es visible al compilar, lo que lo hace
más barato que los errores de memoria, pero bloquea cualquier otro experimento hasta que se corrige.

## Prueba que lo detecta

El segundo bloque de `tests/run_failure_tests.sh`. Busca `void main` en `variant-01` y `int main` en
`variant-03`, e imprime `failure-002=detected` solo cuando ambos patrones coinciden. El par es
deliberado: una variante muestra el fallo y la otra muestra la forma estándar.

La comprobación lee el texto del código. No compila el archivo, así que detecta la declaración, no
el veredicto del compilador.

## Arreglo mínimo

```cpp
int main()
{
    // ...
    return 0;
}
```

Si el programa llega a su final normal, `return 0;` declara el estado de forma explícita. El retorno
implícito de `main` también es válido, pero la forma explícita es más clara en una lección sobre
contratos.

## Arreglo estructural

Mantén `main` como un adaptador delgado: que no lea estado del juego, que llame a una función que
ejecute la partida y que devuelva su estado. Esa función se puede llamar desde una prueba sin
cruzar la frontera de un proceso.

```cpp
int runGame();
int main() { return runGame(); }
```

Peldaño de la pista A:
[`construction-03-functions`](../../construction/lessons/construction-03-functions/lesson.md)
(funciones, parámetros y valores de retorno).

## Confianza

Alta. La declaración es visible en las líneas citadas y la regla del lenguaje no es ambigua. La
única incertidumbre es el comportamiento exacto de la extensión en el compilador de 2008, que no
hace falta para diagnosticar el fallo de portabilidad.

## Nota sobre la prueba

La prueba es una reproducción aislada del patrón del fallo. No es una prueba del programa original.
La búsqueda demuestra que la declaración existe; no demuestra cómo reaccionará un compilador
concreto.
