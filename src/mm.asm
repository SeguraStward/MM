;MM.ASM - Mastermind - EIF205 II Ciclo 2026
;Integrantes: Angel Segura, Cristhian Castillo
;Ensamblado: build.bat
;
INCLUDE macros.inc

.MODEL SMALL
.STACK 400h

ANCHO_PANT   EQU 320 ; pixeles por fila en modo 13h

; estados de pantalla
EST_TITULO   EQU 0
EST_MENU     EQU 1
EST_NIVEL    EQU 2
EST_INSTRUC  EQU 3
EST_CREDITOS EQU 4
EST_JUEGO    EQU 5
EST_FIN      EQU 6
EST_SALIR    EQU 7

.DATA
    modoOriginal DB ?  ; modo de video del DOS
    INCLUDE paleta.inc
    estado    DB EST_TITULO
    tablaPant DW PantTitulo, PantMenu, PantNivel, PantInstruc
              DW PantCreditos, PantJuego, PantFin

.CODE
INICIO PROC FAR
        mov ax, @DATA
        mov ds, ax
        call IniciarVideo

        mov si, OFFSET PALETA
        mov al, PAL_INICIO
        mov cx, PAL_CANT
        call CargarPaleta

BUCLE:  cmp  estado, EST_SALIR
        je   TERMINAR
        mov  bl, estado              ; BX = estado * 2
        xor  bh, bh
        shl  bx, 1
        call tablaPant[bx]           ; llama a la pantalla actual
        jmp  BUCLE

TERMINAR:
        call RestaurarVideo
        SALIR_DOS
INICIO ENDP

INCLUDE video.inc
INCLUDE teclado.inc
INCLUDE pantalla.inc

END INICIO