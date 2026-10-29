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


;DibujarSprite
;Proposito: copiar un sprite a la pantalla saltando el color 0 
;REcibe: pila (en este orden) x,y, ancho, alto, ptrSprite
;  ES = 0A000h
; Devuelve: nada 
; Destruye: nada


DibujarSprite PROC NEAR
        push bp
        mov bp, sp  ; BP = Ancla para leer los parametros
        push ax
        push bx
        push cx
        push dx
        push si
        push di
        
        cld

        mov ax, [bp+10] ; y
        mov bx, ANCHO_PANT
        mul bx          ; AX = y* 320 (mul tambien pisa dx)
        add ax, [bp+12] ; + x
        mov di, ax      ; DI = esquina superior izquierda en vram
        mov si, [bp+4]  ; SI = primer byte del sprite
        mov dx, [bp+6]  ; DX = filas que faltan

DS_Fila:
        mov cx, [bp+8]  ; CX = pixeles de esta fila
        push di         ; recordar donde empieza fila
DS_Pix:
        lodsb           ; AL = color de pixel, SI++
        or al, al       ; color 0 = transparente, no se pinta
        jz DS_Salta
        mov es:[di], al
DS_Salta:
        inc di
        loop DS_pix

        pop di          ; volver al inicio de la fila
        add di, ANCHO_PANT  ; bajar una fila en pantalla
        dec dx
        jnz DS_Fila

        pop di
        pop si
        pop dx
        pop cx
        pop bx
        pop ax
        pop bp
        ret 10          ; 5 parametros X 2 bytes

DibujarSprite ENDP
