@echo off

set CLIENT_PATH="%~dp0..\src\Client"
set SERVER_PATH="%~dp0..\src\Server\Server.ino"
set SERVER_PORT="COM6"
set BUILD_DIR="%~dp0..\build"
set SOLUTION_PATH="%~dp0..\src\AutoDetectCOMPort\AutoDetectCOMPort.sln"
set CURRENT_DIR=%cd%
set IS_GITHUB_ACTION=%GITHUB_ACTIONS%

:: Перевірка на локальний чи CI запуск
if "%IS_GITHUB_ACTION%"=="true" (
    echo [INFO] Running in GitHub Actions environment.
    set IS_LOCAL=false
) else (
    echo [INFO] Running locally.
    set IS_LOCAL=true
)



:: Якщо локально, завантажуємо Arduino CLI, інакше пропускаємо цей крок
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
) else (
    echo [INFO] Skipping Arduino CLI download in GitHub Actions.
)

echo ===================================
if not exist "%SOLUTION_PATH%" (
    echo [ERROR] AutoDetectCOMPort .sln file not found: "%SOLUTION_PATH%"
    pause
    exit /b 1
)

dotnet build "%SOLUTION_PATH%" -c Release

if %errorlevel% neq 0 (
    echo [ERROR] Failed to compile client code.
    pause
    exit /b %errorlevel%
)
echo [INFO] AutoDetectCOMPort compiled successfully.

echo ===================================
if "%IS_LOCAL%"=="true" (
echo [INFO] Launching AutoDetectCOMPort.exe.
	"..\src\AutoDetectCOMPort\AutoDetectCOMPort\bin\Release\net8.0\AutoDetectCOMPort.exe"
	IF not %ERRORLEVEL% EQU 0 (
    	echo [ERROR] Failed to execute AutoDetectCOMPort.exe.
    	pause
    	exit /b 1
	)
	echo [INFO] AutoDetectCOMPort.exe executed successfully.
)
echo ===================================
echo [INFO] Compiling client code...
if not exist %CLIENT_PATH% (
    echo [ERROR] Client source file not found: %CLIENT_PATH%
    pause
    exit /b 1
)

rem Створення build директорії, якщо не існує
if not exist %BUILD_DIR% (
    mkdir %BUILD_DIR%
    echo [INFO] Build directory created: %BUILD_DIR%
)

g++ -I"%~dp0..\src\third_party\include" %CLIENT_PATH%\* -o %BUILD_DIR%\Client.exe -lole32
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
if "%IS_LOCAL%"=="true" (
	if exist "%~dp0..\doc\Doxyfile" 
	(
    		echo [INFO] Doxyfile found, generating documentation...
   		cd /d "%~dp0..\doc"
    		doxygen Doxyfile
		cd /d %CURRENT_DIR%
	) 
	else 
	(
    		echo [ERROR] Doxyfile not found, skipping documentation generation.
	)

)else (
    echo [INFO] Skipping generating documentation GitHub Actions.
)

:: Якщо локально, завантажуємо код на плату Arduino
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
    echo [INFO] Skipping Arduino upload in GitHub Actions.
)

echo ===================================
if "%IS_LOCAL%"=="true" (
    echo [INFO] Deleting Arduino CLI files...

    rem Видаляємо Arduino CLI якщо був завантажений
    if exist arduino-cli.exe (
        del arduino-cli.exe
        echo [INFO] arduino-cli.exe executable deleted.
    )

    rem Видаляємо LICENSE.txt якщо був завантажений
    if exist LICENSE.txt (
        del LICENSE.txt
        echo [INFO] LICENSE.txt deleted.
    )
)

echo ===================================
echo [INFO] All tasks completed successfully.
pause