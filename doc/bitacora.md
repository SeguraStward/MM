# Bitácora del Proyecto II — Mastermind

> Una entrada por sesión de trabajo. Sirve para el **E6 (bitácora)** y la sección
> **"Dificultades"** del documento técnico (E4). Escríbela con tus palabras.

## Plantilla (copia esto en cada sesión)

### AAAA-MM-DD — Fase N: título corto
- **Qué hice:** (archivos y PROC que escribí)
- **Qué aprendí:** (el porqué, en 1–3 frases, como si se lo explicara a alguien)
- **Errores / dificultades:**
  | Síntoma (qué vi) | Causa (por qué pasó) | Solución |
  |---|---|---|
  |  |  |  |
- **Uso de IA:** (qué pregunté y qué me explicó; va a la declaración de IA)
- **Commit:** `tipo(ámbito): resumen`
- **Horas:**

---

## Entradas

### 2026-10-10 — Fase 0: setup
- **Qué hice:** DOSBox 0.74-3 + TASM 3.2, estructura de carpetas, `build.bat`, prueba de humo act1 → imprime `12`.
- **Errores / dificultades:**
  | Síntoma | Causa | Solución |
  |---|---|---|
  | 2da compilación: `Fatal: Error writing object file` | `tasm ... ,bin\mm.obj` (nombre de archivo de salida) falla en TASM 3.2 + DOSBox | Usar `,bin\` (solo la carpeta) |
  | act1 imprimía mal el 2º dígito | `INT 21h/02h` deja AL = DL, pisa lo que había en AL | Guardar las unidades en BH antes de imprimir |

---
  
### 2026-10-23 — Fase 1: esqueleto y modo 13h
- **Qué hice:** 
- - Cree macros.inc y en ella la macro para salir de el DOS, cuando utilice la macro en el programa ese nombre sera reemplazado por las instrucciones para salir.
- - Cree dos PROC que son como funciones, una para Iniciar el modo video y otra para salir del modo video
- **Qué aprendí:** 
- - Diferencia entre Macro y PROC
- - Registros de segmento. DS apunta a una zona en donde se encuentran los datos que utilizamos en la pantalla por eso todos los programas empienzan con " mov ax, @DATA  mov ds, ax" y no se puede hacer mov ds, @DATA por decision de diseno.
- - ES apunta a la VRAM, ES es Extra segment y es un segmento libre que uno puede utilizar con lo que sea, esta vez lo utilizamos para apuntar a la zona de la vram y lo que yo escriba en esa zona se pintara en la pantalla porque la pantalla lee esa memoria.
- - aprendi sobre la pila (push, pop) y como se pueden agregar valores de los registros y luego obtenerlos(LIFO)
- - aprendi sobre "int 10h" se comporta como un peticion donde ah es un endpoint y los demas registros son parametros dependiendo del servicio por ejemplo:  mov ax, 0013h  (ah = 00 y 00 significa o comanda a fijar un modo, al = 13h seria cual modo se debe fijar que en este caso es el 13h)
- - la VRAM y el offset. la vram es una sola fila larga de 64 000 bytes y para pintar un pixel en la fila 35 debo de multiplicar por 320 porque cada 320 bytes avanzo a la siguiente fila o sea el eje y, una vez en una fila sumo x y ese es el punto en donde se dibuja o sea offset = y * 320 + x 
- **Errores / dificultades:**
  | Síntoma (qué vi) | Causa (por qué pasó) | Solución |
  |---|---|---|
  |  |  |  |
- **Uso de IA:** 
- **Commit:** `tipo(ámbito): resumen`
- **Horas:**