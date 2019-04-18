@echo off
if "%1" == "/clear" goto :doit
goto :skip
:doit
erase docs\*.* /Q /S
:skip
jsdoc -t templates\default -d docs -r src -c myconf.json