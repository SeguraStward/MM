@echo off
REM Ensambla y enlaza Mastermind. Uso: escribir BUILD en la raiz del proyecto
tasm /zi /m2 /isrc src\mm.asm, bin\mm.obj
if errorlevel 1 goto error
tlink /v bin\mm.obj, bin\mm.exe
if errorlevel 1 goto error
echo === OK: bin\mm.exe ===
goto fin
:error
echo === HUBO ERRORES: revisa los mensajes de arriba ===
:fin
