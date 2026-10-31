;MM.ASM - Mastermind - EIF205 II Ciclo 2026
;Integrantes: Angel Segura, Cristhian Castillo
;Ensamblado: build.bat
;
INCLUDE macros.inc

.MODEL SMALL
.STACK 400h

ANCHO_PANT EQU 320 ; pixeles por fila en modo 13h

.DATA
    modoOriginal DB ?  ; modo de video del DOS
    txtTitulo DB 'MASTERMIND: DATA PIPELINE', 0

        INCLUDE paleta.inc
        INCLUDE sprites.inc


.CODE
INICIO PROC FAR
        mov ax, @DATA 
        mov ds, ax
        call IniciarVideo
        
        mov si, OFFSET PALETA
        mov al, PAL_INICIO  
        mov cx, PAL_CANT
        call CargarPaleta

                ;titulo
        mov si, OFFSET txtTitulo
        mov dh, 2           ; fila 2
        mov dl, 7           ; columna 7
        mov bl, 23          ; amarillo de la paleta
        call EscribirTexto

        ;fondo gris y las 8 fichas encima (checkpoint de la fase 2)
        RECT 0, 80, ANCHO_PANT, 40, 20
        DIBUJAR F_NUBE,    16, 92, 16, 16
        DIBUJAR F_ENGR,    52, 92, 16, 16
        DIBUJAR F_EMBUDO,  88, 92, 16, 16
        DIBUJAR F_FLECHA, 124, 92, 16, 16
        DIBUJAR F_JSON,   160, 92, 16, 16
        DIBUJAR F_RAYO,   196, 92, 16, 16
        DIBUJAR F_BD,     232, 92, 16, 16
        DIBUJAR F_CSV,    268, 92, 16, 16
        DIBUJAR PATO_1,   152, 150, 16, 16

 

        mov ah, 00h ; esperar una tecla para ver el resultado
        int 16h

        call RestaurarVideo
        SALIR_DOS
INICIO ENDP

INCLUDE video.inc

END INICIO