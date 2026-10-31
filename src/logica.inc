;LOGICA.INC

;IniciarAzar
;Proposito: sembrar el generador con el reloj del BIOS (RF-04)
;Recibe: nada
;Devuelve: semilla (variable)
;Destruye: nada

IniciarAzar PROC NEAR
        push ax
        push cx
        push dx
        mov ah, 00h
        int 1Ah             ; BIOS: CX:DX = tics desde medianoche
        mov semilla, dx     ; la parte baja cambia 18 veces por segundo
        pop dx
        pop cx
        pop ax
        ret
IniciarAzar ENDP

;Azar
;Proposito: siguiente numero pseudoaleatorio en el rango 0..BL-1
;Recibe: BL = rango (1..255)
;Devuelve: AL = numero en 0..BL-1
;Destruye: AH

Azar PROC NEAR
        push dx
        mov ax, semilla     ; semilla = semilla*25173 + 13849
        mov dx, 25173
        mul dx              ; nos quedamos con AX, el mod 65536 sale gratis
        add ax, 13849
        mov semilla, ax
        mov al, ah          ; el byte alto es mas aleatorio que el bajo
        xor ah, ah
        div bl              ; AL = cociente, AH = residuo (0..BL-1)
        mov al, ah
        pop dx
        ret
Azar ENDP

;CargarNivel
;Proposito: copiar la fila del nivel elegido a numPos, numColores, etc.
;Recibe: nivelSel
;Devuelve: numPos, numColores, permiteRep, maxIntentos
;Destruye: nada

CargarNivel PROC NEAR
        push ax
        push bx
        mov bl, nivelSel    ; BX = nivelSel * 4 (cada fila mide 4 bytes)
        xor bh, bh
        shl bx, 1
        shl bx, 1
        mov al, niveles[bx]
        mov numPos, al
        mov al, niveles[bx+1]
        mov numColores, al
        mov al, niveles[bx+2]
        mov permiteRep, al
        mov al, niveles[bx+3]
        mov maxIntentos, al
        pop bx
        pop ax
        ret
CargarNivel ENDP


;GenerarSecreto
;Proposito: llenar secreto[] al azar segun el nivel (RF-04, RF-05)
;Recibe: numPos, numColores, permiteRep
;Devuelve: secreto[]
;Destruye: nada

GenerarSecreto PROC NEAR
        push ax
        push bx
        push cx
        push si
        push di
        call IniciarAzar
        xor di, di              ; DI = posicion que se esta llenando
GS_Pos:
        mov bl, numColores
        call Azar
        inc al                  ; 0..C-1 -> 1..C
        cmp permiteRep, 1       ; Normal/Dificil: cualquier color sirve
        je GS_Guardar
        xor si, si              ; Facil: buscar AL en secreto[0..DI-1]
GS_Busca:
        cmp si, di              ; ya revise todos y no estaba
        je GS_Guardar
        cmp secreto[si], al     ; repetido: sacar otro color
        je GS_Pos
        inc si
        jmp GS_Busca
GS_Guardar:
        mov secreto[di], al
        inc di
        mov cl, numPos          ; CX = numPos (byte -> word para comparar con DI)
        xor ch, ch
        cmp di, cx
        jb GS_Pos               ; faltan posiciones
        pop di
        pop si
        pop cx
        pop bx
        pop ax
        ret
GenerarSecreto ENDP

;EvaluarIntento
;Proposito: contar toros y vacas, cada ficha cuenta una sola vez (RF-08)
;Recibe: secreto[], intento[], numPos, numColores
;Devuelve: AL = toros, AH = vacas
;Destruye: nada (AX es lo que devuelve)

EvaluarIntento PROC NEAR
        push bx
        push cx
        push dx
        push si
        xor bx, bx              ; poner en cero los 9 contadores
        mov cx, 9
EI_Cero:
        mov cuentaS[bx], 0
        mov cuentaI[bx], 0
        inc bx
        loop EI_Cero

        xor dx, dx              ; DL = toros, DH = total
        xor si, si
        mov cl, numPos
        xor ch, ch
EI_Pos:                         ; recorrer posiciones
        mov al, secreto[si]
        cmp al, intento[si]     ; mismo color en el mismo lugar = toro
        jne EI_Contar
        inc dl
EI_Contar:
        mov bl, secreto[si]     ; el color se usa como indice
        inc cuentaS[bx]
        mov bl, intento[si]
        inc cuentaI[bx]
        inc si
        loop EI_Pos

        mov bx, 1               ; colores 1..numColores
        mov cl, numColores
EI_Color:
        mov al, cuentaS[bx]     ; AL = min(cuentaS[c], cuentaI[c])
        cmp al, cuentaI[bx]
        jbe EI_Min
        mov al, cuentaI[bx]
EI_Min:
        add dh, al              ; total += min
        inc bx
        loop EI_Color

        sub dh, dl              ; vacas = total - toros
        mov al, dl
        mov ah, dh
        pop si
        pop dx
        pop cx
        pop bx
        ret
EvaluarIntento ENDP
