@echo off

echo Configuration de l'environnement
set Path=C:\Qt\5.12.3\mingw73_64\bin;%PATH%

echo Execution de AS2MViewer
AS2MViewer.exe ruinart_ 1

exit