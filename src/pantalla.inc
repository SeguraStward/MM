LlenarPantalla PROC NEAR
        push cx
        push di
        cld
        xor  di, di
        mov  cx, 64000
        rep  stosb
        pop  di
        pop  cx
        ret
LlenarPantalla ENDP

PantTitulo PROC NEAR
        push ax
        mov  al, 22
        call LlenarPantalla
        call LeerTecla
        mov  estado, EST_MENU
        pop  ax
        ret
PantTitulo ENDP

PantMenu PROC NEAR
        push ax
        mov  al, 19
        call LlenarPantalla
PM_Leer:
        call LeerTecla
        cmp  al, 1Bh
        je   PM_Salir
        cmp  al, '1'
        je   PM_Jugar
        cmp  al, '2'
        je   PM_Nivel
        cmp  al, '3'
        je   PM_Instruc
        cmp  al, '4'
        je   PM_Creditos
        jmp  PM_Leer
PM_Salir:
        mov  estado, EST_SALIR
        jmp  PM_Fin
PM_Jugar:
        mov  estado, EST_JUEGO
        jmp  PM_Fin
PM_Nivel:
        mov  estado, EST_NIVEL
        jmp  PM_Fin
PM_Instruc:
        mov  estado, EST_INSTRUC
        jmp  PM_Fin
PM_Creditos:
        mov  estado, EST_CREDITOS
PM_Fin: pop  ax
        ret
PantMenu ENDP

PantNivel PROC NEAR
        push ax
        mov  al, 25
        call LlenarPantalla
        call LeerTecla
        mov  estado, EST_MENU
        pop  ax
        ret
PantNivel ENDP

PantInstruc PROC NEAR
        push ax
        mov  al, 18
        call LlenarPantalla
        call LeerTecla
        mov  estado, EST_MENU
        pop  ax
        ret
PantInstruc ENDP

PantCreditos PROC NEAR
        push ax
        mov  al, 23
        call LlenarPantalla
        call LeerTecla
        mov  estado, EST_MENU
        pop  ax
        ret
PantCreditos ENDP

PantJuego PROC NEAR
        push ax
        mov  al, 24
        call LlenarPantalla
        call LeerTecla
        mov  estado, EST_MENU
        pop  ax
        ret
PantJuego ENDP

PantFin PROC NEAR
        push ax
        mov  al, 26
        call LlenarPantalla
        call LeerTecla
        mov  estado, EST_MENU
        pop  ax
        ret
PantFin ENDP