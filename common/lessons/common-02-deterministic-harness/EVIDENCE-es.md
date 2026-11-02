# common-02 — Evidencia: arnés determinista
> [English](EVIDENCE.md) | [Español](EVIDENCE-es.md)

> Este es tu registro. Escríbelo para tu yo futuro.
> Registra lo que observaste, no lo que la lección parecía querer.

## 1. Preparación

- Fecha:
- Semilla 7:
- Semilla 8:
- Nombre de la persona:
- Máximo de movimientos:
- Ruta de la referencia:
- Directorio de trabajo:
- Comando para la semilla 7:
- Comando para la semilla 8:

## 2. Predicción — escribir antes de ejecutar

- ¿Qué tres entradas pueden cambiar la transcripción?
- ¿Qué partes de la transcripción dependen de la semilla?
- ¿La semilla 7 y la semilla 8 producirán la misma partida? ¿Qué línea debe diferir primero?
- ¿Qué se rompería si el arnés leyera el reloj, una variable de entorno o una colección sin orden?
- Una frase falsable:

>

## 3. Ejecución

- Estado de salida de la semilla 7:
- Estado de salida de la semilla 8:
- Ruta de la transcripción de la semilla 7:
- Ruta de la transcripción de la semilla 8:
- Algo inesperado:

## 4. Comparar las dos semillas

### 4.1 Comparación de bytes

- Resultado de `cmp` (IDENTICAL / DIFFERENT):
- Estado de salida de `cmp`:

### 4.2 Comparación con diff

- Primera línea que difiere:
- Campos que cambiaron:
- ¿También cambiaron las tiradas, los inicios, las posiciones, los movimientos y el resultado, o
  solo
  la línea de la semilla?

### 4.3 Conteo de turnos

- Conteo de `turn=` para la semilla 7:
- Conteo de `turn=` para la semilla 8:

### 4.4 Qué prueba esto

- Qué prueba la diferencia:
- Qué no prueba:

## 5. Trazar la semilla por el arnés

- Constantes del LCG:
- Máscara:
- Línea donde la semilla inicializa el estado:
- Tres primeras llamadas aleatorias, en orden:
- Un lugar donde se actualiza el estado:
- Un lugar donde el valor generado llega a la transcripción:
- Qué cambiaría primero si se quitara la máscara:
- Qué se movería si se intercambiara el orden de las llamadas:

## 6. Predicción contra observación

- Qué coincidió:
- Qué difirió, con las líneas exactas:
- Qué me dice la brecha sobre mi modelo mental:

## 7. Explicación — por qué se requiere una semilla fija

Escribe un párrafo, sin palabras de sintaxis. Cubre:

- qué necesita una comparación antes de poder atribuir una diferencia a una causa;
- qué dejarías de poder afirmar si la semilla cambiara entre los dos lados;
- por qué “parecía aleatorio” no excusa una ejecución no reproducible;
- la diferencia entre eliminar la aleatoriedad y hacerla repetible.

>

## 8. Autocomprobación

- [ ] Leí el arnés y nombré sus tres entradas.
- [ ] Localicé las constantes del LCG y la máscara.
- [ ] Trazé las tres primeras llamadas aleatorias.
- [ ] Escribí la predicción antes de ejecutar.
- [ ] Ejecuté la semilla 7 y la semilla 8 una vez cada una.
- [ ] `cmp` reporta las dos transcripciones como diferentes.
- [ ] Registré la primera línea que difiere y los campos que cambiaron.
- [ ] Registré el conteo de `turn=` para ambas semillas.
- [ ] Nombré una entrada oculta que rompería el arnés si llegara a la salida.
- [ ] Puedo explicar por qué una semilla fija es un control, no una prueba de corrección.
- [ ] Sé mi siguiente paso.

## 9. Siguiente paso

- Qué puedo explicar ahora:
- Qué no puedo explicar todavía:
- Siguiente lección o repetición:

## 10. Nota opcional

- Cualquier otra cosa:

---

> Recordatorio: este archivo es evidencia para ti, no una nota para nadie más.
