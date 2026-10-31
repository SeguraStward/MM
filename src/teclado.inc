;TECLADO.INC

;LeerTecla
;Proposito: esperar una tecla, las flechas siempre llegan con AL = 0
;Recibe: nada
;Devuelve: AL = ASCII (0 si es flecha u otra especial), AH = codigo de la tecla
;Destruye: nada (AX es lo que devuelve)

LeerTecla PROC NEAR
        mov ah, 00h
        int 16h             ; espera tecla: AL = ASCII, AH = scan code
        cmp al, 0E0h        ; las flechas grises llegan con E0h...
        jne LT_Fin
        xor al, al          ; ...las dejamos igual que las del teclado numerico
LT_Fin: ret
LeerTecla ENDP

;HayTecla
;Proposito: saber si hay una tecla esperando SIN detener el programa
;           (sirve para animar mientras se espera, fase 6)
;Recibe: nada
;Devuelve: ZF = 0 si hay tecla (leerla despues con LeerTecla)
;Destruye: nada

HayTecla PROC NEAR
        push ax
        mov ah, 01h
        int 16h             ; solo mira, actualiza ZF
        pop ax              ; pop no cambia las banderas
        ret
HayTecla ENDP
