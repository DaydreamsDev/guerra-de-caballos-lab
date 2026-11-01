# common-03 — Evidencia: compilar y ejecutar

> [English](EVIDENCE.md) | [Español](EVIDENCE-es.md)

> Este es tu registro. Escríbelo para tu yo futuro.
> Registra lo que observaste, no lo que la lección parecía querer.

## 1. Preparación

- Fecha:
- Ruta del repositorio:
- Directorio temporal:
- Shell y versión:
- Salida de `python3 --version`:
- Ruta de la referencia:
- Resumen esperado para la semilla 7 y el jugador Learner:
  `b8ff5f7999322ac770a5b37056e86f7eb4fe8c71ea3f4fd5c457cc720af27dba`

## 2. Predicción — escribir antes de ejecutar

- Desde la raíz del repositorio, ¿el comando relativo saldrá con `0`? ¿Por qué?
- Desde `/tmp`, ¿el mismo comando relativo saldrá con `0`? ¿Qué dirá el shell?
- Desde `/tmp`, ¿la ruta absoluta saldrá con `0`? ¿Por qué?
- ¿Producirán las dos ejecuciones exitosas transcripciones idénticas byte a byte? ¿Qué podría
  hacerlas diferentes?
- ¿Qué línea de `run.sh` hace que la ejecución absoluta funcione desde otro directorio?
- ¿Qué pasaría si `run.sh` llamara a `python3 reference.py` sin `scriptDirectory`?
- ¿Qué predice la lógica de ubicación del envoltorio sobre las salidas y qué no predice?
- Una frase falsable:

>

## 3. Ejecución desde la raíz del repositorio

- Directorio de trabajo:
- Comando:
- Estado de salida:
- Ruta de la transcripción:
- Resultado de `wc -l`:
- Últimas dos líneas:

```text

```

## 4. Ejecución desde otro directorio

- Directorio de trabajo:
- Comando:
- Estado de salida:
- Ruta de la transcripción:
- Algo inesperado:

### Control con la ruta relativa

- Directorio de trabajo:
- Comando:
- Estado de salida:
- Mensaje exacto del shell:

```text

```

## 5. Comparación

### 5.1 Comparación byte a byte

- Resultado de `cmp` (IDENTICAL / DIFFERENT):
- Estado de salida de `cmp`:

### 5.2 Comparación de resúmenes

- SHA-256 desde la raíz del repositorio:
- SHA-256 desde el otro directorio:
- ¿Coinciden los resúmenes? [ ] sí [ ] no
- ¿Ambos son iguales al resumen esperado? [ ] sí [ ] no

### 5.3 Comparación con diff

- ¿El diff entre las dos transcripciones estaba vacío? [ ] sí [ ] no
- Primera línea que difiere, si existe:
- Campo que difiere:

### 5.4 Cruce con la traza de referencia

- Ruta de la traza:
- ¿El diff contra la traza estaba vacío? [ ] sí [ ] no
- Primera línea que difiere, si existe:

### 5.5 Qué prueba la comparación

- ¿Qué campo de la transcripción podría haber contenido el directorio de trabajo?
- ¿Por qué importa la ausencia de ese campo?

>

## 6. Predicción contra observación

- Qué coincidió:
- Qué difirió, con las líneas exactas:
- Qué me dice la brecha sobre mi modelo mental:
- ¿El control con la ruta relativa falló exactamente como lo predijiste? [ ] sí [ ] no

## 7. Explicación — por qué no hay paso de compilación

Escribe un párrafo, sin palabras de sintaxis. Cubre:

- qué produce normalmente un paso de compilación y por qué ese artefacto está ausente aquí;
- qué hace el intérprete en su lugar y cuándo lo hace;
- qué resuelve el envoltorio antes de que arranque el programa y qué deja sin controlar;
- por qué la versión del intérprete forma parte de los controles aunque no se compile nada;
- por qué un port a un lenguaje compilado agrega un paso de compilación sin cambiar el concepto.

>

## 8. Autoverificación

- [ ] Leí `run.sh` y puedo explicar qué hace `scriptDirectory`.
- [ ] Escribí la predicción antes de ejecutar nada.
- [ ] Ejecuté la semilla 7 y el jugador Learner desde la raíz; el estado de salida fue `0`.
- [ ] Ejecuté la ruta absoluta desde otro directorio; el estado de salida fue `0`.
- [ ] Ejecuté la ruta relativa desde otro directorio y observé el fallo esperado.
- [ ] `cmp` indica que las dos transcripciones exitosas son idénticas.
- [ ] Los dos resúmenes SHA-256 coinciden y ambos son iguales al resumen esperado.
- [ ] El diff entre las dos transcripciones está vacío.
- [ ] El diff contra `common/traces/seed-0007.txt` está vacío.
- [ ] Registré la versión de Python y los comandos exactos.
- [ ] Puedo explicar por qué no hay paso de compilación, sin palabras de sintaxis.
- [ ] Sé cuál es mi siguiente paso.

## 9. Siguiente paso

- Qué puedo explicar ahora:
- Qué no puedo explicar todavía:
- Siguiente lección o repetición:

## 10. Nota opcional

- Cualquier otra cosa:

---

> Recordatorio: este archivo es evidencia para ti, no una nota para nadie más.
