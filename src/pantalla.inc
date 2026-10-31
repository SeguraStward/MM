;PANTALLA.INC

;PantTitulo
;Proposito: presentacion con nombre, tematica, integrantes y mascota (RF-01)
;Recibe: nada
;Devuelve: estado = EST_MENU
;Destruye: nada

PantTitulo PROC NEAR
        push ax
        push bx
        push dx
        push si
        RECT 0, 0, ANCHO_PANT, 200, 0       ; limpiar pantalla
        mov si, OFFSET txtTitulo
        mov dh, 3
        mov dl, 7
        mov bl, COL_TITULO
        call EscribirTexto
        mov si, OFFSET txtIntegr
        mov dh, 20
        mov dl, 4
        mov bl, COL_NORMAL
        call EscribirTexto
        mov si, OFFSET txtTecla
        mov dh, 23
        mov dl, 8
        mov bl, COL_NORMAL
        call EscribirTexto
        DIBUJAR PATO_1, 152, 90, 16, 16     ; en la fase 6 se anima
        call LeerTecla                      ; cualquier tecla sigue
        mov estado, EST_MENU
        pop si
        pop dx
        pop bx
        pop ax
        ret
PantTitulo ENDP

;PantVacia
;Proposito: pantalla temporal para lo que todavia no existe
;Recibe: nada
;Devuelve: estado = EST_MENU al presionar ESC
;Destruye: nada

PantVacia PROC NEAR
        push ax
        push bx
        push dx
        push si
        RECT 0, 0, ANCHO_PANT, 200, 0
        mov si, OFFSET txtPronto
        mov dh, 12
        mov dl, 6
        mov bl, COL_NORMAL
        call EscribirTexto
PV_Tecla:
        call LeerTecla
        cmp al, 1Bh             ; solo ESC vuelve al menu
        jne PV_Tecla
        mov estado, EST_MENU
        pop si
        pop dx
        pop bx
        pop ax
        ret
PantVacia ENDP



;PantMenu
;Proposito: menu principal, flechas mueven la seleccion, ENTER elige (RF-02)
;Recibe: nada
;Devuelve: estado = destino de la opcion elegida
;Destruye: nada

PantMenu PROC NEAR
        push ax
        push bx
        push cx
        push dx
        push si
        RECT 0, 0, ANCHO_PANT, 200, 0
        mov si, OFFSET txtTitulo
        mov dh, 3
        mov dl, 7
        mov bl, COL_TITULO
        call EscribirTexto

PM_Dibujar:                     ; escribe las 5 opciones, la elegida en otro color
        xor bx, bx              ; BX = posicion en opcMenu (0, 2, 4...)
        mov cx, N_OPC
        mov dh, 9               ; fila de la primera opcion
PM_Opc:
        mov si, opcMenu[bx]     ; SI = direccion del texto de esta opcion
        push bx
        shr bx, 1               ; BX/2 = numero de opcion (0..4)
        cmp bl, opcSel
        mov bl, COL_NORMAL      ; mov no cambia las banderas del cmp
        jne PM_Color
        mov bl, COL_RESALT
PM_Color:
        mov dl, 15
        call EscribirTexto
        pop bx
        add bx, 2               ; siguiente DW
        add dh, 2               ; dos filas mas abajo
        loop PM_Opc

PM_Tecla:
        call LeerTecla
        cmp al, 0Dh             ; ENTER: elegir la opcion
        je PM_Enter
        cmp al, 'p'             ; P activa/desactiva el modo prueba
        je PM_Prueba
        cmp al, 'P'
        je PM_Prueba
        cmp al, 0               ; si no es flecha, se ignora
        jne PM_Tecla
        cmp ah, 48h             ; flecha arriba
        je PM_Arriba
        cmp ah, 50h             ; flecha abajo
        je PM_Abajo
        jmp PM_Tecla            ; otra tecla especial (F1...), se ignora
PM_Arriba:
        cmp opcSel, 0           ; ya esta en la primera, no sube
        je PM_Tecla
        dec opcSel
        jmp PM_Dibujar          ; redibujar opciones con el nuevo resaltado
PM_Abajo:
        cmp opcSel, N_OPC-1     ; ya esta en la ultima, no baja
        je PM_Tecla
        inc opcSel
        jmp PM_Dibujar
PM_Prueba:
        xor modoPrueba, 1       ; 0 -> 1, 1 -> 0
        mov si, OFFSET txtModoOff
        cmp modoPrueba, 1
        jne PM_Aviso
        mov si, OFFSET txtModoOn
PM_Aviso:
        mov dh, 21
        mov dl, 14
        mov bl, COL_TITULO
        call EscribirTexto
        jmp PM_Tecla

PM_Enter:
        mov bl, opcSel          ; estado = destinos[opcSel]
        xor bh, bh
        mov al, destinos[bx]
        mov estado, al
        pop si
        pop dx
        pop cx
        pop bx
        pop ax
        ret
PantMenu ENDP
