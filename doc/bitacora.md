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
- **Qué hice:** Cree macros.inc y en ella la macro para salir de el DOS, cuando utilice la macro en el programa ese nombre sera reemplazado por las instrucciones para salir.
- **Qué aprendí:** Diferencia entre Macro y PROC
- **Errores / dificultades:**
  | Síntoma (qué vi) | Causa (por qué pasó) | Solución |
  |---|---|---|
  |  |  |  |
- **Uso de IA:** (qué pregunté y qué me explicó; va a la declaración de IA)
- **Commit:** `tipo(ámbito): resumen`
- **Horas:**