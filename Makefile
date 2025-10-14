# Makefile "driver" para projetos C/C++ com CMake, sem custom commands no CMake.
# Fedora/Unix-like. Orquestra build, symlinks e testes de forma reprodutível.

# ========= Config =========
BUILD_DIR       ?= build
BUILD_TYPE      ?= Release        # use Debug para dev: `make debug`
GENERATOR       ?= $(shell command -v ninja >/dev/null 2>&1 && echo Ninja || echo "Unix Makefiles")

# Força pastas de saída previsíveis (sem alterar seu CMakeLists)
OUT_DIR         := $(abspath $(BUILD_DIR))/out
BIN_DIR         := $(OUT_DIR)/bin
LIB_DIR         := $(OUT_DIR)/lib
ARCHIVE_DIR     := $(OUT_DIR)/lib

# Flags úteis
CMAKE_COMMON_FLAGS := -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
                      -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) \
                      -DCMAKE_RUNTIME_OUTPUT_DIRECTORY=$(BIN_DIR) \
                      -DCMAKE_LIBRARY_OUTPUT_DIRECTORY=$(LIB_DIR) \
                      -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY=$(ARCHIVE_DIR)

# Paralelismo
JOBS ?= $(shell nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

# ========= Alvos =========
.PHONY: help configure reconfigure build all debug release test install-local \
        bin-symlink compile-commands clean distclean run format

help:
	@echo "alvos:"
	@echo "  make [all]        -> configura, compila, cria symlinks ./bin e ./compile_commands.json"
	@echo "  make debug        -> idem acima com BUILD_TYPE=Debug"
	@echo "  make release      -> idem acima com BUILD_TYPE=Release"
	@echo "  make test         -> roda ctest"
	@echo "  make install-local-> instala em ./local (útil p/ uso acadêmico sem sudo)"
	@echo "  make clean        -> limpeza do build (mantém build/)"
	@echo "  make distclean    -> remove build/, ./bin e compile_commands.json"
	@echo "  make run TARGET=<exe> -> executa binário de $(BIN_DIR)"
	@echo "  make format       -> roda clang-format (se disponível)"

all: build bin-symlink compile-commands

debug:
	@$(MAKE) BUILD_TYPE=Debug all

release:
	@$(MAKE) BUILD_TYPE=Release all

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

configure: $(BUILD_DIR)
	@echo "==> CMake configure ($(BUILD_TYPE))"
	@cmake -S . -B $(BUILD_DIR) -G "$(GENERATOR)" $(CMAKE_COMMON_FLAGS)

reconfigure:
	@echo "==> Reconfigurando..."
	@cmake -S . -B $(BUILD_DIR) $(CMAKE_COMMON_FLAGS)

build: configure
	@echo "==> Build"
	@cmake --build $(BUILD_DIR) -- -j$(JOBS)

test:
	@echo "==> ctest"
	@ctest --test-dir $(BUILD_DIR) --output-on-failure

install-local: build
	@echo "==> Install em ./local"
	@cmake --install $(BUILD_DIR) --prefix "$(abspath ./local)"

bin-symlink:
	@echo "==> Symlink ./bin -> $(BIN_DIR)"
	@mkdir -p $(BIN_DIR)
	@ln -sfn $(BIN_DIR) bin

compile-commands:
	@echo "==> Symlink ./compile_commands.json"
	@ln -sfn $(BUILD_DIR)/compile_commands.json compile_commands.json

run:
	@if [ -z "$(TARGET)" ]; then \
	  echo "Uso: make run TARGET=<nome_do_executavel>"; exit 1; \
	fi
	@"$(BIN_DIR)/$(TARGET)"

clean:
	@echo "==> Clean"
	@cmake --build $(BUILD_DIR) --target clean || true

distclean:
	@echo "==> Distclean"
	@rm -rf "$(BUILD_DIR)"
	@rm -f compile_commands.json
	@rm -f bin

format:
	@command -v clang-format >/dev/null 2>&1 || { echo "clang-format não encontrado (sudo dnf install clang-tools-extra)"; exit 0; }
	@echo "==> clang-format em include/ e src/"
	@find include src -type f \( -name "*.c" -o -name "*.h" -o -name "*.cpp" -o -name "*.hpp" \) -print0 | xargs -0 clang-format -i

