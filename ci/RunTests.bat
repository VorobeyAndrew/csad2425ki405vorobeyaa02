@echo off

set RESULT_DIR="%~dp0..\TestResult"
echo ===================================
if not exist %RESULT_DIR% (
        mkdir %RESULT_DIR%
	echo [INFO] Directory for test results created: %RESULT_DIR%
        )

echo ===================================
    	echo [INFO] Compile client tests...
    	g++ -std=c++17 -mconsole -I".\src\third_party\include" -I".\src\Tests\googletestsrc" .\src\Tests\googletestsrc\src\gtest-all.cc .\src\Tests\googletestsrc\src\gmock-all.cc .\src\Tests\Client\ClientCommunicationTest.cpp .\src\Tests\Client\HelperUtilsTest.cpp .\src\Client\ClientCommunication.cpp .\src\Client\HelperUtils.cpp -o ClientTest.exe
    	if %errorlevel% neq 0 (
       	    echo [ERROR] Failed to compile client tests.
            pause
            exit /b %errorlevel%
        )
        echo [INFO] Client tests build succsesfully.

    	echo ===================================
    	echo [INFO] Run client tests...
    	.\ClientTest.exe > %RESULT_DIR%\ClientTestResult.txt
	type %RESULT_DIR%\ClientTestResult.txt
    	IF not %ERRORLEVEL% EQU 0 (
    	    echo [ERROR] Failed to execute ClientTest.exe.
    	    pause
    	    exit /b 1
	    )
	echo [INFO] ClientTest.exe executed successfully.

    	echo ===================================
    	echo [INFO] Compile server tests...
    	g++ -std=c++17 -mconsole -I".\src\third_party\include" -I".\src\Tests\googletestsrc" .\src\Tests\googletestsrc\src\gtest-all.cc .\src\Tests\Server\ServerLogicTest.cpp .\src\Server\ServerLogic.cpp -o ServerTest.exe
    	if %errorlevel% neq 0 (
       	    echo [ERROR] Failed to compile server tests.
            pause
            exit /b %errorlevel%
        )
        echo [INFO] Server tests build succsesfully.
    	echo ===================================
    	echo [INFO] Run server tests...
    	.\ServerTest.exe > %RESULT_DIR%\ServerTestResult.txt
	type %RESULT_DIR%\ServerTestResult.txt
    	IF not %ERRORLEVEL% EQU 0 (
    	    echo [ERROR] Failed to execute ServerTest.exe.
    	    pause
    	    exit /b 1
	    )
	echo [INFO] ServerTest.exe executed successfully.