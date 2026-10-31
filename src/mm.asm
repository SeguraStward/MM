;MM.ASM - Mastermind - EIF205 II Ciclo 2026
;Integrantes: Angel Segura, Cristhian Castillo
;Ensamblado: build.bat
;
INCLUDE macros.inc

.MODEL SMALL
.STACK 400h

ANCHO_PANT EQU 320 ; pixeles por fila en modo 13h

;colores del texto
COL_NORMAL EQU 17          ; blanco
COL_RESALT EQU 23          ; amarillo, opcion seleccionada
COL_TITULO EQU 22          ; naranja

;estados (el orden es el mismo que en tablaPant)
EST_TITULO   EQU 0
EST_MENU     EQU 1
EST_NIVEL    EQU 2
EST_INSTRUC  EQU 3
EST_CREDITOS EQU 4
EST_JUEGO    EQU 5
EST_FIN      EQU 6
EST_SALIR    EQU 7

N_OPC EQU 5                ; opciones del menu





.DATA
    modoOriginal DB ?  ; modo de video del DOS
    txtTitulo DB 'MASTERMIND: DATA PIPELINE', 0

    estado    DB EST_TITULO       ; pantalla actual
    tablaPant DW PantTitulo, PantMenu, PantVacia, PantVacia
                 DW PantVacia, PantVacia, PantVacia
    txtIntegr DB 'Angel Segura - Cristhian Fernandez', 0
    txtTecla  DB 'presione una tecla', 0
    txtPronto DB 'en construccion (ESC = menu)', 0

    ;menu  
    txtJugar    DB 'JUGAR', 0
    txtNivel    DB 'NIVEL', 0
    txtInstruc  DB 'INSTRUCCIONES', 0
    txtCreditos DB 'CREDITOS', 0
    txtSalir    DB 'SALIR', 0
    opcMenu   DW txtJugar, txtNivel, txtInstruc, txtCreditos, txtSalir
    destinos  DB EST_JUEGO, EST_NIVEL, EST_INSTRUC, EST_CREDITOS, EST_SALIR
    opcSel    DB 0                ; opcion resaltada (0..4)


        INCLUDE paleta.inc
        INCLUDE sprites.inc


.CODE
INICIO PROC FAR
        mov ax, @DATA 
        mov ds, ax
        call IniciarVideo
        
        mov si, OFFSET PALETA
        mov al, PAL_INICIO  
        mov cx, PAL_CANT
        call CargarPaleta


;maquina de estados: llama a la pantalla del estado actual
BUCLE:  cmp estado, EST_SALIR
        je TERMINAR
        mov bl, estado          ; BX = estado * 2 (cada entrada es un DW)
        xor bh, bh
        shl bx, 1
        call tablaPant[bx]      ; llamada indirecta a la pantalla
        jmp BUCLE
TERMINAR:


        call RestaurarVideo
        SALIR_DOS
INICIO ENDP

INCLUDE video.inc
INCLUDE teclado.inc
INCLUDE pantalla.inc

END INICIO