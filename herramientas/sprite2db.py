#!/usr/bin/env python3
"""sprite2db.py: herramienta de DISEÑO para el Proyecto II (EIF205). No forma parte del juego.

Convierte sprites dibujados como texto en tablas DB para TASM y genera la hoja de sprites.

Entrada (carpeta sprites/):
  paleta.txt   una línea por color:   <indice> <caracter> <R> <G> <B> <nombre>
               R, G, B van de 0 a 63 (el DAC del VGA usa 6 bits). Ej:  16 R 63 10 10 rojo_cuerpo
               El caracter '.' siempre es el indice 0 = TRANSPARENTE (no se declara).
  *.txt        uno o varios sprites por archivo:
               @NOMBRE ANCHO ALTO
               <ALTO lineas de ANCHO caracteres>

Salida:
  src/paleta.inc       PAL_INICIO EQU / PAL_CANT EQU / tabla PALETA DB r,g,b,...
  src/sprites.inc      NOMBRE DB ... (un byte por pixel, izquierda→derecha, arriba→abajo)
  doc/hoja_sprites.png hoja con todos los sprites ampliados (entregable E5)
  doc/tabla_paleta.md  tabla de la paleta para el documento técnico (RG-07)

Uso:  python3 sprite2db.py RAIZ_DEL_PROYECTO     (la que contiene sprites/, src/ y doc/)
Solo usa la biblioteca estándar de Python.
"""
import os
import struct
import sys
import zlib


def leer_paleta(ruta):
    pal = {}  # caracter -> (indice, r, g, b, nombre)
    for n, linea in enumerate(open(ruta, encoding="utf-8"), 1):
        linea = linea.split("#")[0].strip()
        if not linea:
            continue
        p = linea.split()
        if len(p) < 5:
            sys.exit(f"paleta.txt:{n}: se esperaban 'indice caracter R G B nombre'")
        idx, ch, r, g, b = int(p[0]), p[1], int(p[2]), int(p[3]), int(p[4])
        if ch == "." or len(ch) != 1:
            sys.exit(f"paleta.txt:{n}: el caracter debe ser uno solo y distinto de '.'")
        if not (1 <= idx <= 255) or not all(0 <= v <= 63 for v in (r, g, b)):
            sys.exit(f"paleta.txt:{n}: indice 1..255 y R,G,B 0..63")
        pal[ch] = (idx, r, g, b, p[5] if len(p) > 5 else "")
    return pal


def leer_sprites(carpeta, pal):
    sprites = []  # (nombre, ancho, alto, [indices])
    for arch in sorted(os.listdir(carpeta)):
        if not arch.endswith(".txt") or arch == "paleta.txt":
            continue
        lineas = [l.rstrip("\n") for l in open(os.path.join(carpeta, arch), encoding="utf-8")]
        i = 0
        while i < len(lineas):
            l = lineas[i].strip()
            if not l.startswith("@"):
                i += 1
                continue
            nombre, ancho, alto = l[1:].split()
            ancho, alto = int(ancho), int(alto)
            filas = lineas[i + 1:i + 1 + alto]
            datos = []
            for f, fila in enumerate(filas):
                fila = fila.strip()
                if len(fila) != ancho:
                    sys.exit(f"{arch}: sprite {nombre}, fila {f + 1} mide {len(fila)} y debe medir {ancho}")
                for ch in fila:
                    if ch == ".":
                        datos.append(0)
                    elif ch in pal:
                        datos.append(pal[ch][0])
                    else:
                        sys.exit(f"{arch}: sprite {nombre}: caracter '{ch}' no está en paleta.txt")
            if len(filas) != alto:
                sys.exit(f"{arch}: sprite {nombre} tiene {len(filas)} filas y debe tener {alto}")
            sprites.append((nombre, ancho, alto, datos))
            i += 1 + alto
    return sprites


def escribir_inc(raiz, pal, sprites):
    os.makedirs(os.path.join(raiz, "src"), exist_ok=True)
    cols = sorted(pal.values())
    inicio = cols[0][0]
    if [c[0] for c in cols] != list(range(inicio, inicio + len(cols))):
        sys.exit("paleta.txt: los indices deben ser consecutivos (ej. 16, 17, 18, ...)")
    with open(os.path.join(raiz, "src", "paleta.inc"), "w", newline="\r\n") as f:
        f.write("; Generado por herramientas/sprite2db.py -- no editar a mano\n")
        f.write(f"PAL_INICIO EQU {inicio}\nPAL_CANT   EQU {len(cols)}\n")
        f.write("PALETA LABEL BYTE\n")
        for idx, r, g, b, nom in cols:
            f.write(f"        DB {r:2d},{g:2d},{b:2d}   ; {idx:3d} {nom}\n")
    with open(os.path.join(raiz, "src", "sprites.inc"), "w", newline="\r\n") as f:
        f.write("; Generado por herramientas/sprite2db.py -- no editar a mano\n")
        f.write("; Un byte por pixel (indice de paleta), 0 = transparente\n")
        for nombre, ancho, alto, datos in sprites:
            f.write(f"\n; {nombre}: {ancho} x {alto} = {ancho * alto} bytes\n{nombre} LABEL BYTE\n")
            for y in range(alto):
                fila = datos[y * ancho:(y + 1) * ancho]
                f.write("        DB " + ",".join(f"{v:3d}" for v in fila) + "\n")


def escribir_png(ruta, ancho, alto, rgb):
    raw = b"".join(b"\x00" + bytes(rgb[y * ancho * 3:(y + 1) * ancho * 3]) for y in range(alto))

    def chunk(t, d):
        return struct.pack(">I", len(d)) + t + d + struct.pack(">I", zlib.crc32(t + d) & 0xFFFFFFFF)
    with open(ruta, "wb") as f:
        f.write(b"\x89PNG\r\n\x1a\n" + chunk(b"IHDR", struct.pack(">IIBBBBB", ancho, alto, 8, 2, 0, 0, 0))
                + chunk(b"IDAT", zlib.compress(raw, 9)) + chunk(b"IEND", b""))


def escribir_hoja(raiz, pal, sprites, zoom=6, margen=8, por_fila=6):
    color = {idx: (r * 255 // 63, g * 255 // 63, b * 255 // 63) for idx, r, g, b, _ in pal.values()}
    celda_w = max(s[1] for s in sprites) * zoom + margen
    celda_h = max(s[2] for s in sprites) * zoom + margen
    filas = (len(sprites) + por_fila - 1) // por_fila
    W, H = por_fila * celda_w + margen, filas * celda_h + margen
    img = bytearray([60, 60, 60] * W * H)
    for k, (nombre, an, al, datos) in enumerate(sprites):
        ox = margen + (k % por_fila) * celda_w
        oy = margen + (k // por_fila) * celda_h
        for y in range(al):
            for x in range(an):
                v = datos[y * an + x]
                # transparente = cuadriculado gris claro/oscuro
                c = color[v] if v else ((200, 200, 200) if (x + y) % 2 else (150, 150, 150))
                for dy in range(zoom):
                    for dx in range(zoom):
                        p = ((oy + y * zoom + dy) * W + ox + x * zoom + dx) * 3
                        img[p:p + 3] = bytes(c)
    os.makedirs(os.path.join(raiz, "doc"), exist_ok=True)
    escribir_png(os.path.join(raiz, "doc", "hoja_sprites.png"), W, H, img)
    with open(os.path.join(raiz, "doc", "tabla_paleta.md"), "w", encoding="utf-8") as f:
        f.write("| Índice | Carácter | R | G | B | Uso |\n|---|---|---|---|---|---|\n")
        for ch, (idx, r, g, b, nom) in sorted(pal.items(), key=lambda kv: kv[1][0]):
            f.write(f"| {idx} | `{ch}` | {r} | {g} | {b} | {nom} |\n")
        f.write("\nOrden de la hoja de sprites: " + ", ".join(s[0] for s in sprites) + "\n")


def main():
    raiz = sys.argv[1] if len(sys.argv) > 1 else "."
    carpeta = os.path.join(raiz, "sprites")
    pal = leer_paleta(os.path.join(carpeta, "paleta.txt"))
    sprites = leer_sprites(carpeta, pal)
    if not sprites:
        sys.exit("No encontré sprites (@NOMBRE ANCHO ALTO) en sprites/*.txt")
    escribir_inc(raiz, pal, sprites)
    escribir_hoja(raiz, pal, sprites)
    total = sum(s[1] * s[2] for s in sprites)
    print(f"OK: {len(pal)} colores, {len(sprites)} sprites, {total} bytes de datos")


if __name__ == "__main__":
    main()
