:: Pfad und Maschinentyp anpassen!
call "c:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" x86

cl -EHsc -c UniSocket.cpp
cl -EHsc -c SocketWrapperWin.cpp

cl -c server.cpp -EHsc
link -out:serverWin.exe server.obj ws2_32.lib UniSocket.obj SocketWrapperWin.obj

cl -c client.cpp -EHsc
link -out:clientWin.exe client.obj ws2_32.lib UniSocket.obj SocketWrapperWin.obj

REM .\serverWin.exe 61000
REM .\clientWin.exe 127.0.0.1 61000 "Hallo Welt"
