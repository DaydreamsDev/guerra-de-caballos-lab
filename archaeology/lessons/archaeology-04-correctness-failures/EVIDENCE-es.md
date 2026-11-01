# archaeology-04 — Evidencia: fallos de corrección

> Este es tu registro. Escríbelo para tu yo del futuro.
> Registra lo que observaste, no lo que esperabas que la lección pidiera.

## 1. Preparación

- Fecha:
- Ruta del archivo:
- Etiqueta descargada:
- Hash del commit (`git rev-parse HEAD`):
- Compilador y versión (`c++ --version`):
- Shell y versión:
- Kernel y distribución (`uname -sr`):
- Directorio de trabajo:
- Algo inesperado en la preparación:

## 2. Predicción — escribe antes de leer las tarjetas

No edites esta tabla después del paso 3. La diferencia entre la predicción y la observación es la
señal de aprendizaje; reparar la predicción destruye esa señal.

| # | Fallo | Mi predicción: qué hace el código | Cómo sabría que me equivoco |
|---:|---|---|---|
| 1 | Límite del bucle | | |
| 2 | Punto de entrada | | |
| 3 | Semilla en el bucle de reintento | | |
| 4 | Inicialización con punteros | | |
| 5 | Lectura del nombre | | |
| 6 | Condición de victoria | | |

Una frase falsable sobre el conjunto:

>

## 3. Ejecución de las pruebas

Comando:

```bash
bash tests/run_failure_tests.sh /ruta/al/archivo
```

Salida observada:

```text
failure-001=detected
failure-003=detected
failure-004=detected
failure-005=detected
failure-002=detected
failure-006=detected
```

- Código de salida:
- Compilador usado por el arnés:
- Ruta del archivo usada:
- ¿El orden de la salida coincidió con la referencia? Si no, ¿por qué?

Qué significa `detected` aquí: la prueba reprodujo el patrón. No significa que el programa archivado
se haya compilado, ejecutado o demostrado roto en tiempo de ejecución.

## 4. Registro de tarjetas — diagnóstico

| Tarjeta | Variante:línea | Síntoma | Causa raíz | Severidad | Confianza |
|---|---|---|---|---|---|
| failure-001 | | | | | |
| failure-002 | | | | | |
| failure-003 | | | | | |
| failure-004 | | | | | |
| failure-006 | | | | | |
| failure-005 | | | | | |

## 5. Registro de tarjetas — arreglos y pruebas

| Tarjeta | Prueba que lo detecta | Qué comprueba la prueba | Arreglo mínimo | Arreglo estructural y peldaño |
|---|---|---|---|---|
| failure-001 | | | | |
| failure-002 | | | | |
| failure-003 | | | | |
| failure-004 | | | | |
| failure-006 | | | | |
| failure-005 | | | | |

## 6. Orden por severidad

Ordena los seis por coste en tu contexto. El orden no es universal; la razón es la evidencia.

| Puesto | Tarjeta | Coste en el trabajo real | Por qué este puesto |
|---:|---|---|---|
| 1 | | | |
| 2 | | | |
| 3 | | | |
| 4 | | | |
| 5 | | | |
| 6 | | | |

## 7. Qué demuestran y qué no demuestran las pruebas

| Afirmación | ¿Sostenida? | Por qué |
|---|---|---|
| El patrón existe en la línea citada. | | |
| El programa original se bloquea o corrompe memoria. | | |
| El programa original es explotable. | | |
| El arreglo mínimo propuesto elimina el daño. | | |
| El arreglo estructural propuesto previene la clase. | | |
| `void main` falla en todos los compiladores. | | |
| `cin >>` desborda con toda entrada larga. | | |

## 8. Límites de la evidencia

Escribe cuatro viñetas, una por límite:

- Las pruebas son reproducciones aisladas; no compilan ni ejecutan el código archivado.
- Las dos comprobaciones de shell leen texto; no ejecutan el programa.
- El código es el único testigo; no hay binario ni cadena de herramientas de 2008 para comparar.
- La intención detrás de una decisión se infiere, no se verifica. Di qué afirmaciones son
  inferencias.

Añade cualquier límite específico de tu entorno:

-

## 9. Qué arreglaría primero

Un párrafo. Nombra la tarjeta, el arreglo mínimo, la prueba y la razón por la que el coste es mayor
que el de las demás. No propongas una reescritura en este párrafo.

>

## 10. Integridad del archivo

- `git status` en el archivo:
- ¿Edité `original/`? (Debe ser no.)
- ¿Apliqué algún parche dentro del archivo? (Debe ser no.)
- ¿Dónde guardé mi copia de trabajo, si hice una?

## 11. Siguiente paso

- Continúa con `archaeology-05` — estado global y acoplamiento oculto.
- Vuelve a leer la tarjeta del fallo que ordenaste al final. ¿El puesto se debió al coste o a lo
  fácil que era ver el defecto? Escribe una frase.
- Si una prueba imprimió `not-detected`, registra la causa antes de cambiar la prueba. Una
  comprobación que falla es evidencia sobre la comprobación, antes que nada.
