@echo off
echo Building Shadow Engine...
cd build
cmake --build . --config Debug
if %ERRORLEVEL% NEQ 0 (
    echo Build failed!
    pause
    exit /b 1
)

echo.
echo Copying shader files...
mkdir windows\x64\Debug\bin\shaders 2>nul
copy ..\shaders\*.* windows\x64\Debug\bin\shaders\ >nul

echo.
echo Running Shadow Engine...
windows\x64\Debug\bin\ShadowEngine.exe
pause 