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
- **Que hice:** instale DOSBox 0.74-3 y TASM 3.2 (TLINK, TD), arme las carpetas del proyecto, build.bat, run.bat, debug.bat y mm.conf. Prueba de humo con el act1 del curso (suma 4 + 8 e imprime 12).
- **Errores / dificultades:**
  | Sintoma | Causa | Solucion |
  |---|---|---|
  | la 2da compilacion daba `Fatal: Error writing object file` | en build.bat ponia `,bin\mm.obj` y TASM 3.2 en DOSBox no lo soporta bien | poner solo la carpeta `,bin\` |
  | el act1 imprimia mal el segundo digito | `INT 21h/02h` deja AL = DL y pisa lo que tenia en AL | guardar las unidades en BH antes de imprimir |
- **Uso de IA:** me ayudo con la instalacion y a encontrar por que fallaba la segunda compilacion.
- **Commit:** `Fase 0: estructura, build.bat, mm.conf y prueba de humo`
- **Horas:** 2h

---

### 2026-10-17 al 19 — Estudio (sin codigo del proyecto)
- **Que hice:** semana del parcial de MAT006 y del Quiz 3, asi que solo estudie. Repaso de registros, segmentos, pila, saltos e interrupciones. Practica suelta en DOSBox (`~/asm/PRACT`).
- **Que aprendi:** int 21h es como llamar una API del DOS, AH dice que servicio (09h imprimir, 4Ch salir) y DX lleva la direccion del texto.
- **Uso de IA:** explicaciones y ejercicios de repaso para el quiz.
- **Horas:** 3h

---

### 2026-10-23 — Fase 1: esqueleto y modo 13h
- **Que hice:**
  - Cree macros.inc y en ella la macro para salir de el DOS, cuando utilice la macro en el programa ese nombre sera reemplazado por las instrucciones para salir.
  - Cree dos PROC que son como funciones, una para Iniciar el modo video y otra para salir del modo video
  - En mm.asm pinte un pixel y despues una linea con rep stosb
- **Que aprendi:**
  - Diferencia entre Macro y PROC
  - Registros de segmento. DS apunta a una zona en donde se encuentran los datos que utilizamos en la pantalla por eso todos los programas empienzan con " mov ax, @DATA  mov ds, ax" y no se puede hacer mov ds, @DATA por decision de diseno.
  - ES apunta a la VRAM, ES es Extra segment y es un segmento libre que uno puede utilizar con lo que sea, esta vez lo utilizamos para apuntar a la zona de la vram y lo que yo escriba en esa zona se pintara en la pantalla porque la pantalla lee esa memoria.
  - aprendi sobre la pila (push, pop) y como se pueden agregar valores de los registros y luego obtenerlos(LIFO)
  - aprendi sobre "int 10h" se comporta como un peticion donde ah es un endpoint y los demas registros son parametros dependiendo del servicio por ejemplo:  mov ax, 0013h  (ah = 00 y 00 significa o comanda a fijar un modo, al = 13h seria cual modo se debe fijar que en este caso es el 13h)
  - la VRAM y el offset. la vram es una sola fila larga de 64 000 bytes y para pintar un pixel en la fila 35 debo de multiplicar por 320 porque cada 320 bytes avanzo a la siguiente fila o sea el eje y, una vez en una fila sumo x y ese es el punto en donde se dibuja o sea offset = y * 320 + x.
  - stosb instruccion que hace dos cosas, escribir AL en ES:[DI] y aumentar DI, asi esta hecho en 8086 las instrucciones individuales para aumentar, escribir debieron de repetirse mucho como para crear una instruccion que haga las dos cosas
- **Errores / dificultades:**
  | Sintoma | Causa | Solucion |
  |---|---|---|
  | no pintaba en la pantalla | puse `0A00h` en vez de `0A000h`, me falto un cero | corregir a `0A000h` |
  | TASM se quejaba de la PROC | a RestaurarVideo le faltaba el `ENDP` | agregar `RestaurarVideo ENDP` |
  | la linea salio verde y no roja | rep stosb escribe lo que hay en AL y yo nunca hice `mov al, 4`, AL tenia basura | poner `mov al, 4` antes del rep stosb |
  | la linea media 321 pixeles | pinte uno con mov y despues 320 con rep stosb | dejar solo el rep stosb con CX = 320 |
- **Uso de IA:** me explico macro vs PROC, push/pop, ES y la VRAM, y me ayudo a encontrar los errores de arriba.
- **Commit:** `feat(video): linea roja con rep stosb`
- **Horas:** 4h

---

### 2026-10-24 — Fase 2: tematica, sprites y paleta
- **Que hice:**
  - Escogimos la tematica: Data Engineering (pipeline roto, toro = etapa en su lugar, vaca = etapa en otro lugar, mascota patito de debugging)
  - sprites en texto (sprites/fichas.txt) y paleta (sprites/paleta.txt), los converti con sprite2db.py a sprites.inc y paleta.inc
  - CargarPaleta en video.inc, programa el DAC con OUT 3C8h y 3C9h
- **Que aprendi:**
  - un sprite es una lista de bytes, cada byte es un indice de la paleta, fila por fila, igual que la vram
  - los colores del DAC van de 0 a 63 porque usa 6 bits
  - el puerto va en DX porque 3C8h es mayor a 255, y lo que se envia sale de AL
  - lodsb lee DS:[SI] en AL y sube SI, cld hace que avance hacia adelante, loop resta CX y repite
- **Errores / dificultades:**
  | Sintoma | Causa | Solucion |
  |---|---|---|
  | la linea seguia roja despues de cargar la paleta | la linea usaba el color 4 y la paleta nueva es del 16 al 26 | dibujar con el 22 (naranja) y el 26 para probar el primer y el ultimo color |
- **Uso de IA:** los bocetos de los sprites y los valores de la paleta los propuso Claude, yo los revise. Me explico OUT, los puertos y lodsb.
- **Commit:** `feat(colores): cargar paleta de colores proppios`
- **Horas:** 4h
