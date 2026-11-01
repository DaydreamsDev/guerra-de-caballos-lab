# archaeology-13 — Evidencia: documentación técnica para mantenedores

> Este es tu registro. Escríbelo para tu yo futuro.
> Registra lo que observaste, no lo que esperabas que la lección pidiera.

## 1. Entorno

- Fecha:
- Ruta del archivo:
- Etiqueta descargada:
- Hash del commit (`git rev-parse HEAD`):
- Compilador y versión (`c++ --version`):
- Shell y versión:
- Kernel y distribución (`uname -sr`):
- Directorio de trabajo:
- Directorio de documentación (`$DOCBENCH/docs`):
- Directorio de compilación (`$DOCBENCH`):
- Herramientas usadas (versiones de `timeout`, `wc`, `grep`, `cp`):
- Algo inesperado en el entorno:

## 2. Alcance y audiencia

- Fuente documentada:
- Versión o etiqueta:
- Compilador y banderas documentadas:
- Plataforma documentada:
- Audiencia:
- Dentro del alcance:
- Fuera del alcance:
- Una frase que un mantenedor debería poder responder después de leer:

>

## 3. Inventario de documentación

| Documento | Ruta | Pregunta que responde | Pregunta que deja abierta |
|---|---|---|---|
| | | | |
| | | | |
| | | | |
| | | | |
| | | | |

- El hueco que estoy llenando:
- El hueco que no estoy llenando:
- El documento que casi copié, y por qué no lo hice:

## 4. Inspección de la fuente

Comandos y anclajes:

```bash
wc -l "$ARCHIVE/original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp"
file "$ARCHIVE/original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp"
grep -c '//' "$ARCHIVE/original/variant-01-baseline-2008/source/guerra-de-caballos-baseline.cpp"
```

- Total de líneas:
- Codificación (salida de `file`, literal):
- Líneas que contienen `//`:

Anclajes que verifiqué:

| Anclaje | Línea(s) | Qué demuestra | ¿Verificado? |
|---|---:|---|---|
| Includes | 10–14 | La fuente asume cabeceras preestándar. | |
| Glifos | 16 | El tablero usa los valores enteros 21, 156 y 219. | |
| Tablero y orden | 17 | El tablero tiene 64 casillas; el orden tiene 8 entradas. | |
| Estado mutable | 18 | El estado es global. | |
| Punto de entrada | 32 | `void main()` no es C++ estándar. | |
| Llamadas a consola | 38, 43 | El programa asume una consola de Windows. | |
| Entrada | 42 | `getch()` es la única entrada. | |
| Bucle de juego | 44 | `jugada` es la condición del bucle y el centinela. | |
| Off-by-one | 61 | La inicialización escribe una posición más allá del tablero. | |
| Semilla | 76 | `srand(time(NULL))` está dentro del bucle de reintento. | |
| Colocación inicial | 78–83 | Las casillas y tiradas son aleatorias y se redibujan en colisión. | |
| Orden de movimientos | 176–205 | Los ocho casos se evalúan en una rotación aleatoria. | |
| Fórmula del índice | 226–229 | Un índice de casilla es `fila * 8 + columna`. | |
| Captura | 254–257 | La captura pone `ganador=0`. | |
| Trampa | 702–717 | Ocho casos fallidos terminan la partida. | |
| Orden de turnos | 726–751 | Empieza quien saca la tirada mayor. | |
| Salida del tablero | 870–928 | El tablero se imprime como 8 por 8 casillas. | |

## 5. Registro del README

| Sección de la plantilla | Qué escribí | Fuente de la afirmación |
|---|---|---|
| Qué es | | |
| Compilar | | |
| Ejecutar | | |
| Probar | | |
| Especificación | | |
| Evidencia | | |

- El comando de compilación que documenté:
- El comando de ejecución que documenté:
- El comando de prueba que documenté:
- La cadena de herramientas y las banderas que registré:
- Los ficheros esperados que registré:
- El control de semilla que registré:
- El límite de cobertura que registré:
- La frase débil que reemplacé, y por qué:

>

## 6. Registro de la especificación

| Cláusula | Texto de la especificación | Líneas fuente | Marca (verificada / inferida / desconocida) |
|---|---|---|---|
| Tablero | | | |
| Piezas | | | |
| Casillas iniciales | | | |
| Orden de inicio | | | |
| Orden de turnos | | | |
| Orden de movimientos | | | |
| Movimiento legal | | | |
| Captura | | | |
| Trampa | | | |
| Empate | | | |
| Aleatoriedad | | | |
| Salida | | | |

- ¿Reglas, aleatoriedad y presentación separadas? (sí / no)
- La cláusula que no pude verificar:
- La cláusula que inferí, y por qué:
- El desconocido que declaré en la especificación:
- La cláusula que borré porque describía la implementación:

## 7. Registro de ejecución de comandos

| # | Comando (corto) | Directorio | Estado | Observado | Esperado | ¿Coincide? |
|---:|---|---|---:|---|---|---|
| 1 | Preparar fuente y cabeceras | | | | | |
| 2 | Compilar objeto | | | | | |
| 3 | Enlazar sonda | | | | | |
| 4 | Ejecutar con timeout | | | | | |
| 5 | Contar stdout y stderr | | | | | |
| 6 | Prueba de humo | | | | | |
| 7 | Compilación ingenua (bloqueo) | | | | | |

- El shell limpio que usé:
- El comando que falló la primera vez:
- La frase del README que añadí o corregí:
- Volumen de salida en tres ejecuciones:
- La primera línea exacta de stderr, literal:
- ¿Algún comando de compilación o ejecución dependió de un fichero fuera de `$DOCBENCH` después de
  la preparación? (Debe ser no.)

## 8. Auditoría de afirmaciones

| Afirmación en la documentación | Fuente | Tipo (línea / comando / documento) | Veredicto |
|---|---|---|---|
| | | | |
| | | | |
| | | | |
| | | | |
| | | | |
| | | | |

- Afirmaciones que borré porque no tenían fuente:
- Afirmaciones que rebajé a desconocidas:
- Afirmaciones que eran falsas aunque un comando pasara:

## 9. Desconocidos

| Desconocido | Por qué importa | Qué lo resolvería |
|---|---|---|
| | | |
| | | |
| | | |
| | | |

- El desconocido que bloquea a un mantenedor hoy:
- El desconocido que no bloquea, pero debe registrarse:
- El desconocido que me tienta a esconder, y por qué no lo haré:

## 10. Comprobación de alcance

- [ ] Cada frase ayuda a un mantenedor a compilar, ejecutar, probar o cambiar el artefacto.
- [ ] Eliminé las frases que describen el contexto del artefacto en lugar de su uso.
- [ ] El README no tiene una sección de historia.
- [ ] La especificación no es un recorrido función por función.
- [ ] La sección de evidencia nombra la fuente, los comandos y los desconocidos.

La frase que eliminé, y la sección donde estaba:

>

## 11. Integridad del archivo

- `git status` en el archivo:
- ¿Edité `original/`? (Debe ser no.)
- ¿Apliqué un parche dentro del archivo? (Debe ser no.)
- ¿Dónde vivió la copia de la fuente?
- ¿Edité las plantillas de la lección? (Debe ser no; las copié.)

## 12. Lista de aceptación

- [ ] Inventarié al menos cuatro documentos existentes y nombré sus huecos.
- [ ] Escribí la frase de alcance antes de redactar el README.
- [ ] El README completa cada sección de `README-TEMPLATE-es.md`.
- [ ] La sección de compilación nombra la cadena de herramientas, las banderas, las dependencias y
      los ficheros esperados.
- [ ] La sección de ejecución nombra la entrada, la salida, el control de semilla y el estado
      esperado.
- [ ] La sección de prueba nombra el comando y qué no cubre.
- [ ] La especificación completa cada sección de `SPEC-TEMPLATE-es.md`.
- [ ] Cada cláusula cita líneas fuente y lleva una marca de confianza.
- [ ] Las reglas, la aleatoriedad y la presentación están separadas.
- [ ] Ejecuté cada comando del README desde un directorio limpio.
- [ ] Registré el estado de salida y la salida observada de cada bloque.
- [ ] Audité cada afirmación contra una línea, un comando o un documento.
- [ ] Declaré los desconocidos con un camino de resolución.
- [ ] La comprobación de alcance pasa.
- [ ] El archivo está sin modificar.

## 13. Siguiente paso

- Continúa con `archaeology-14` — pruebas y determinismo.
- Si un comando necesitó un arreglo, guarda el texto fallido junto al texto que funciona. El par es
  la evidencia de que el README se ejecutó.
- Si la especificación tiene una cláusula inferida, nombra la observación que la promovería a
  verificada.
- Una frase que quiero que mi yo futuro lea antes de escribir otro README:

>
