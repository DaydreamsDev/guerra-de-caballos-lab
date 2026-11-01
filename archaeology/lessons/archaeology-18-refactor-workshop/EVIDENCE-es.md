# archaeology-18 — Evidencia: taller de refactorización

> Este es tu registro. Escríbelo para tu yo futuro.
> Registra lo que observaste, no lo que el taller parecía querer.
>
> El informe (`WORKSHOP-REPORT-es.md`) argumenta el caso. Este archivo lo demuestra. Cada afirmación
> del informe debería tener un comando y una salida aquí.

## 1. Configuración

- Fecha:
- Ruta del archivo:
- Etiqueta en uso:
- Hash del commit (`git rev-parse HEAD`):
- Compilador y versión (`c++ --version`):
- Banderas de compilación usadas:
- Versión de `git`:
- ¿Sanitizador disponible? (`c++ -fsanitize=address` sobre un archivo de una línea):
- Directorio de trabajo usado:
- Shell y versión:
- Algo inesperado en la configuración:

## 2. Elección

| Campo | Valor |
|---|---|
| Fallo elegido (lección + ID de fallo) | |
| Ubicación en el archivo (`variant:línea`) | |
| Clase de fallo | |
| Escalón objetivo | |
| Prueba de cambio del escalón, copiada de `lesson-catalog-es.md` | |

Una frase: por qué este fallo:

>

Una frase: por qué este escalón:

>

La frase del defecto, con la forma "el programa hace X porque Y no está en Z":

>

Comprobación: ¿el escalón toca `Y` o `Z`? Si no, el emparejamiento está mal.

## 3. El banco

| Pregunta | Respuesta |
|---|---|
| ¿Qué región del archivo transcribí? | |
| ¿Qué cambié para que compile? | |
| ¿Qué entrada controlé (semilla, nombre, jugadas guionizadas, reloj guionizado)? | |
| ¿Qué **no** reparé a propósito? | |
| ¿Dónde va la traza? | |

La transcripción, en una línea por evento:

```cpp

```

Si el defecto es el no determinismo, cómo controlé la entrada sin borrar el defecto:

>

Dos ejecuciones, misma entrada:

```bash
./bench 17 > run-1.txt
./bench 17 > run-2.txt
cmp run-1.txt run-2.txt
```

- Código de salida de `cmp`:
- ¿Determinista? (debe ser sí):

Qué habría convertido el banco en una demostración en lugar de una línea base:

>

## 4. Traza A

```bash
./bench 17 > trace-a.txt
sha256sum trace-a.txt
```

- Comando:
- Digest SHA-256:
- Conteo de bytes (`wc -c < trace-a.txt`):
- Conteo de líneas (`wc -l < trace-a.txt`):
- Commit que la registró:
- ¿Edité `trace-a.expected.txt` en algún momento? (debe ser no):

## 5. Conteos de acoplamiento (antes)

| Métrica | Comando | Valor |
|---|---|---:|
| Archivos del artefacto | `ls *.cpp *.h \| wc -l` | |
| Líneas del artefacto | `cat *.cpp *.h \| wc -l` | |
| Nombres públicos que un llamador debe conocer | declaraciones en cabeceras | |
| Archivos que deben leerse para responder "¿cómo se almacena una celda?" | `grep -rln` | |

Para la cuarta fila, el mismo número puede significar dos cosas opuestas:

- el archivo que debe leerse es el **llamador**: la representación se filtró;
- el archivo que debe leerse es la **implementación**: la representación está oculta.

Cuál tengo:

>

Los mismos conteos del archivo, para comparar:

| Métrica | Archivo | Mi banco |
|---|---:|---:|
| Archivos | | |
| Líneas | | |
| Líneas que mencionan el nombre del almacenamiento | | |
| Cabeceras | | |

## 6. Plan y predicción

Archivos a copiar:

Archivos a cambiar:

Archivos a crear:

Intacto:

La predicción, escrita antes de la refactorización y no editada después:

| Métrica | Previsto | Por qué ese número |
|---|---:|---|
| Archivos tocados | | |
| Líneas añadidas / borradas | | |
| Nombres públicos después | | |
| Pruebas después | | |
| Tiempo de compilación | | |

La prueba de cambio del escalón, reformulada como un comando que puedo ejecutar:

>

## 7. Bitácora de pasos de la refactorización

Una fila por paso confirmado. Cada fila debe mostrar una traza verde.

| Paso | Commit | Qué cambió | Archivos tocados | Traza A |
|---:|---|---|---:|---|
| 1 | | | | |
| 2 | | | | |
| 3 | | | | |
| 4 | | | | |
| 5 | | | | |

Pasos que revertí, y por qué:

| Paso | Síntoma | A qué revertí |
|---:|---|---|
| | | |

¿Revertí un paso o toda la refactorización? (lo primero es lo correcto):

>

El comando que ejecuté después de cada paso:

```bash
c++ -std=c++20 -Wall -Wextra -Werror -O2 *.cpp -o bench-new
./bench-new 17 | cmp - trace-a.expected.txt
```

## 8. Verificación

### Compilación

- Comando:
- Diagnósticos:
- ¿`-Werror` detuvo algo? ¿Qué?

### Trazas

```bash
./bench-new 17 | cmp - trace-a.expected.txt
diff -u trace-a.expected.txt <(./bench-new 17) || true
```

- Código de salida de `cmp`:
- Salida de `diff`:
- Si el artefacto es el juego completo, trazas doradas coincidentes: ___ de 20

### Determinismo

- ¿Dos ejecuciones idénticas? (debe ser sí):
- Comando:

### Sanitizador

```bash
c++ -std=c++20 -g -O1 -fsanitize=address,undefined *.cpp -o bench-asan
./bench-asan 17 > asan-out.txt 2> asan-err.txt
```

- ¿`asan-out.txt` coincidió con la traza A?
- Reporte del sanitizador antes de la refactorización:
- Reporte del sanitizador después de la refactorización:
- ¿Cambió el reporte? Si sí, cuál de las dos explicaciones aplica:
  - arreglé el defecto dentro de la refactorización (la tabla de costes está mal);
  - moví la escritura donde el sanitizador no puede verla (peor que el original).

### La bifurcación de corrección

Solo si el fallo elegido es un defecto de corrección.

| Elemento | Valor |
|---|---|
| Comando de la traza B | |
| Digest de la traza B | |
| Líneas que difieren entre A y B | |
| Cambio que causó cada línea | |
| Commit del arreglo | |

```bash
diff -u trace-a.expected.txt trace-b.txt
```

- Salida de `diff`:
- ¿Cada línea cambiada está atribuida al arreglo y no a la refactorización?

## 9. Coste medido

| Métrica | Antes | Después | Delta | Comando |
|---|---:|---:|---:|---|
| Archivos | | | | |
| Líneas | | | | |
| Nombres públicos | | | | |
| Pruebas | | | | |
| Tiempo de compilación (mediana de 5) | | | | |

Tiempo de compilación, las cinco ejecuciones:

| Ejecución | Segundos |
|---:|---:|
| 1 | |
| 2 | |
| 3 | |
| 4 | |
| 5 | |
| Mediana | |

Qué métrica me sorprendió, y por qué:

>

Qué métrica predije peor, y por cuánto:

>

## 10. La prueba de cambio del escalón

| Pregunta | Respuesta |
|---|---|
| El cambio que el escalón promete abaratar | |
| La edición que hice para probarlo | |
| Archivos que tuve que tocar | |
| ¿Toqué el llamador? | |
| ¿Las trazas siguieron pasando después de la edición de prueba? | |
| ¿Revertí la edición de prueba después? | |

Si el conteo de archivos es el mismo que antes del escalón, el escalón no se alcanzó. Qué haría
después:

>

## 11. Frontera de evidencia

- Verificado leyendo la fuente:
- Verificado compilando:
- Verificado ejecutando la comparación de trazas:
- Verificado ejecutando el conjunto dorado:
- Verificado ejecutando un sanitizador:
- Verificado con `git diff --numstat`:
- Inferido:
- No medido:
- Desconocido:

## 12. Qué prueban y qué no prueban las trazas

| Afirmación | ¿Sostenida? | Por qué |
|---|---|---|
| Para la entrada registrada, la salida es idéntica byte a byte. | | |
| Ningún comportamiento observable cambió para las entradas que ejecuté. | | |
| Los dos programas son equivalentes para toda entrada. | | |
| La refactorización no introdujo comportamiento indefinido. | | |
| La refactorización eliminó el defecto. | | |
| La capacidad del escalón se ganó. | | |
| El diseño es mejor. | | |
| El archivo está arreglado. | | |

La afirmación más fuerte que puedo hacer desde este taller:

>

La afirmación más débil que alguien podría leer en él:

>

## 13. Integridad del archivo

- `git status` en el archivo:
- ¿Edité `original/`? (debe ser no):
- ¿Apliqué algún parche dentro del archivo? (debe ser no):
- ¿Copié material del archivo al árbol versionado? (debe ser no):
- ¿Dónde guardé mi copia de trabajo?
- ¿Edité la estructura de `WORKSHOP-REPORT-es.md` en lugar de rellenarla?

## 14. Lista de aceptación

- [ ] Elegí un fallo y un escalón, y justifiqué ambos en una frase por cada uno.
- [ ] Mi banco transcribe el defecto y no lo repara.
- [ ] Mi banco controla su entrada en lugar de borrar el defecto.
- [ ] Dos ejecuciones del banco con la misma entrada produjeron los mismos bytes.
- [ ] Registré la traza A, su comando y su digest SHA-256 antes de la primera edición.
- [ ] Registré los cuatro conteos de acoplamiento, y dije cuál es el de representación.
- [ ] Escribí la lista de cambios, incluida la lista de lo intacto.
- [ ] Escribí la tabla de coste previsto antes de la refactorización y no la edité después.
- [ ] Copié la prueba de cambio del escalón desde el catálogo a mi plan.
- [ ] Cada paso confirmado compiló con `-Wall -Wextra -Werror`.
- [ ] Cada paso confirmado mantuvo la traza A idéntica byte a byte.
- [ ] Revertí los pasos fallidos individualmente, no toda la refactorización.
- [ ] Ejecuté la prueba de cambio del escalón y registré cuántos archivos tocó.
- [ ] Ejecuté la comprobación de determinismo sobre el artefacto refactorizado.
- [ ] Ejecuté un sanitizador y expliqué si su reporte cambió.
- [ ] Si tomé la bifurcación de corrección, la traza B existe y cada línea cambiada está atribuida.
- [ ] La tabla de costes contiene valores medidos, no adjetivos.
- [ ] El veredicto nombra qué compró el escalón, qué costó y qué no arregló.
- [ ] `WORKSHOP-REPORT-es.md` está completo.
- [ ] El archivo está sin modificar.

## 15. Siguiente paso

- Continúa a `archaeology-19` — la lista de revisión y la retrospectiva final.
- Si mi predicción falló por un factor de dos, la razón es:
- Si la refactorización tardó más de lo planificado, el plan que escribiría ahora es:
- El escalón que probaría después, y su precio:
