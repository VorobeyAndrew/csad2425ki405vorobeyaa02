@echo off

set CLIENT_PATH="%~dp0..\src\Client\main.cpp"
set SERVER_PATH="%~dp0..\src\Server\Server.ino"
set SERVER_PORT="COM6"
set BUILD_DIR="%~dp0..\build"

echo ===================================
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

g++ %CLIENT_PATH% -o %BUILD_DIR%\Client.exe
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

echo ===================================
echo [INFO] Uploading server code to Arduino...
arduino-cli.exe upload -p %SERVER_PORT% --fqbn arduino:avr:nano %SERVER_PATH%
if %errorlevel% neq 0 (
    echo [ERROR] Failed to upload server code to Arduino.
    pause
    exit /b %errorlevel%
)
echo [INFO] Server code uploaded successfully.

echo ===================================
echo [INFO] Deleting Arduino CLI files...

rem Remove the arduino-cli executable if it was downloaded
if exist arduino-cli.exe (
    del arduino-cli.exe
    echo [INFO] arduino-cli.exe executable deleted.
)

rem Remove the LICENSE.txt executable if it was downloaded
if exist LICENSE.txt (
    del LICENSE.txt
    echo [INFO] LICENSE.txt executable deleted.
)

echo ===================================
echo [INFO] All tasks completed successfully.
pause
