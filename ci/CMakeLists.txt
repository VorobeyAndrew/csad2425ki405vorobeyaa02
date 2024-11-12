# Мінімальна версія CMake
cmake_minimum_required(VERSION 3.10)

# Назва проекту
project(Client)

# Встановлюємо стандарт C++
set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR}/../src/Client
${CMAKE_SOURCE_DIR}/../src/third_party/include)

# Додаємо виконуваний файл для клієнта
add_executable(Client ${CMAKE_SOURCE_DIR}/../src/Client/ClientCommunication.cpp 
    ${CMAKE_SOURCE_DIR}/../src/Client/main.cpp
    ${CMAKE_SOURCE_DIR}/../src/Client/HelperUtils.cpp)

# Змінні для Arduino
set(ARDUINO_CLI "arduino-cli")
set(ARDUINO_BOARD "arduino:avr:nano")
set(ARDUINO_PORT "COM6")
set(ARDUINO_SRC "../src/Server/Server.ino")

# Компіляція серверного коду для Arduino
add_custom_target(compile_server ALL
    COMMAND ${ARDUINO_CLI} compile --fqbn ${ARDUINO_BOARD} ${ARDUINO_SRC}
    COMMENT "Compiling Arduino server..."
)

# Додаємо залежність компіляції Arduino до клієнта
add_dependencies(Client compile_server)

# Очищення зібраних файлів
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${CMAKE_BINARY_DIR}/Client;${ARDUINO_SRC}")

# Додаємо C# проект
set(CSHARP_SOLUTION "${CMAKE_SOURCE_DIR}/../src/AutoDetectCOMPort/AutoDetectCOMPort.sln")

# Використовуємо dotnet build для збірки C# проекту
add_custom_target(AutoDetectCOMPort ALL
    COMMAND dotnet build ${CSHARP_SOLUTION} --configuration Release
    COMMENT "Building C# AutoDetectCOMPort project with dotnet..."
)

# Додаємо залежність C++ клієнта від збірки C# проекту
add_dependencies(Client AutoDetectCOMPort)