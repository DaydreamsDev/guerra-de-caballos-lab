# archaeology-05 — Evidencia: estado global y acoplamiento oculto

> Este es tu registro. Escríbelo para tu yo futuro.
> Registra lo que observaste, no lo que esperabas que la lección pidiera.

## 1. Configuración

- Fecha:
- Ruta del archivo:
- Etiqueta en uso:
- Hash del commit (`git rev-parse HEAD`):
- Shell y versión:
- Kernel y distribución (`uname -sr`):
- Directorio de trabajo:
- Algo inesperado en la configuración:

## 2. Predicción — escribir antes de leer las funciones

No edites esta tabla después del paso 3. La diferencia entre predicción y observación es la señal de
aprendizaje; reparar la predicción la destruye.

| # | Pregunta | Mi predicción | Cómo sabría que me equivoco |
|---:|---|---|---|
| 1 | Global escrito por más funciones | | |
| 2 | Global leído pero no escrito tras la configuración | | |
| 3 | Global que carga más de un significado | | |
| 4 | Función más difícil de probar | | |

Una frase falsable sobre el conjunto:

>

## 3. Ejecución del escáner

Comando:

```bash
./archaeology/lessons/archaeology-05-global-state/scan_globals.sh /ruta/al/archivo
```

Conteos de menciones observados:

```text
vectab=
vecjugadas=
cab1=
cab2=
jugada=
orden=
ganador=
cf1=
cc1=
cf2=
cc2=
cab=
simcab1=
simcab2=
sep=
```

- Código de salida:
- Ruta del archivo usada:
- Encabezados de función que reconoció el escáner:
- Por qué el número de encabezados no es el número de funciones:

El conteo de menciones es un punto de partida, no el mapa. Una mención no es una mutación.

## 4. Mapa de globales — mutaciones por global

Añade filas si encuentras un global que la plantilla no liste.

| Global | Tipo | Declarado en | Escrito por | Leído por | Efectos laterales | Ruta de mutación | Confianza |
|---|---|---|---|---|---|---|---|
| `vectab` | | | | | | | |
| `vecjugadas` | | | | | | | |
| `cab1` | | | | | | | |
| `cab2` | | | | | | | |
| `jugada` | | | | | | | |
| `orden` | | | | | | | |
| `ganador` | | | | | | | |
| `cf1` | | | | | | | |
| `cc1` | | | | | | | |
| `cf2` | | | | | | | |
| `cc2` | | | | | | | |
| `cab` | | | | | | | |

Símbolos de archivo de solo lectura excluidos del mapa, con la razón:

- `simcab1`:
- `simcab2`:
- `sep`:

## 5. Mapa de funciones

| Función | Lee | Escribe | Imprime | Acoplamiento oculto |
|---|---|---|---|---|
| `main` | | | | |
| `inicializacion` | | | | |
| `cabubicacion` | | | | |
| `impresion` | | | | |
| `conversionvm` | | | | |
| `conversionmv` | | | | |
| `vecjug` | | | | |
| `imprevecjug` | | | | |
| `movcab` | | | | |
| `ordenjuego` | | | | |
| `impresionorden` | | | | |
| `impresion0` | | | | |

Escritores indirectos que marqué como indirectos:

-

## 6. Rutas de mutación

Escribe cada ruta como una cadena de llamadas que termina en el valor que cambia.

- Ruta 1:
- Ruta 2:
- Ruta 3:
- Ruta más difícil de probar:
- Por qué es la más difícil:

## 7. Frontera de evidencia

- Verificado en la fuente:
- Inferido de los sitios de llamada:
- Desconocido:

## 8. Arreglos

- Arreglo mínimo (una costura):
- Arreglo estructural:
- Enlace a la pista A:
- Prueba que detectaría una regresión:
- Qué no arregla el arreglo mínimo:
- Qué no arregla el arreglo estructural:

## 9. Qué prueba y qué no prueba el mapa

| Afirmación | ¿Soportada? | Por qué |
|---|---|---|
| Un global se escribe en la línea citada. | | |
| Una función depende de un global que no recibe. | | |
| Una función imprime como efecto lateral. | | |
| El orden de mutación en ejecución es el del mapa. | | |
| El fallo de diseño causa un resultado equivocado. | | |
| El arreglo mínimo elimina el acoplamiento. | | |
| El arreglo estructural elimina la clase. | | |

## 10. Severidad y coste

Un párrafo. Nombra el coste en trabajo real, no la fealdad del código. Di si el coste es medio o
alto en tu contexto y por qué. No propongas una reescritura en este párrafo.

>

## 11. Integridad del archivo

- `git status` en el archivo:
- ¿Edité `original/`? (Debe ser no.)
- ¿Apliqué algún parche dentro del archivo? (Debe ser no.)
- ¿Dónde guardé mi copia de trabajo, si hice una?

## 12. Siguiente paso

- Continúa con `archaeology-06` — duplicación y el `switch` de ocho casos.
- Relee el global que ordenaste último. ¿El orden fue por cantidad de mutaciones o por consecuencia?
  Escribe una frase.
- Si un global no tuvo escritor en tu mapa, comprueba si se escribe a través de un llamado antes de
  concluir que es constante.
