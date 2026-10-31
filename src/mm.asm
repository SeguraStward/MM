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

;colores del juego
COL_TABLERO EQU 19         ; azul oscuro, fondo del tablero
COL_CASILLA EQU 20         ; gris, casilla vacia
COL_GANO    EQU 24         ; verde

;zonas de la pantalla de juego (maqueta)
TAB_X      EQU 4           ; zona B: tablero
TAB_Y      EQU 22
FILA_ALTO  EQU 14          ; 12 filas x 14 = 168, caben las de Facil
FICHA_X0   EQU TAB_X + 4
FICHA_PASO EQU 20          ; 16 de ficha + 4 de espacio
FEED_X     EQU FICHA_X0 + 5*FICHA_PASO + 4   ; iconos toro/vaca
FEED_PASO  EQU 18
INTENTO_X  EQU 208         ; zona C: intento en construccion
INTENTO_Y  EQU 52
DISP_X     EQU 208         ; zona C: fichas disponibles
DISP_Y     EQU 88
DISP_PASO  EQU 24
ESTADO_Y   EQU 190         ; zona D: barra de ayuda y avisos
SEC_X      EQU 110         ; secreto revelado en PantFin
SEC_Y      EQU 96





.DATA
    modoOriginal DB ?  ; modo de video del DOS
    txtTitulo DB 'MASTERMIND: DATA PIPELINE', 0

    estado    DB EST_TITULO       ; pantalla actual
    tablaPant DW PantTitulo, PantMenu, PantVacia, PantVacia
                 DW PantVacia, PantJuego, PantFin
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


    ;niveles: posiciones, colores, permite repetir (0/1), intentos
    niveles     DB 4, 6, 0, 12      ; Facil
                DB 4, 6, 1, 10      ; Normal
                DB 5, 8, 1, 10      ; Dificil
    nivelSel    DB 1                ; 0..2
    numPos      DB ?                ; se copian de la tabla al empezar
    numColores  DB ?
    permiteRep  DB ?
    maxIntentos DB ?

    semilla     DW ?
    secreto     DB 5 DUP(0)         ; colores 1..C, el 0 = vacio
    intento     DB 5 DUP(0)
    cuentaS     DB 9 DUP(0)         ; cuantas veces sale cada color en el secreto
    cuentaI     DB 9 DUP(0)         ; ... y en el intento

    ;partida
    historial   DB 12*5 DUP(0)      ; fila i, posicion j -> historial[i*5 + j]
    torosHist   DB 12 DUP(0)
    vacasHist   DB 12 DUP(0)
    intentoAct  DB 0                ; 0..maxIntentos-1
    posActiva   DB 0                ; posicion del intento que se edita
    resultado   DB 0                ; 1 gano, 0 perdio
    modoPrueba  DB 0                ; 1 = el secreto se escribe a mano
    avisoOn     DB 0                ; 1 = hay un aviso en la barra

    txtIntN     DB 'INTENTO 00/00', 0
    txtSuInt    DB 'SU INTENTO:', 0
    txtFichas   DB 'FICHAS:', 0
    txtDig1     DB '1  2  3  4', 0
    txtDig2     DB '5  6', 0
    txtDig3     DB '5  6  7  8', 0
    txtAyuda    DB 'FLECHAS mover/color  1-8  ENTER  ESC', 0
    txtAviso    DB 'INTENTO INCOMPLETO: llene todo', 0
    txtPrueba   DB 'MODO PRUEBA: arme el secreto', 0
    txtFacil    DB 'FACIL', 0
    txtNormal   DB 'NORMAL', 0
    txtDificil  DB 'DIFICIL', 0
    tablaNivTxt DW txtFacil, txtNormal, txtDificil
    txtGano     DB 'PIPELINE REPARADO!', 0
    txtPerdio   DB 'PIPELINE CAIDO', 0
    txtSecreto  DB 'SECRETO:', 0
    txtFinOpc   DB 'ENTER: otra partida   ESC: menu', 0
    txtModoOn   DB 'MODO PRUEBA', 0
    txtModoOff  DB '           ', 0


        INCLUDE paleta.inc
        INCLUDE sprites.inc

    ;puntero al sprite de cada color (color 1 = posicion 0)
    tablaFichas DW F_NUBE, F_ENGR, F_EMBUDO, F_FLECHA
                DW F_JSON, F_RAYO, F_BD, F_CSV


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
INCLUDE logica.inc
INCLUDE juego.inc

END INICIO