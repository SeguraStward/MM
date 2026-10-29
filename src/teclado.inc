LeerTecla PROC NEAR
        mov  ah, 00h
        int  16h
        cmp  al, 0E0h
        jne  LT_Fin
        xor  al, al
LT_Fin: ret
LeerTecla ENDP

HayTecla PROC NEAR
        push ax
        mov  ah, 01h
        int  16h
        pop  ax
        ret
HayTecla ENDP