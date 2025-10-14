# Cria o executável de exemplo.
add_executable(arandu_version arandu_version.c)
add_executable(arandu_rng arandu_rng.c)

# Linka o executável com a biblioteca 'arandu'.
target_link_libraries(arandu_version PRIVATE arandu)
target_link_libraries(arandu_rng PRIVATE arandu)
