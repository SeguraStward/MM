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


.CODE
INICIO PROC FAR
        mov ax, @DATA 
        mov ds, ax
        call IniciarVideo

        ; prueba 
        cld
        mov di, 20 * ANCHO_PANT 
        mov cx, ANCHO_PANT
        mov al, 4  
        rep stosb

        mov ah, 00h ; esperar una tecla para ver el resultado
        int 16h

        call RestaurarVideo
        SALIR_DOS
INICIO ENDP

INCLUDE video.inc

END INICIO