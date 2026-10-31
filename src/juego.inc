;JUEGO.INC

;DibujarFicha
;Proposito: dibujar la ficha de un color, o una casilla vacia si es 0
;Recibe: AL = color (0 = vacia), CX = x, DX = y
;Devuelve: nada
;Destruye: nada

DibujarFicha PROC NEAR
        push ax
        push bx
        or al, al
        jnz DF_Sprite
        RECT cx, dx, 16, 16, COL_CASILLA
        jmp DF_Fin
DF_Sprite:
        mov bl, al              ; BX = (color-1)*2, cada puntero es un DW
        xor bh, bh
        dec bx
        shl bx, 1
        push cx                 ; x
        push dx                 ; y
        mov ax, 16
        push ax                 ; ancho
        push ax                 ; alto
        push tablaFichas[bx]    ; sprite de ese color
        call DibujarSprite
DF_Fin: pop bx
        pop ax
        ret
DibujarFicha ENDP

;MostrarBarra
;Proposito: escribir un mensaje en la barra de abajo (zona D)
;Recibe: SI = cadena, BL = color
;Devuelve: nada
;Destruye: nada

MostrarBarra PROC NEAR
        push dx
        RECT 0, ESTADO_Y, ANCHO_PANT, 10, 0     ; borrar el mensaje anterior
        mov dh, 24
        mov dl, 1
        call EscribirTexto
        pop dx
        ret
MostrarBarra ENDP

;NumADos
;Proposito: convertir un numero 0..99 en dos letras ASCII
;Recibe: AL = numero, DI = donde escribir las 2 letras
;Devuelve: [DI], [DI+1] = decenas, unidades
;Destruye: nada

NumADos PROC NEAR
        push ax
        push bx
        xor ah, ah
        mov bl, 10
        div bl                  ; AL = decenas, AH = unidades
        add ax, 3030h           ; sumar '0' a los dos
        mov [di], al
        mov [di+1], ah
        pop bx
        pop ax
        ret
NumADos ENDP

;DibujarPantJuego
;Proposito: dibujar las 4 zonas de la partida una sola vez al empezar
;Recibe: nivelSel, numColores
;Devuelve: nada
;Destruye: nada

DibujarPantJuego PROC NEAR
        push ax
        push bx
        push cx
        push dx
        push si
        RECT 0, 0, ANCHO_PANT, 200, 0
        RECT TAB_X, TAB_Y, 196, 168, COL_TABLERO    ; zona B
        mov si, OFFSET txtTitulo                    ; zona A
        mov dh, 1
        mov dl, 1
        mov bl, COL_TITULO
        call EscribirTexto
        mov bl, nivelSel                            ; nombre del nivel
        xor bh, bh
        shl bx, 1
        mov si, tablaNivTxt[bx]
        mov dh, 1
        mov dl, 29
        mov bl, COL_RESALT
        call EscribirTexto
        mov si, OFFSET txtSuInt                     ; zona C
        mov dh, 5
        mov dl, 26
        mov bl, COL_NORMAL
        call EscribirTexto
        mov si, OFFSET txtFichas
        mov dh, 10
        call EscribirTexto

        mov al, 1                   ; fichas disponibles, 4 por fila
        mov cx, DISP_X
        mov dx, DISP_Y
DPJ_Ficha:
        call DibujarFicha
        add cx, DISP_PASO
        cmp al, 4
        jne DPJ_Sig
        mov cx, DISP_X              ; segunda fila
        add dx, DISP_PASO
DPJ_Sig:
        inc al
        cmp al, numColores
        jbe DPJ_Ficha

        mov si, OFFSET txtDig1      ; numeros debajo de las fichas
        mov dh, 13
        mov dl, 27
        mov bl, COL_NORMAL
        call EscribirTexto
        mov si, OFFSET txtDig2
        cmp numColores, 8
        jne DPJ_Dig
        mov si, OFFSET txtDig3
DPJ_Dig:
        mov dh, 16
        call EscribirTexto

        DIBUJAR PATO_1, 256, 150, 16, 16
        mov si, OFFSET txtAyuda     ; zona D
        mov bl, COL_NORMAL
        call MostrarBarra
        mov avisoOn, 0
        pop si
        pop dx
        pop cx
        pop bx
        pop ax
        ret
DibujarPantJuego ENDP

;DibujarIndicadores
;Proposito: mostrar "INTENTO 01/10" en el panel (RF-10)
;Recibe: intentoAct, maxIntentos
;Devuelve: nada
;Destruye: nada

DibujarIndicadores PROC NEAR
        push ax
        push bx
        push dx
        push si
        push di
        mov al, intentoAct          ; se muestra contando desde 1
        inc al
        mov di, OFFSET txtIntN + 8
        call NumADos
        mov al, maxIntentos
        mov di, OFFSET txtIntN + 11
        call NumADos
        mov si, OFFSET txtIntN
        mov dh, 3
        mov dl, 26
        mov bl, COL_NORMAL
        call EscribirTexto
        pop di
        pop si
        pop dx
        pop bx
        pop ax
        ret
DibujarIndicadores ENDP

;DibujarIntento
;Proposito: dibujar el intento que se esta armando, con marca en la posicion activa
;Recibe: intento[], numPos, posActiva
;Devuelve: nada
;Destruye: nada

DibujarIntento PROC NEAR
        push ax
        push bx
        push cx
        push dx
        push si
        xor si, si
        mov cx, INTENTO_X
        mov dx, INTENTO_Y
DI_Pos:
        RECT cx, dx, 16, 16, 0      ; borrar la ficha anterior (es transparente)
        mov al, intento[si]
        call DibujarFicha
        mov bx, 0                   ; marca: negra = borrar
        mov ax, si
        cmp al, posActiva
        jne DI_Marca
        mov bx, COL_RESALT          ; amarilla en la posicion activa
DI_Marca:
        RECT cx, INTENTO_Y+18, 16, 2, bx
        add cx, FICHA_PASO
        inc si
        mov al, numPos
        xor ah, ah
        cmp si, ax
        jb DI_Pos
        pop si
        pop dx
        pop cx
        pop bx
        pop ax
        ret
DibujarIntento ENDP

;DibujarFilaHist
;Proposito: dibujar una fila del historial con sus toros y vacas (RF-09)
;Recibe: AL = numero de fila, historial[], torosHist[], vacasHist[]
;Devuelve: nada
;Destruye: nada

DibujarFilaHist PROC NEAR
        push ax
        push bx
        push cx
        push dx
        push si
        push di
        xor ah, ah
        mov di, ax                  ; DI = fila
        mov bl, FILA_ALTO           ; y = TAB_Y + fila*FILA_ALTO
        mul bl
        add ax, TAB_Y
        mov dx, ax
        mov ax, di                  ; SI = fila*5, inicio de la fila en historial
        mov bl, 5
        mul bl
        mov si, ax
        mov cx, FICHA_X0
        mov bl, numPos
DFH_Pos:
        mov al, historial[si]
        call DibujarFicha
        inc si
        add cx, FICHA_PASO
        dec bl
        jnz DFH_Pos

        mov cx, FEED_X              ; un icono por cada toro...
        mov bl, torosHist[di]
DFH_Toro:
        or bl, bl
        jz DFH_Vacas
        DIBUJAR IND_TORO, cx, dx, 16, 16
        add cx, FEED_PASO
        dec bl
        jmp DFH_Toro
DFH_Vacas:
        mov bl, vacasHist[di]       ; ...y luego uno por cada vaca
DFH_Vaca:
        or bl, bl
        jz DFH_Fin
        DIBUJAR IND_VACA, cx, dx, 16, 16
        add cx, FEED_PASO
        dec bl
        jmp DFH_Vaca
DFH_Fin:
        pop di
        pop si
        pop dx
        pop cx
        pop bx
        pop ax
        ret
DibujarFilaHist ENDP

;ValidarIntento
;Proposito: revisar que no quede ninguna posicion vacia (RF-07)
;Recibe: intento[], numPos
;Devuelve: CF = 1 si falta alguna, CF = 0 si esta completo
;Destruye: nada

ValidarIntento PROC NEAR
        push cx
        push si
        xor si, si
        mov cl, numPos
        xor ch, ch
VI_Pos:
        cmp intento[si], 0
        je VI_Mal
        inc si
        loop VI_Pos
        clc
        jmp VI_Fin
VI_Mal: stc
VI_Fin: pop si                      ; pop no cambia las banderas
        pop cx
        ret
ValidarIntento ENDP

;QuitarAviso
;Proposito: si hay un aviso en la barra, volver a poner la ayuda
;Recibe: avisoOn
;Devuelve: avisoOn = 0
;Destruye: nada

QuitarAviso PROC NEAR
        push bx
        push si
        cmp avisoOn, 0
        je QA_Fin
        mov si, OFFSET txtAyuda
        mov bl, COL_NORMAL
        call MostrarBarra
        mov avisoOn, 0
QA_Fin: pop si
        pop bx
        ret
QuitarAviso ENDP

;ArmarIntento
;Proposito: leer teclas hasta tener un intento completo (RF-06, RF-07)
;Recibe: numPos, numColores
;Devuelve: intento[] lleno, CF = 1 si presiono ESC
;Destruye: nada

ArmarIntento PROC NEAR
        push ax
        push bx
        push si
        xor bx, bx                  ; vaciar el intento
AI_Vaciar:
        mov intento[bx], 0
        inc bx
        cmp bx, 5
        jb AI_Vaciar
        mov posActiva, 0
AI_Dibujar:
        call DibujarIntento
AI_Tecla:
        call LeerTecla
        mov bl, posActiva           ; BX = posicion activa
        xor bh, bh
        cmp al, 1Bh
        je AI_Esc
        cmp al, 0Dh
        jne AI_Otra
        call ValidarIntento         ; ENTER
        jnc AI_Fin                  ; completo: CF = 0
        mov si, OFFSET txtAviso     ; incompleto: aviso y no cuenta turno
        mov bl, COL_TITULO
        call MostrarBarra
        mov avisoOn, 1
        jmp AI_Tecla
AI_Esc: stc
AI_Fin: pop si                      ; pop no cambia CF
        pop bx
        pop ax
        ret
AI_Otra:
        cmp al, 08h
        je AI_Borrar
        cmp al, 0
        je AI_Flecha
        sub al, '0'                 ; numero 1..numColores
        jbe AI_Tecla
        cmp al, numColores
        ja AI_Tecla
        jmp AI_Pon
AI_Borrar:
        xor al, al
        jmp AI_Pon
AI_Flecha:
        cmp ah, 4Bh
        je AI_Izq
        cmp ah, 4Dh
        je AI_Der
        cmp ah, 48h
        je AI_Arriba
        cmp ah, 50h
        je AI_Abajo
        jmp AI_Tecla                ; otra tecla especial, se ignora
AI_Izq:
        cmp posActiva, 0
        je AI_Tecla
        dec posActiva
        jmp AI_Dibujar
AI_Der:
        mov al, posActiva
        inc al
        cmp al, numPos
        jae AI_Tecla
        mov posActiva, al
        jmp AI_Dibujar
AI_Arriba:                          ; siguiente color, despues del ultimo vuelve al 1
        mov al, intento[bx]
        inc al
        cmp al, numColores
        jbe AI_Pon
        mov al, 1
        jmp AI_Pon
AI_Abajo:                           ; color anterior, antes del 1 va al ultimo
        mov al, intento[bx]
        cmp al, 1
        jbe AI_Ultimo
        dec al
        jmp AI_Pon
AI_Ultimo:
        mov al, numColores
AI_Pon:
        mov intento[bx], al
        call QuitarAviso
        jmp AI_Dibujar
ArmarIntento ENDP

;PantJuego
;Proposito: una partida completa: secreto, turnos, historial, ganar o perder
;Recibe: nivelSel, modoPrueba
;Devuelve: estado = EST_FIN (resultado = 1 gano, 0 perdio) o EST_MENU con ESC
;Destruye: nada

PantJuego PROC NEAR
        push ax
        push bx
        push cx
        push si
        push di
        call CargarNivel
        call GenerarSecreto
        xor bx, bx                  ; limpiar el historial
PJ_Limpia:
        mov historial[bx], 0
        inc bx
        cmp bx, 12*5
        jb PJ_Limpia
        mov intentoAct, 0
        call DibujarPantJuego

        cmp modoPrueba, 1           ; modo prueba: el secreto se escribe a mano
        jne PJ_Turno
        mov si, OFFSET txtPrueba
        mov bl, COL_TITULO
        call MostrarBarra
        call ArmarIntento
        jc PJ_Menu
        xor bx, bx                  ; el intento armado pasa a ser el secreto
PJ_Copia:
        mov al, intento[bx]
        mov secreto[bx], al
        inc bx
        cmp bx, 5
        jb PJ_Copia
        mov si, OFFSET txtAyuda
        mov bl, COL_NORMAL
        call MostrarBarra

PJ_Turno:
        call DibujarIndicadores
        call ArmarIntento
        jc PJ_Menu
        call EvaluarIntento         ; AL = toros, AH = vacas
        mov cx, ax                  ; guardarlos en CL / CH
        mov bl, intentoAct
        xor bh, bh
        mov torosHist[bx], cl
        mov vacasHist[bx], ch
        mov al, 5                   ; DI = intentoAct*5, inicio de la fila
        mul bl
        mov di, ax
        xor si, si
PJ_Guarda:                          ; copiar el intento al historial
        mov al, intento[si]
        mov historial[di], al
        inc di
        inc si
        cmp si, 5
        jb PJ_Guarda
        mov al, intentoAct
        call DibujarFilaHist        ; solo la fila nueva (RG-08)
        cmp cl, numPos              ; todos toros = gano (RF-11)
        je PJ_Gano
        inc intentoAct
        mov al, intentoAct
        cmp al, maxIntentos
        jb PJ_Turno
        mov resultado, 0            ; se acabaron los intentos
        jmp PJ_Fin
PJ_Gano:
        mov resultado, 1
PJ_Fin:
        mov estado, EST_FIN
        jmp PJ_Sale
PJ_Menu:
        mov estado, EST_MENU
PJ_Sale:
        pop di
        pop si
        pop cx
        pop bx
        pop ax
        ret
PantJuego ENDP

;PantFin
;Proposito: fin de partida, al perder muestra el secreto (RF-11, RF-13)
;Recibe: resultado, secreto[], numPos
;Devuelve: estado = EST_JUEGO (ENTER) o EST_MENU (ESC)
;Destruye: nada

PantFin PROC NEAR
        push ax
        push bx
        push cx
        push dx
        push si
        RECT 0, 0, ANCHO_PANT, 200, 0
        mov si, OFFSET txtGano
        mov bl, COL_GANO
        cmp resultado, 1
        je PF_Titulo
        mov si, OFFSET txtPerdio
        mov bl, COL_TITULO
PF_Titulo:
        mov dh, 5
        mov dl, 11
        call EscribirTexto
        cmp resultado, 1
        je PF_Opc
        mov si, OFFSET txtSecreto   ; perdio: revelar el secreto
        mov dh, 10
        mov dl, 16
        mov bl, COL_NORMAL
        call EscribirTexto
        xor si, si
        mov cx, SEC_X
        mov dx, SEC_Y
        mov bl, numPos
        xor bh, bh
PF_Sec:
        mov al, secreto[si]
        call DibujarFicha
        add cx, FICHA_PASO
        inc si
        cmp si, bx
        jb PF_Sec
PF_Opc:
        DIBUJAR PATO_1, 152, 130, 16, 16
        mov si, OFFSET txtFinOpc
        mov dh, 20
        mov dl, 5
        mov bl, COL_NORMAL
        call EscribirTexto
PF_Tecla:
        call LeerTecla
        mov estado, EST_JUEGO
        cmp al, 0Dh                 ; ENTER: otra partida
        je PF_Sale
        mov estado, EST_MENU
        cmp al, 1Bh                 ; ESC: menu
        jne PF_Tecla
PF_Sale:
        pop si
        pop dx
        pop cx
        pop bx
        pop ax
        ret
PantFin ENDP
