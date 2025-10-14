# Versão mínima do CMake.
cmake_minimum_required(VERSION 3.10)

# Definição do projeto.
project(arandu VERSION 0.1.0 LANGUAGES C)

# Padrão da linguagem C.
set(CMAKE_C_COMPILER gcc)
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Delega a construção para os subdiretórios.
add_subdirectory(src)
add_subdirectory(examples)

# Seção de Instalação.
include(GNUInstallDirs)

# Instala os cabeçalhos.
install(
    DIRECTORY include/arandu
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Instala a biblioteca.
install(
    TARGETS arandu
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
