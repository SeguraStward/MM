;MACROS.INC 
;SALIR_DOS: Termina el programa y devuelve el control al DOS

SALIR_DOS MACRO
        mov ax, 4C00h
        int 21h
ENDM


;DIBUJAR spr, px, py, an, alt: dibuja el sprite cuya etiqueta es spr
;los parametros pueden ser numeros, constantes, variables o registros (menos AX)
DIBUJAR MACRO spr, px, py, an, alt
        push ax             ; la macro usa AX, lo protege
        mov ax, px
        push ax
        mov ax, py
        push ax
        mov ax, an
        push ax
        mov ax, alt
        push ax
        mov ax, OFFSET spr
        push ax
        call DibujarSprite
        pop ax
ENDM

;RECT px, py, an, alt, col: rectangulo de un solo color
RECT MACRO px, py, an, alt, col
        push ax
        mov ax, px
        push ax
        mov ax, py
        push ax
        mov ax, an
        push ax
        mov ax, alt
        push ax
        mov ax, col
        push ax
        call RellenarRect
        pop ax
ENDM
