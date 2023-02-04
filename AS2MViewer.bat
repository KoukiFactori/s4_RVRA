@echo off

echo Configuration de l'environnement
set Path=C:\Qt\QtOnline\5.12.3\mingw73_64;%PATH%

echo Execution de AS2MViewer
.\release\AS2MViewer.exe ruinart_ 2

exit
