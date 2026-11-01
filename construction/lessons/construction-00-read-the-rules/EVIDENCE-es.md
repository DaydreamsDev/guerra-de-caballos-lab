# construction-00 — Evidencia (evidence)

> [English](EVIDENCE.md) | [Español](EVIDENCE-es.md)

> Registro propio de quien aprende. Escribe en español.

Esta es una plantilla. Sustituye las preguntas por lo que ocurrió en realidad. Si no
escribiste una predicción antes de ejecutar, dilo; el registro es más útil cuando es honesto.

## 1. Preparación de la ejecución

- Fecha:
- Semilla: `1`
- Nombre del jugador: `Learner`
- Raíz del repositorio:
- Versión de Python (`python3 --version`):
- Reglas leídas el (fecha):
- Formato de transcripción leído el (fecha):

## 2. Comprobación de lectura de las reglas (antes de la ejecución)

Responde con tus propias palabras. Marca todo lo que tuviste que suponer.

- Un movimiento es legal cuando:
- La única excepción a la regla de celdas visitadas es:
- El caballo que empieza es:
- Una casilla inicial pasa a estar visitada cuando:
- Un caballo sin ningún movimiento legal:
- El orden del turno se construye:

## 3. Predicción (escrita antes de ejecutar)

### 3.1 Lo que las reglas obligan

| Campo | Sección de la regla | Valor o forma previstos |
|---|---|---|
| Caballo que empieza | §4 | |
| Forma del orden del turno 1 | §6 | |
| Primer movimiento intentado | §6 | |
| Condición de legalidad | §7, §8 | |
| Resultado si ningún intento es legal | §10 | |

### 3.2 Lo que decide la semilla — mi suposición comprometida

- Quién empieza:
- `order` del turno 1:
- Primera línea `try=`:
- Resultado esperado de ese intento:
- Primera línea `move=`:
- Confianza (0–100 %):
- Por qué elegí esta suposición:

## 4. Ejecución

- Comando:
- Estado de salida:
- Transcripción guardada en:
- Salida completa:

```text

```

## 5. Comparación

- Primera línea `move=` observada:
- Campos que coincidieron:
- Campos que difirieron:
- Diff exacto:

```diff
- previsto:
+ observado:
```

- Campos del encabezado que difirieron:
- Desacuerdo entre el ejemplo del formato y esta ejecución:
- Qué fuente gana, y por qué:

## 6. Explicar la brecha

- El campo en el que me equivoqué:
- ¿Fue una regla que leí mal o un valor que eligió la semilla?
- El supuesto que la especificación nunca hizo:
- Un párrafo, sin palabras de lenguaje de programación:

## 7. Criterio de aceptación — predecir el siguiente movimiento

- Turno que elegí:
- Estado que usé (posiciones y celdas visitadas):
- Mis líneas `try=` previstas:
- Mi línea `move=` prevista:
- Líneas observadas:
- ¿Coincide? Qué me enseñó la diferencia:

## 8. Reflexión y siguiente paso

- Qué puedo explicar ahora sin notas:
- Qué no puedo explicar todavía:
- Una cosa que volvería a leer:
- Siguiente paso:

## 9. Si falta la línea base

Completa esto solo si el comando no pudo ejecutarse.

- Archivo ausente o error:
- Salida del comando:
- Qué hice en lugar de inventar una transcripción:
- Cuándo volveré a intentarlo:

## 10. Autoverificación

- [ ] Mi predicción estaba escrita antes de ejecutar nada.
- [ ] Capturé la transcripción completa, no un resumen.
- [ ] Nombré las líneas exactas que difieren o expliqué por qué no había ninguna.
- [ ] Predije un movimiento posterior a partir del estado y las reglas, y lo comprobé.
- [ ] Puedo explicar la diferencia entre una regla y un valor que depende de la semilla.
- [ ] Registré el siguiente paso.

Sin puntuación. Si una casilla queda sin marcar, escribe la siguiente acción más pequeña en §8.
