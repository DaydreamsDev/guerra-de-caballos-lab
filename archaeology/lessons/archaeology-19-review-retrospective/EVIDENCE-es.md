# archaeology-19 — Evidencia: lista de revisión y retrospectiva

> Este es tu registro. Escríbelo para tu yo futuro.
> Registra lo que observaste, no lo que la lección parecía querer.

## 1. Configuración

- Fecha:
- Ruta del archivo:
- Etiqueta en uso:
- Hash del commit (`git rev-parse HEAD`):
- Compilador y versión (`c++ --version`):
- Shell y versión:
- Directorio de trabajo:
- Directorio temporal usado:
- Ubicación del portafolio:
- Algo inesperado en la configuración:

## 2. Predicción — escribir antes de la pasada sellada

No edites esta tabla después del paso 3. La brecha entre predicción y observación es la señal de
aprendizaje; reparar la predicción la destruye.

| # | Pregunta | Mi predicción | Cómo sabría que me equivoco |
|---:|---|---|---|
| 1 | ¿Cuántos de los diez fallos esperados encontrará la lista en 20 minutos? | | |
| 2 | ¿Qué familia producirá más hallazgos? | | |
| 3 | ¿Qué familia producirá menos? | | |
| 4 | ¿Cuál será el primer hallazgo de severidad alta, y en qué minuto? | | |
| 5 | ¿Qué elemento producirá un falso positivo, y por qué? | | |

Una frase falsable sobre el valor de la lista en esta muestra:

>

## 3. Inventario del portafolio

Cuenta artefactos, no lecciones leídas.

```bash
cd archaeology/lessons
find . -name 'EVIDENCE.md' -o -name 'EVIDENCE-es.md' | sort
find . -name '*.md' | wc -l
grep -rl 'failure-0' --include='*.md' . | sort
```

| Lección | Artefacto que produje | Estado de evidencia | ¿Reutilizable? |
|---|---|---|---|
| archaeology-00 | | | |
| archaeology-01 | | | |
| archaeology-02 | | | |
| archaeology-03 | | | |
| archaeology-04 | | | |
| archaeology-05 | | | |
| archaeology-06 | | | |
| archaeology-07 | | | |
| archaeology-08 | | | |
| archaeology-09 | | | |
| archaeology-10 | | | |
| archaeology-11 | | | |
| archaeology-12 | | | |
| archaeology-13 | | | |
| archaeology-14 | | | |
| archaeology-15 | | | |
| archaeology-16 | | | |
| archaeology-17 | | | |
| archaeology-18 | | | |

Artefactos ausentes y por qué:

>

Los tres artefactos en los que confiaría en una revisión real:

1.
2.
3.

## 4. Auditoría de la plantilla

Lee `REVIEW-CHECKLIST-es.md` y registra los elementos que no son directamente observables.

| Elemento de la plantilla | Por qué es vago | Evidencia que lo resolvería | Mi reescritura |
|---|---|---|---|
| Límites comprobados. | | | |
| Estado global mapeado. | | | |
| Representación oculta. | | | |
| Los errores son valores o bloqueos documentados. | | | |
| El árbol está limpio. | | | |
| | | | |
| | | | |

Qué elementos de la plantilla ya pasan la prueba de dos revisores:

Qué elemento es el más peligroso porque suena completo:

>

## 5. Mi lista

Al menos 15 filas. Cada fila necesita las ocho columnas.

| ID | Familia | Pregunta | Evidencia | Criterio de aprobación | Clase | Escalón | Severidad si falla |
|---|---|---|---|---|---|---|---|
| C-01 | Corrección | | | | | | |
| C-02 | Corrección | | | | | | |
| C-03 | Corrección | | | | | | |
| D-01 | Diseño | | | | | | |
| D-02 | Diseño | | | | | | |
| D-03 | Diseño | | | | | | |
| R-01 | Robustez | | | | | | |
| R-02 | Robustez | | | | | | |
| R-03 | Robustez | | | | | | |
| P-01 | Práctica | | | | | | |
| P-02 | Práctica | | | | | | |
| P-03 | Práctica | | | | | | |
| E-01 | Evidencia | | | | | | |
| E-02 | Evidencia | | | | | | |
| E-03 | Evidencia | | | | | | |
| E-04 | Evidencia | | | | | | |
| | | | | | | | |
| | | | | | | | |

Matriz de cobertura:

| Familia | Elementos | ¿Mínimo cumplido? | Fallos cubiertos | Escalones de Track A usados |
|---|---|---:|---|---|
| Corrección | | 3 | | |
| Diseño | | 3 | | |
| Robustez | | 3 | | |
| Práctica profesional | | 3 | | |
| Evidencia | | 3 | | |

Cobertura de clases:

| Clase | Elementos | ¿Cubierta? |
|---|---|---|
| Corrección | | |
| Diseño | | |
| Robustez | | |
| Práctica profesional | | |

Control de calidad:

| Prueba | Elemento que falló | Qué cambié |
|---|---|---|
| Prueba de dos revisores | | |
| Prueba de evidencia | | |
| Prueba de aprobación | | |
| Prueba de clase | | |
| Prueba de reutilización | | |

Los dos elementos que apliqué a otro código:

1.
2.

Qué pasó:

## 6. Muestra sellada

- Variante elegida:
- Por qué esta variante:
- Ruta de origen:
- Ruta de trabajo:
- Recuento de líneas (`wc -l`):
- ¿Abrí el catálogo durante la pasada cronometrada? (Debe ser no.)

Comando de copia:

```bash
mkdir -p build/review-sample
cp original/variant-02-records-2008/source/guerra-de-caballos-records.cpp \
   build/review-sample/sample.cpp
```

Anclajes que verifiqué antes de la pasada:

| Anclaje | Línea | ¿Verificado? |
|---|---:|---|
| Punto de entrada | 37 | |
| Bucle con off-by-one (`i <= 64`) | 72 | |
| Primer include de `conio.h` o `iostream.h` | | |
| Primera llamada a `system("color")` | | |
| Primer `cout` dentro de la lógica de movimiento | | |
| Llamada a la semilla | | |

## 7. Pasada cronometrada

- Hora de inicio:
- Hora de fin:
- Duración real:
- ¿Me detuve al sonar el temporizador? (Debe ser sí.)

| ID | Hallazgo | Evidencia (fichero:línea o comando) | Clase | Severidad | Elemento de la lista | Veredicto |
|---|---|---|---|---|---|---|
| F-01 | | | | | | confirmado / falso positivo / omitido |
| F-02 | | | | | | |
| F-03 | | | | | | |
| F-04 | | | | | | |
| F-05 | | | | | | |
| F-06 | | | | | | |
| F-07 | | | | | | |
| F-08 | | | | | | |
| F-09 | | | | | | |
| F-10 | | | | | | |
| F-11 | | | | | | |
| F-12 | | | | | | |

Hallazgos que noté después del temporizador:

>

## 8. Comparación con el catálogo

| Esperado | Fallo | ¿Detectado? | Mi evidencia | Elemento que debería haberlo detectado |
|---|---|---|---|---|
| failure-001 | `i <= 64` sobre un arreglo de 64 elementos | | | |
| failure-002 | `void main()` | | | |
| failure-003 | `srand(time(NULL))` dentro de un bucle de reintentos | | | |
| failure-007 | Estado global mutable | | | |
| failure-008 | Ocho cuerpos `case` copiados a mano | | | |
| failure-009 | Arreglos paralelos o variables sueltas del caballo | | | |
| failure-012 | Entrada y salida dentro de la lógica de movimiento | | | |
| failure-015 | `conio.h`, `iostream.h`, `system("color")` | | | |
| failure-016 | Sin pruebas, sin arnés, sin semilla | | | |
| failure-019 | Comentarios que narran o mienten | | | |

Hallazgos inesperados que no están en el catálogo:

>

## 9. Medición

| Métrica | Fórmula | Mi resultado |
|---|---|---|
| Detectados | Fallos esperados confirmados | |
| Omitidos | Fallos esperados no detectados | |
| Falsos positivos | Hallazgos reportados que la evidencia no sostuvo | |
| Tasa de detección | Detectados / 10 | |
| Precisión | Reales / reportados | |
| Tiempo hasta el primer hallazgo de severidad alta | Minutos desde el inicio | |

Qué elemento produjo más valor por minuto:

Qué elemento produjo el falso positivo, y cómo lo reescribiría:

>

Qué fallo omitido fue un hueco de la lista:

Qué fallo omitido fue un hueco del revisor:

Qué no puede probar la muestra:

- No prueba que la lista detecte todas las clases de fallo.
- No prueba que la tasa de detección se generalice a otro código.
- No prueba que el archivo sea correcto o incorrecto.
- No reemplaza una prueba, un sanitizador o una traza.
- No mide falsos negativos fuera de los diez fallos esperados.

## 10. Retrospectiva

El recorrido en una tabla:

| Fase | Lecciones | Qué enseñó | Artefacto |
|---|---|---|---|
| Contexto | 00–03 | | |
| Corrección | 04 | | |
| Diseño | 05–11 | | |
| Práctica | 12–15 | | |
| Entorno | 16 | | |
| Enseñanza | 17 | | |
| Síntesis | 18 | | |
| Revisión | 19 | | |

Cuatro preguntas:

- Diagnóstico más fuerte, con evidencia:
- Diagnóstico más débil, con la evidencia que falta:
- Clase diagnosticada de menos, y por qué:
- Clase diagnosticada de más, y por qué:

Conservar / dejar / empezar:

| Conservar | Dejar | Empezar |
|---|---|---|
| | | |

Qué cambió en mi forma de leer código:

>

Autorrevisión basada en evidencia — qué hizo que un elemento débil pareciera suficiente:

>

## 11. Auditoría de afirmaciones

| # | Afirmación de mi portafolio | Estado | Evidencia | Qué la cambiaría |
|---:|---|---|---|---|
| 1 | | verificada / documentada / inferida / desconocida | | |
| 2 | | | | |
| 3 | | | | |
| 4 | | | | |
| 5 | | | | |

Si tres o más son inferidas o desconocidas, qué dice eso del portafolio:

>

## 12. Explicación

La versión de 60 segundos, escrita:

>

Tres cosas que la lista no puede hacer:

1.
2.
3.

Qué cambiaría antes de usarla en otro código:

>

El escalón de Track A que estudiaría después, y por qué:

>

## 13. Integridad del archivo

- `git status --short` en el archivo:
- ¿Edité `original/`? (Debe ser no.)
- ¿Apliqué algún parche dentro del archivo? (Debe ser no.)
- ¿Dónde guardé mi copia de trabajo?
- ¿Edité el directorio `code/` de la lección? (Debe ser no.)

## 14. Lista de aceptación

- [ ] Inventarié el portafolio y nombré los artefactos ausentes.
- [ ] Audité la plantilla y reescribí al menos cinco elementos vagos.
- [ ] Mi lista tiene al menos 15 elementos en las cinco familias.
- [ ] Cada elemento nombra pregunta, evidencia, criterio de aprobación, clase y escalón.
- [ ] Apliqué la lista a una variante sellada en una copia de trabajo.
- [ ] Registré la pasada cronometrada, los hallazgos y la evidencia de cada hallazgo.
- [ ] Calculé detección, omisiones y falsos positivos.
- [ ] Enuncié los límites de la muestra.
- [ ] Escribí la retrospectiva con un conservar, un dejar y un empezar.
- [ ] Audité cinco afirmaciones como verificadas, documentadas, inferidas o desconocidas.
- [ ] Expliqué la lista en 60 segundos y nombré tres límites.
- [ ] Ligé cada familia a un fallo y a un escalón de Track A.
- [ ] El archivo no está modificado.

## 15. Siguiente paso

- Continúa a Track A: `construction-02`, `construction-08`, `construction-09`, `construction-12`,
  `construction-14`, `construction-20` o `construction-24`.
- O revisa otro código con esta lista y añade una ficha de fallo que el catálogo no tenga.
- Si la tasa de detección te sorprendió, aplica un segundo revisor a la misma muestra y compara.
- Si un elemento siguió fallando la prueba de reutilización, mueve sus nombres del proyecto a la
  columna de evidencia.
- Si la retrospectiva no encontró ningún diagnóstico débil, mira otra vez; un registro perfecto
  suele ser una auditoría débil.
