@echo off
set indent=c:\indent\bin\indent.exe -kr -ut -i8 -ppi 4
%indent% *.cpp
%indent% *.h 
