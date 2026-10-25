;Iniciar Video
;Proposito: guardar el modo de video del Dos y entrar en modo 13h
;Recibe: nada
;devuelve: ES = 0A000h (segmento de la memoria de video)
;destruye: nada


IniciarVideo PROC NEAR
        push ax
        push bx
        mov ah, 0Fh ; preguntar a la bios el modo actual
        int 10h
        mov modoOriginal, al ; guardarlo para dejarlo igual al salir
        mov ax, 0013h ; AH = 00h fijar modo, AL = 13h 320x200x256
        int 10h
        mov ax, 0A000h ; de aqui en adelante ES apunta a la vram
        mov es, ax
        pop bx
        pop ax
        ret
IniciarVideo ENDP


; 
;RestaurarVideo
;Proposito: volver al modo de video que tenia el DOS 
;Recibe: modoOriginal (variable)
;Devuelve: nada
;Destruye: nada
;

RestaurarVideo PROC NEAR 
        push ax
        mov ah,00h
        mov al, modoOriginal
        int 10h
        pop ax
        ret
RestaurarVideo ENDP


;CargarPaleta
;Proposito: redefinir colores del DAC del VGA (RG-07)
;Recibe: DS:SI = tabla de bytes R, G, B (0..63)
;        AL = primer indice a redefinir
;        CX = cantidad de colores
; Devuelve: nada
; Destruye: nada
;

CargarPaleta PROC NEAR
        push ax
        push cx
        push dx
        push si
        cld
        mov dx, 3C8h  ; indicarle al DAC desde que indice empezar
        out dx, al 
        mov dx, 3C9h  ; 3C9h: puerto de datos del DAC
        mov ax, cx    ; cada color son 3 bytes -> CX  = CX*3
        shl cx, 1
        add cx, ax
CP_Sig: lodsb   ; siguiente componente R, G o B

        out  dx, al  ;el DAC avanza solo al siguiente
        loop CP_Sig
        pop si
        pop dx
        pop cx
        pop ax
        ret
CargarPaleta ENDP