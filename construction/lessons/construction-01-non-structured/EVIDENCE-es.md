# construction-01 — Evidencia

> [English](EVIDENCE.md) | [Español](EVIDENCE-es.md)

> Registro propio de quien aprende. Escribe en español.

Esta es una plantilla. Sustituye las preguntas por lo que ocurrió en realidad. Si
contaste antes de predecir, dilo; el registro es más útil cuando es honesto.

## 1. Preparación de la ejecución

- Fecha:
- Semilla: `1`
- Nombre del jugador: `Learner`
- Raíz del repositorio:
- Compilador y versión (`g++ --version` o `clang++ --version`):
- Línea base leída el (fecha):
- Reglas leídas el (fecha):

## 2. Inventario de saltos (antes de la ejecución)

Complétalo desde el archivo, no de memoria. Una fila por etiqueta.

| Etiqueta | Línea | Se alcanza desde (líneas) | ¿Entrada de turno? |
|---|---|---|---|
| | | | |

Recuentos:

- Etiquetas:
- Saltos:
- Etiquetas alcanzadas desde más de un salto:
- Etiquetas alcanzadas tanto por un camino de jugador como por uno de máquina:
- Entradas de turno:

Lugares donde se sortea el orden de movimientos:

- Línea:
- Etiquetas que pueden llegar allí:

## 3. Predicción (escrita antes de medir)

R-SKIP, con mis propias palabras:

- Cuándo se salta el turno:
- Qué registra igualmente el turno:
- Qué número imprime un turno saltado en `turn=`, y si es aceptable:
- Qué no cuenta el turno saltado:
- Por qué esto no necesita ningún sorteo nuevo:

Mis números comprometidos:

| Pregunta | Mi número | Confianza | Razón |
|---|---|---|---|
| Etiquetas que inician un turno | | | |
| Saltos que hay que recablear | | | |
| Etiquetas nuevas | | | |
| Saltos nuevos | | | |
| Lugares que deciden quién juega después | | | |

El punto de inserción único más barato, y por qué no es suficiente:

## 4. Ejecución

- Comando de compilación (o `./run.sh`):
- Comando de ejecución:
- Estado de salida:
- Comando de diff y destino (`../../../common/traces/seed-0001.txt`):
- ¿Diff vacío? (sí / no):

```text
<salida del diff, o «sin diferencias»>
```

Si el diff no estaba vacío, detente aquí y completa §9.

## 5. Comparación — la tabla de coste

| Elemento | Previsto | Contado | Diferencia |
|---|---|---|---|
| Etiquetas que inician un turno | | | |
| Saltos recableados (llegadas a una entrada de turno) | | | |
| Etiquetas nuevas necesarias | | | |
| Saltos nuevos necesarios | | | |
| Lugares que deciden quién juega después | | | |
| Archivos tocados | 1 | 1 | 0 |

Cada salto recableado, listado:

| Línea del salto | Destino | ¿La entrada comprueba? | Qué cambia para esta llegada |
|---|---|---|---|
| | | | |

- Predicción más cercana, y por qué:
- Predicción más lejana, y qué no vi:
- ¿Bastó el punto de inserción único más barato? Si no, qué comportamiento se rompe:
- ¿Mostraría la transcripción esa rotura? Cómo:

## 6. Explicar el coste

Una frase, sin las palabras *malo*, *goto*, *espagueti* o *ilegible*:

>

Un párrafo:

- Por qué R-SKIP no tiene un único hogar:
- Qué debe sostener el lector en la memoria de trabajo:
- Qué hace que las comprobaciones duplicadas se separen:
- Qué tiene que hacer el lector para saber que un cambio está completo:

## 7. Criterio de aceptación — saltar turno

- Requisito reformulado sin añadir ningún sorteo:
- Regla de conteo usada:
- Número de saltos recableados:
- Cada salto contado está listado en §5:
- Coste explicado sin nombrar la palabra clave:

¿Implementé R-SKIP? (sí / no / no era necesario)

Si sí, qué cambió en la transcripción y por qué es un juego nuevo y no un diseño nuevo:

## 8. Reflexión y siguiente paso

- Qué puedo explicar ahora sin notas:
- Qué no puedo explicar todavía:
- Una cosa que volvería a leer:
- El número que llevaré a `construction-02`:
- Siguiente paso:

## 9. Si falta la línea base o no es fiel

Completa esto solo si el archivo no estaba, la compilación falló o el diff no estaba vacío.

- Archivo ausente, error de compilación o diff:
- Salida exacta de los comandos:
- Qué hice en lugar de inventar un recuento de saltos:
- Cuándo volveré a intentarlo:

## 10. Autoverificación

- [ ] Inventarié etiquetas y saltos desde el archivo, con números de línea.
- [ ] Escribí el recuento previsto antes de contar.
- [ ] Compilé la línea base y obtuve un diff vacío contra `seed-0001.txt`.
- [ ] Apliqué una única regla de conteo enunciada a cada salto.
- [ ] Listé cada salto recableado con su destino y su línea.
- [ ] Mi explicación de una frase evita las cuatro palabras prohibidas.
- [ ] Mi párrafo nombra la causa estructural, no el síntoma.
- [ ] Registré el número que voy a comparar en `construction-02`.

Sin puntuación. Si una casilla queda sin marcar, escribe la siguiente acción más pequeña en §8.
