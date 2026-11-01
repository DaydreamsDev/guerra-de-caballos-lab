# archaeology-02 — Evidencia: inventario de código y grafo de llamadas

> Este es tu registro. Escríbelo para tu yo futuro.
> Registra lo que contaste, no lo que creías que la lección quería.

## 1. Entorno

- Ruta de la fuente:
- Ruta de la copia de trabajo:
- Cantidad de líneas (`wc -l`):
- Suma de verificación del archivo inspeccionado (`sha256sum`):
- Primera línea de `grep --version`:
- Shell y versión:
- Directorio de trabajo:
- Algo inesperado en el entorno:

## 2. Comandos de inspección

Pega el comando y la parte de su salida que usaste. Una fila por comando.

| # | Comando | Salida que usé |
|---|---|---|
| 1 | `wc -l` | |
| 2 | `grep -n '^#include'` | |
| 3 | `grep` de definiciones | |
| 4 | `grep` de declaraciones | |

- Cuántas líneas coincidieron con el `grep` de definiciones:
- Cuántas eran prototipos:
- Cuántas eran definiciones:
- Cómo las separé:
- Cuántos nombres globales conté:
- En qué líneas están declarados:

## 3. Inventario de archivos

| Archivo | Líneas | Rol | Incluido por |
|---|---:|---|---|
| | | | |

- Archivos del directorio de la fuente que no son código:
- Archivos que la fuente incluye y no están en el directorio:

## 4. Inventario de funciones

Regla que usé para el rango: línea de definición hasta la llave de cierre, con blancos incluidos.

| Función | Líneas | Entradas | Salida | Efectos laterales | Llama a |
|---|---:|---|---|---|---|
| | | | | | |
| | | | | | |
| | | | | | |
| | | | | | |
| | | | | | |
| | | | | | |
| | | | | | |
| | | | | | |
| | | | | | |
| | | | | | |
| | | | | | |
| | | | | | |
| | | | | | |

- Suma de los rangos:
- Líneas que no están dentro de ninguna función:
- ¿La suma más el resto equivale a `wc -l`? sí / no
- Si no, ¿qué hueco me faltó?:

### 4.1 Reglas de conteo que usé

- Una definición es:
- Un prototipo es:
- Las llamadas de biblioteca van en:
- El trabajo de una función `void` se registra como:

## 5. Globales y datos

| Nombre | Tipo | Declarado en | Mutado por | Leído por |
|---|---|---|---|---|
| | | | | |
| | | | | |
| | | | | |
| | | | | |
| | | | | |
| | | | | |
| | | | | |
| | | | | |
| | | | | |
| | | | | |
| | | | | |
| | | | | |
| | | | | |
| | | | | |
| | | | | |

### 5.1 Conteo de escrituras

| Nombre | Escrituras | Funciones que escriben |
|---|---:|---:|
| | | |
| | | |
| | | |
| | | |
| | | |

- Patrón que usé para contar una escritura:
- Patrón que usé para excluir una comparación:
- Nombres que casi pierdo, y por qué:
- Globales mutados mediante un parámetro por referencia, y qué parámetro:

## 6. Grafo de llamadas

```text

```

- Raíces:
- Hojas:
- Ciclos:
- Funciones sin llamador:
- Funciones sin llamadas salientes:
- Camino más largo, en aristas:
- Funciones alcanzables desde la raíz:
- Funciones no alcanzables desde la raíz:

### 6.1 Aristas frente a sitios de llamada

| Llamador | Llamados distintos | Sitios de llamada |
|---|---:|---:|
| | | |
| | | |
| | | |
| | | |
| | | |
| **Total** | | |

- Aristas distintas:
- Total de sitios de llamada:
- Por qué los dos números difieren aquí:

### 6.2 Grado

| Nodo | Entrantes | Salientes | Grado |
|---|---:|---:|---:|
| | | | |
| | | | |
| | | | |
| | | | |
| | | | |

- Nodo más conectado, por grado:
- Nodo más llamado, por grado entrante:
- La definición de "conectado" que estoy usando:
- Si hubiera usado la otra definición, la respuesta sería:

## 7. Puntos de entrada

- Entrada del programa:
- Entrada de biblioteca:
- Entrada de prueba:
- Cómo sé que no hay entrada de biblioteca:
- Cómo sé que no hay entrada de prueba:

## 8. Hallazgos

Cada hallazgo necesita una afirmación, un `archivo:línea` y una etiqueta de confianza. La etiqueta
es exactamente una de `verificado`, `inferido` o `desconocido`.

| # | Hallazgo | Evidencia (archivo:línea) | Confianza | Consecuencia o pregunta abierta |
|---|---|---|---|---|
| 1 | | | | |
| 2 | | | | |
| 3 | | | | |
| 4 | | | | |
| 5 | | | | |
| 6 | | | | |
| 7 | | | | |
| 8 | | | | |

### 8.1 Los tres hallazgos obligatorios

- Función más grande, con rango y porcentaje del archivo:
- Función más conectada, con el grado y la definición usada:
- Global más mutado, con el conteo de escrituras y las funciones que escriben:

### 8.2 Código muerto o inalcanzable

| Qué | Evidencia | Por qué es inalcanzable | Confianza |
|---|---|---|---|
| | | | |
| | | | |

- Funciones sin camino entrante desde una raíz:
- Código que parece muerto pero es alcanzable, y por qué:

### 8.3 Lo que la forma predice

Dos predicciones, cada una escrita como un conteo que se podría verificar.

- Predicción 1:
- Predicción 2:
- Qué funciones toca la predicción:
- Qué resultado probaría que la predicción está mal:

## 9. Qué no pude clasificar

| Elemento | Por qué no pude clasificarlo | Qué evidencia lo decidiría |
|---|---|---|
| | | |
| | | |
| | | |

## 10. Párrafo de forma

Un párrafo, sin juicios de valor: dónde vive el trabajo, dónde vive el estado, qué dice el grafo
sobre el cambio.

>

## 11. Autoevaluación

- [ ] Ejecuté los cuatro comandos de inspección y registré su salida.
- [ ] Separé prototipos de definiciones con la regla de la llave.
- [ ] Conté nombres globales, no líneas de declaración.
- [ ] Mis rangos de funciones más el resto equivalen al conteo de líneas.
- [ ] Conté escrituras solo como asignación, incremento o decremento.
- [ ] Registré los globales mutados mediante parámetros por referencia.
- [ ] Dibujé el grafo y nombré una raíz y todas las hojas.
- [ ] Reporté aristas y sitios de llamada como números separados.
- [ ] Definí el grado antes de nombrar el nodo más conectado.
- [ ] Registré el código muerto o inalcanzable con su razón.
- [ ] Cada hallazgo tiene número de línea y etiqueta de confianza.
- [ ] Escribí el párrafo de forma y dos predicciones verificables.
- [ ] La fuente original está sin modificar.

## 12. Paso siguiente

- Qué puedo explicar ahora:
- Qué no puedo explicar todavía:
- Lección siguiente o repetición:
- El defecto que noté y decidí no perseguir:

## 13. Nota opcional

- Cualquier otra cosa:

---

> Recordatorio: este archivo es evidencia para ti.
