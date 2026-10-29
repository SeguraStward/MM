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

        ;dibujarsprites
        mov ax, 150     ;x
        push ax
        mov ax, 90      ;y
        push ax
        mov ax, 16      ;ancho
        push ax
        push ax         ;alto (tambien 16)
        mov ax, OFFSET PATO_1
        push ax
        call DibujarSprite


        mov ah, 00h ; esperar una tecla para ver el resultado
        int 16h

        call RestaurarVideo
        SALIR_DOS
INICIO ENDP

INCLUDE video.inc

END INICIO