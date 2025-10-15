# Cria a biblioteca 'arandu'.
add_library(arandu
    arandu.c
    rng.c
)

# Define os diretórios de include para o alvo 'arandu'.
target_include_directories(arandu PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(arandu PUBLIC m)
