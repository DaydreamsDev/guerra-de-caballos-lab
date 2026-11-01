# archaeology-00 — Evidencia: orientación

> Este es tu registro. Escríbelo para tu yo futuro.
> Registra lo que observaste, no lo que creías que la lección quería.

## 1. Preparación

- Directorio de trabajo:
- Ruta del código:
- Cantidad de líneas del código (`wc -l`):
- Compilador y versión (`c++ --version`):
- Shell y versión:
- Directorio temporal usado para la compilación:
- Algo inesperado en la preparación:

## 2. Predicción — escribir antes de leer cualquier cuerpo de función

No edites esta tabla después del paso 5. La diferencia entre predicción y observación es la señal de
aprendizaje; reparar la predicción la destruye.

| # | Pregunta | Mi predicción | Cómo sabría que está mal |
|---|---|---|---|
| 1 | Propósito en una línea | | |
| 2 | Interfaz: pantalla, registro o sin salida | | |
| 3 | Qué declaraciones son estado mutable | | |
| 4 | Qué inicia y qué termina el programa | | |
| 5 | Primera cosa que se rompe en mi máquina | | |

Una frase falsable sobre el conjunto:

>

## 3. Punto de entrada

- Archivo:
- Función:
- Primera llamada:
- Última llamada:
- Qué inicia el programa:
- Qué termina el programa:
- Cómo sale el control del programa:
- Evidencia (números de línea):

## 4. Funciones

Una fila por función. Usa nombres en las columnas de relación y números de línea en la última.

| Función | Propósito | Llamada por | Llama a | Lee | Escribe | Líneas |
|---|---|---|---|---|---|---|
| | | | | | | |
| | | | | | | |
| | | | | | | |
| | | | | | | |
| | | | | | | |
| | | | | | | |
| | | | | | | |
| | | | | | | |

Agrega filas hasta que cada función tenga la suya. Una función que no puedes explicar lleva `?` en
la columna de propósito y una etiqueta de confianza en la última.

## 5. Estado

Una fila por nombre con alcance de archivo. Separa el estado mutable de las constantes.

| Global o estructura | Tipo | Escrito por | Leído por | Por qué existe | Confianza |
|---|---|---|---|---|---|
| | | | | | |
| | | | | | |
| | | | | | |
| | | | | | |
| | | | | | |
| | | | | | |

Constantes excluidas de la tabla de estado, y por qué:

- Nombres:
- Tipo:
- Por qué no son estado:

## 6. Flujo de datos

- Entrada:
- Transformación:
- Salida:
- Dónde se muta el estado:
- Dónde se produce la salida:

Un valor trazado desde la entrada hasta la salida. Escribe una línea por flecha, con su número de
línea.

```text

```

## 7. Compilación y ejecución

- Comando de compilación:
- Resultado (compila / compila pero no ejecuta / no compila):
- Primer resultado observable, literal:

```text

```

- Punto de fallo (archivo:línea):
- Comando de ejecución, si se produjo un binario:
- Estado de salida:
- Primera línea de salida:
- Primera línea de error:
- Qué habría significado `124`:

## 8. Modelo mental

- Una frase: ¿qué es este programa?
- La parte que entiendo mejor:
- La parte que todavía no puedo explicar:
- La siguiente pregunta que respondería:
- Por qué responderla cambiaría al menos una fila del mapa:

### 8.1 Veredictos de la predicción

| # | Predicho | Evidencia del mapa o de la compilación | Veredicto: confirmada / contradicha / sin resolver |
|---|---|---|---|
| 1 | | | |
| 2 | | | |
| 3 | | | |
| 4 | | | |
| 5 | | | |

- Qué fila fue la sorpresa más útil, y por qué:

### 8.2 La prueba de la frase

| Prueba | Mi respuesta |
|---|---|
| Predicción: si cambio el orden de movimientos, ¿qué cambia? | |
| Compresión: ¿la frase cubre entrada, estado y salida? | |
| Falsabilidad: ¿qué línea probaría que está mal? | |

## 9. Límite de la evidencia

| Afirmación | Estado: leído / ejecutado / inferido / desconocido | Evidencia o razón |
|---|---|---|
| El punto de entrada está en la línea citada. | | |
| La función citada escribe el global citado. | | |
| El propósito del programa es la frase que escribí. | | |
| El resultado de compilación vale en otra máquina. | | |

## 10. Autoevaluación

- [ ] Escribí cinco predicciones antes de leer cualquier cuerpo de función.
- [ ] Leí `main` completo y nombré qué inicia y qué termina el programa.
- [ ] Agrupé los prototipos por trabajo y corregí la agrupación después de leer.
- [ ] Leí un caso de `movcab` y la rama `default`, y registré la repetición.
- [ ] Cada función tiene una fila con propósito, llamadores, llamadas, lecturas y escrituras.
- [ ] Cada global mutable tiene una fila con tipo, escritores, lectores y razón de existir.
- [ ] Separé los símbolos `const char` del estado mutable.
- [ ] Trazé un valor desde la entrada hasta la salida con una línea por flecha.
- [ ] Intenté la compilación y registré el comando y el primer resultado observable.
- [ ] Escribí la frase de propósito y la probé de tres maneras.
- [ ] Escribí una siguiente pregunta que nombra un símbolo, un archivo o una línea.
- [ ] Etiqueté cada afirmación incierta como inferida o desconocida.
- [ ] El código está sin modificar y no se escribió ningún artefacto en su árbol.

## 11. Siguiente paso

- Qué puedo explicar ahora:
- Qué no puedo explicar todavía:
- Lección siguiente o repetición:
- La observación que aparqué en lugar de perseguir:

## 12. Nota opcional

- Cualquier otra cosa:

---

> Recordatorio: este archivo es evidencia para ti.
