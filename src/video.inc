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