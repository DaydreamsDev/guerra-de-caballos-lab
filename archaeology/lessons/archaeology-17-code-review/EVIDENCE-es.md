# archaeology-17 — Evidencia: revisión de código de una función heredada

> Este es tu registro. Escríbelo para tu yo futuro.
> Registra lo que observaste, no lo que la lección esperaba que respondieras.

## 1. Entorno

- Ruta de la fuente:
- Función bajo revisión:
- Compilador y versión (si lo usaste):
- Shell y versión:
- Terminal y `TERM`:
- Directorio de trabajo:
- Directorio de trabajo usado para copias:
- Algo inesperado en el entorno:

## 2. Alcance

| Campo | Valor |
|---|---|
| Archivo | |
| Función | |
| Líneas revisadas | |
| Llamadores | |
| Globales leídos | |
| Globales escritos | |
| Salidas | |
| Fuera de alcance | |

Por qué elegí esta función:

>

Cómo encontré los llamadores:

```bash
grep -n '<función>' <archivo>
```

Llamadores observados:

## 3. Notas de inspección

Contrato, en una frase:

>

Estado leído:

Estado escrito:

Flujo de control, en una frase:

Ramas y bucles:

Salidas tempranas y caminos de error:

Qué promete el nombre que el cuerpo no hace:

Qué hace el cuerpo que el nombre no promete:

## 4. Hallazgos

Al menos tres filas. Cada fila tiene los seis campos. Usa la escala de severidad: crítica, alta,
media o baja.

| ID | Ubicación | Síntoma | Causa raíz | Severidad | Evidencia | Prueba que lo detectaría |
|---|---|---|---|---|---|---|
| F-01 | | | | | | |
| F-02 | | | | | | |
| F-03 | | | | | | |
| F-04 (opcional) | | | | | | |

Frase de severidad por fila. Llena los huecos: si esto se dispara, **\<observable\>** le pasa a
**\<quién\>**, en **\<contexto\>**.

- F-01:
- F-02:
- F-03:

El contexto que precifiqué:

>

Preferencias que separé de los hallazgos:

| Preferencia | Por qué no es un hallazgo |
|---|---|
| | |

Una fila que fusioné con otra, y por qué:

>

## 5. Peldaños de evidencia

| Hallazgo | Peldaño alcanzado (leer, grep, compilar, ejecutar, probar) | Qué demuestra ese peldaño |
|---|---|---|
| F-01 | | |
| F-02 | | |
| F-03 | | |

Comandos que ejecuté, con su propósito:

```bash
nl -ba <archivo> | sed -n '<inicio>,<fin>p'
grep -n '<función>' <archivo>
```

- Qué compilé, si algo:
- Qué ejecuté, si algo:
- Qué mostró la salida:

## 6. Priorización

Las tres reglas, aplicadas en orden: severidad, luego alcanzabilidad, luego coste y certeza.

| Orden | Hallazgo | Regla que decide | Por qué |
|---:|---|---|---|
| 1 | | | |
| 2 | | | |
| 3 | | | |

- Arreglar primero:
- Arreglar después:
- Dejar como evidencia:
- Por qué la fila que se deja como evidencia no se arregla ahora:

El contraorden, y el contexto que lo produciría:

>

## 7. Contrato de prueba

El hallazgo de mayor riesgo es:

El nombre de la prueba:

| Parte | Contenido |
|---|---|
| Nombre | |
| Dado | |
| Cuando | |
| Entonces | |
| Oráculo | |
| Falla antes del arreglo | |
| Pasa después del arreglo | |
| Ejecución | |

La costura que la prueba necesita:

```cpp

```

- ¿La firma actual permite la prueba? Si no, ¿qué falta?
- ¿La costura es un cambio mínimo o un rediseño? ¿Cómo lo sé?
- ¿La prueba tiene un caso negativo, para que no pueda pasar por la razón equivocada?

El boceto de la prueba, si escribí uno:

```cpp

```

- ¿La ejecuté? Si no, ¿por qué no?
- ¿Qué la haría fallar por la razón correcta?

## 8. Qué no cambiaría

| Comportamiento o estructura | Por qué lo mantengo |
|---|---|
| | |
| | |

- Qué necesito más evidencia para decidir:
- Qué parece un defecto pero lo exige un llamador:

## 9. Límites de la revisión

- Qué leí:
- Qué ejecuté:
- Qué no ejecuté:
- Qué inferí:
- Qué subiría el hallazgo de arriba al siguiente peldaño:

## 10. Integridad de la fuente

- ¿Edité `original/`? (Debe ser no.)
- ¿Apliqué algún parche dentro del árbol de la fuente? (Debe ser no.)
- ¿Dónde guardé mi copia de trabajo?
- ¿Modifiqué el código de la lección? (Debe ser no.)

## 11. Lista de aceptación

- [ ] Elegí una función y registré su archivo, rango de líneas y llamadores.
- [ ] Escribí la ficha de alcance antes de los hallazgos.
- [ ] Escribí al menos tres hallazgos distintos.
- [ ] Cada hallazgo tiene ubicación, síntoma, causa raíz, severidad, evidencia y prueba.
- [ ] Cada severidad tiene una frase de consecuencia con un contexto.
- [ ] Separé al menos una preferencia de los hallazgos.
- [ ] Ordené los hallazgos con las tres reglas y una razón por fila.
- [ ] Nombré arreglar primero, arreglar después y dejar como evidencia.
- [ ] Nombré una prueba para el hallazgo de mayor riesgo.
- [ ] La prueba tiene dado, cuando, entonces y un oráculo.
- [ ] Declaré si la prueba falla antes del arreglo y pasa después.
- [ ] Nombré la costura que la prueba necesita.
- [ ] Registré el peldaño de evidencia de cada hallazgo.
- [ ] Escribí los límites: leí, ejecuté, no ejecuté, inferí.
- [ ] Registré un comportamiento que no cambiaría, con una razón.
- [ ] La fuente heredada está sin modificar.

## 12. Paso siguiente

- Continúa con `archaeology-18` — taller de refactorización.
- Si me interesó la costura de prueba, ve a `archaeology-11` — entrada y salida.
- Si quiero practicar severidad, reviso la misma función en un segundo contexto.
- Si no estoy de acuerdo con el orden, escribo el contexto que lo cambia.
