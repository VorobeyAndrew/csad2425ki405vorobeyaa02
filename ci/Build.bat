@echo off

set CLIENT_PATH="%~dp0..\src\Client"
set SERVER_PATH="%~dp0..\src\Server\Server.ino"
set SERVER_PORT="COM6"
set BUILD_DIR="%~dp0..\build"
set IS_LOCAL=false

:: Check if Arduino is connected to the COM port (in this case COM6)
echo [INFO] Checking if Arduino is connected to port %SERVER_PORT%...

for /f "tokens=*" %%i in ('arduino-cli.exe board list') do (
    echo %%i | find /i "%SERVER_PORT%" >nul
    if %errorlevel% equ 0 (
        echo [INFO] Arduino detected on %SERVER_PORT%.
        set IS_LOCAL=true
    )
)

if "%IS_LOCAL%"=="true" (
    echo [INFO] Running locally. Arduino upload will be performed.
) else (
    echo [INFO] Not running locally or Arduino not detected. Skipping Arduino upload.
)

echo ===================================

:: Ensure Arduino CLI is available only if running locally
if "%IS_LOCAL%"=="true" (
    echo [INFO] Checking for Arduino CLI...
    if not exist arduino-cli.exe (
        echo [INFO] Arduino CLI not found. Downloading...
        curl -fsSL https://downloads.arduino.cc/arduino-cli/arduino-cli_latest_Windows_64bit.zip -o arduino-cli.zip
        if %errorlevel% neq 0 (
            echo [ERROR] Failed to download Arduino CLI.
            pause
            exit /b %errorlevel%
        )
        tar -xf arduino-cli.zip
        if %errorlevel% neq 0 (
            echo [ERROR] Failed to extract Arduino CLI.
            pause
            exit /b %errorlevel%
        )
        del arduino-cli.zip
        echo [INFO] Arduino CLI downloaded successfully.
    ) else (
        echo [INFO] Arduino CLI found.
    )
)

echo ===================================
echo [INFO] Compiling AutoDetectCOMPort code...
if not exist "..\src\AutoDetectCOMPort" (
    echo [ERROR] AutoDetectCOMPort .sln file not found: "..\src\AutoDetectCOMPort"
    pause
    exit /b 1
)

dotnet build ..\src\AutoDetectCOMPort\AutoDetectCOMPort.sln -c Release

if %errorlevel% neq 0 (
    echo [ERROR] Failed to compile client code.
    pause
    exit /b %errorlevel%
)
echo [INFO] AutoDetectCOMPort compiled successfully.

echo ===================================
echo [INFO] Launch AutoDetectCOMPort.exe.
"..\src\AutoDetectCOMPort\AutoDetectCOMPort\bin\Release\net8.0\AutoDetectCOMPort.exe"
IF not %ERRORLEVEL% EQU 0 (
    echo [ERROR] File AutoDetectCOMPort.exe failed executed.
    pause
    exit /b 1
)
echo [INFO] File AutoDetectCOMPort.exe executed successfully.

echo ===================================
echo [INFO] Compiling client code...
if not exist %CLIENT_PATH% (
    echo [ERROR] Client source file not found: %CLIENT_PATH%
    pause
    exit /b 1
)

rem Check if build directory exists, if not create it
if not exist %BUILD_DIR% (
    mkdir %BUILD_DIR%
    echo [INFO] Build directory created: %BUILD_DIR%
)

g++ -I"..\src\third_party\include" %CLIENT_PATH%\* -o %BUILD_DIR%\Client.exe -lole32
if %errorlevel% neq 0 (
    echo [ERROR] Failed to compile client code.
    pause
    exit /b %errorlevel%
)
echo [INFO] Client compiled successfully.

echo ===================================
echo [INFO] Compiling server code...
if not exist %SERVER_PATH% (
    echo [ERROR] Server source file not found: %SERVER_PATH%
    pause
    exit /b 1
)
arduino-cli.exe compile --fqbn arduino:avr:nano %SERVER_PATH%
if %errorlevel% neq 0 (
    echo [ERROR] Failed to compile server code.
    pause
    exit /b %errorlevel%
)
echo [INFO] Server code compiled successfully.

:: Only upload if running locally (if Arduino was detected)
if "%IS_LOCAL%"=="true" (
    echo ===================================
    echo [INFO] Uploading server code to Arduino...
    arduino-cli.exe upload -p %SERVER_PORT% --fqbn arduino:avr:nano %SERVER_PATH%
    if %errorlevel% neq 0 (
        echo [ERROR] Failed to upload server code to Arduino.
        pause
        exit /b %errorlevel%
    )
    echo [INFO] Server code uploaded successfully.
) else (
    echo [INFO] Skipping Arduino upload step.
)

echo ===================================
if "%IS_LOCAL%"=="true" (
    echo [INFO] Deleting Arduino CLI files...

    rem Remove the arduino-cli executable if it was downloaded
    if exist arduino-cli.exe (
        del arduino-cli.exe
        echo [INFO] arduino-cli.exe executable deleted.
    )

    rem Remove the LICENSE.txt if it was downloaded
    if exist LICENSE.txt (
        del LICENSE.txt
        echo [INFO] LICENSE.txt executable deleted.
    )
)

echo ===================================
echo [INFO] All tasks completed successfully.
pause
