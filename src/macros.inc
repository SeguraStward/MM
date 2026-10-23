;MACROS.INC 
;SALIR_DOS: Termina el programa y devuelve el control al DOS

SALIR_DOS MACRO
        mov ax, 4C00h
        int 21h
ENDM
