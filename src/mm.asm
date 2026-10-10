; =====================================================================
; MM.ASM - PRUEBA DE HUMO (Fase 0): programa act1 del Tema 5.
; Suma 4 + 8 e imprime 12. Se reemplaza en la Fase 1.
; =====================================================================
.MODEL SMALL
.STACK 100H
.DATA
    NUM1 DB 4
    NUM2 DB 8
    RESU DB 0
.CODE
MAIN PROC FAR
    MOV AX, @DATA
    MOV DS, AX
    ; Sumar NUM1 + NUM2
    MOV AL, NUM1
    ADD AL, NUM2
    MOV RESU, AL
    ; Convertir a ASCII (dividir entre 10)
    MOV AH, 0
    MOV AL, RESU
    MOV BL, 10
    DIV BL
    ; AL = decenas, AH = unidades
    ADD AL, 30H
    ADD AH, 30H
    MOV BH, AH          ; guardar unidades: INT 21h/02h devuelve AL = DL
    ; Imprimir decenas
    MOV DL, AL
    MOV AH, 02H
    INT 21H
    ; Imprimir unidades
    MOV DL, BH
    MOV AH, 02H
    INT 21H
    ; Salir al DOS
    MOV AH, 4CH
    INT 21H
MAIN ENDP
END MAIN
