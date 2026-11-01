# common-04 — Evidencia: trazas de referencia
> [English](EVIDENCE.md) | [Español](EVIDENCE-es.md)

> Este es tu registro. Escríbelo para tu yo futuro.
> Registra lo que observaste, no lo que la lección parecía querer.

## 1. Preparación

- Fecha:
- Directorio de trazas:
- Directorio de trabajo del comando de resúmenes:
- Semilla reproducida:
- Nombre del jugador:
- Ruta del directorio temporal:
- Shell usado (nombre y versión):

## 2. Predicción — escribir antes de ejecutar

- Si la comprobación de resúmenes pasa, ¿qué he probado exactamente sobre las trazas?
- ¿Qué dirá la salida correcta, palabra por palabra, en esta máquina, y cuál será el estado de
  salida?
- ¿La semilla reproducida dará un diff vacío? ¿Cuántos registros de cabecera tendrá?
- ¿Qué pasa cuando el comando de resúmenes se ejecuta desde la raíz del repositorio, y por qué?
- ¿Reproducir con otro nombre de jugador dará un diff vacío? ¿Qué líneas diferirán?
- ¿Qué podría estar mal en la implementación y aun así pasar las veinte trazas?
- Una frase falsable:

>

## 3. Verificar el conjunto de resúmenes

- Comando:
- Directorio de trabajo:
- Salida completa (pégala, incluido el texto traducido):

```text

```

- Cantidad de líneas de la salida (`sha256sum -c checksums.txt | wc -l`):
- Estado de salida:
- Alguna línea que no informara una coincidencia:

### 3.1 La trampa del directorio

- Comando ejecutado desde la raíz del repositorio:
- Salida observada:
- Estado de salida:
- Mi explicación del fallo, en una frase:

## 4. Reproducir una traza

- Ruta de la traza de referencia:
- Comando de reproducción:
- Salida del diff (vacía, o las líneas exactas):

```text

```

- Estado de salida del diff:
- Resultado de `cmp` (IDENTICAL / DIFFERENT):
- SHA-256 de la traza de referencia:
- SHA-256 de mi transcripción reproducida:
- Primera línea de `checksums.txt`, para comparar:
- ¿Coinciden los tres resúmenes? [ ] sí [ ] no

### 4.1 Qué estableció cada comprobación

- Qué prueba la comprobación de resúmenes:
- Qué prueba la reproducción:
- Qué prueba la comprobación de formato:
- Cuál de las tres fallaría primero si la implementación cambiara un movimiento:

## 5. Control negativo

- Entrada cambiada (semilla, nombre u otra):
- Comando:
- Estado de salida del diff:
- La línea o líneas exactas que difieren:

```text

```

- Qué aísla el diff:
- ¿El control observado coincidió con mi predicción? [ ] sí [ ] no

## 6. Comparar predicción y observación

- Qué coincidió:
- Qué difirió, con las líneas exactas:
- Qué me dice la brecha sobre mi modelo mental:

## 7. Explicación — por qué una traza es evidencia solo con el comando y la salida

Escribe un párrafo, sin palabras de sintaxis. Cubre:

- qué prueba el resumen y qué no prueba;
- qué prueba la reproducción y qué no prueba;
- por qué el comando forma parte de la evidencia y no es solo una forma de producirla;
- qué habría que añadir al conjunto para cubrir una tabla.

>

## 8. Límites de esta evidencia

- Semillas cubiertas:
- Finales cubiertos:
- Finales no cubiertos:
- Un defecto que sobreviviría a este conjunto de trazas:
- Qué decide la especificación cuando una traza y una implementación discrepan:

## 9. Autoverificación

- [ ] Leí `checksums.txt`, una traza completa y el README de trazas.
- [ ] Escribí la predicción antes de ejecutar cualquier comando.
- [ ] La comprobación de resúmenes informó veinte líneas exitosas desde `common/traces`.
- [ ] Registré el texto exacto y el estado de salida, no solo «pasó».
- [ ] Expliqué por qué el mismo comando falla desde la raíz del repositorio.
- [ ] La traza reproducida dio un diff vacío contra su archivo de referencia.
- [ ] Registré el comando, la salida vacía y el estado de salida.
- [ ] El resumen reproducido es igual al resumen registrado.
- [ ] Produje un diff no vacío cambiando una sola entrada y nombré la línea que difiere.
- [ ] Puedo enunciar qué no prueba el conjunto de trazas, sin palabras de sintaxis.
- [ ] Sé cuál es mi siguiente paso.

## 10. Siguiente paso

- Qué puedo explicar ahora:
- Qué no puedo explicar todavía:
- Siguiente lección o repetición:

## 11. Nota opcional

- Cualquier otra cosa:

---

> Recordatorio: este archivo es evidencia para ti, no una nota para nadie más. Una traza sin su
> comando y su salida observada es un espécimen, no una prueba.
