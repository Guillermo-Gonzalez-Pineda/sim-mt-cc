# Makefile para el proyecto Máquina de Turing
# Sigue las mejores prácticas de compilación con g++

# Configuración del compilador
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -O2
DEBUG_FLAGS = -g -DDEBUG
RELEASE_FLAGS = -DNDEBUG

# Directorios
SRC_DIR = src
BUILD_DIR = build
DATA_DIR = data
TESTS_DIR = tests

# Archivos fuente
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
TARGET = mt-sim

# Objetivo principal
all: $(BUILD_DIR)/$(TARGET)

# Crear ejecutable
$(BUILD_DIR)/$(TARGET): $(OBJECTS) | $(BUILD_DIR)
	$(CXX) $(OBJECTS) -o $@
	@echo "Ejecutable creado: $@"

# Compilar archivos objeto
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Crear directorio de build
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compilación en modo debug
debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: $(BUILD_DIR)/$(TARGET)

# Compilación en modo release
release: CXXFLAGS += $(RELEASE_FLAGS)
release: $(BUILD_DIR)/$(TARGET)

# Limpiar archivos generados
clean:
	rm -rf $(BUILD_DIR)
	@echo "Archivos de compilación eliminados"

# Mostrar información del proyecto
info:
	@echo "=== Información del Proyecto ==="
	@echo "Compilador: $(CXX)"
	@echo "Flags: $(CXXFLAGS)"
	@echo "Fuentes: $(SOURCES)"
	@echo "Ejecutable: $(BUILD_DIR)/$(TARGET)"
	@echo "Archivos de datos: $(wildcard $(DATA_DIR)/*.txt)"
	@echo "Archivos de pruebas: $(wildcard $(TESTS_DIR)/*.txt)"

# Ejecutar pruebas básicas
test: $(BUILD_DIR)/$(TARGET)
	@echo "=== Ejecutando pruebas básicas ==="
	@echo "Probando máquina que acepta cadenas con número impar de 0s:"
	@echo "10101" | ./$(BUILD_DIR)/$(TARGET) $(DATA_DIR)/cadenas_impar_ceros.txt
	@echo ""
	@echo "Probando con archivo de palabras:"
	./$(BUILD_DIR)/$(TARGET) $(DATA_DIR)/cadenas_impar_ceros.txt --words $(TESTS_DIR)/palabras_impar_ceros.txt | head -10

# Ejecutar prueba con traza
test-trace: $(BUILD_DIR)/$(TARGET)
	@echo "=== Prueba con traza habilitada ==="
	@echo "101" | ./$(BUILD_DIR)/$(TARGET) $(DATA_DIR)/cadenas_impar_ceros.txt --trace

# Mostrar información de una máquina
show-info: $(BUILD_DIR)/$(TARGET)
	@echo "=== Información de la máquina de ejemplo ==="
	./$(BUILD_DIR)/$(TARGET) $(DATA_DIR)/cadenas_impar_ceros.txt --info

# Instalar (copiar el ejecutable a un directorio del sistema)
install: $(BUILD_DIR)/$(TARGET)
	@if [ -w /usr/local/bin ]; then \
		cp $(BUILD_DIR)/$(TARGET) /usr/local/bin/; \
		echo "Ejecutable instalado en /usr/local/bin/$(TARGET)"; \
	else \
		echo "No se tienen permisos para instalar en /usr/local/bin"; \
		echo "Ejecutar: sudo make install"; \
	fi

# Desinstalar
uninstall:
	@if [ -f /usr/local/bin/$(TARGET) ]; then \
		rm /usr/local/bin/$(TARGET); \
		echo "Ejecutable desinstalado de /usr/local/bin"; \
	else \
		echo "El ejecutable no está instalado en /usr/local/bin"; \
	fi

# Crear archivo tar con el proyecto
dist: clean
	@echo "Creando distribución..."
	tar -czf mt-sim.tar.gz --exclude='*.tar.gz' --exclude='.git*' *
	@echo "Distribución creada: mt-sim.tar.gz"

# Objetivos que no corresponden a archivos
.PHONY: all clean debug release info test test-trace show-info install uninstall dist

# Mostrar ayuda
help:
	@echo "Objetivos disponibles:"
	@echo "  all        - Compilar el proyecto"
	@echo "  debug      - Compilar en modo debug"
	@echo "  release    - Compilar en modo release"
	@echo "  clean      - Limpiar archivos generados"
	@echo "  info       - Mostrar información del proyecto"
	@echo "  test       - Ejecutar pruebas básicas"
	@echo "  test-trace - Ejecutar prueba con traza"
	@echo "  show-info  - Mostrar información de máquina de ejemplo"
	@echo "  install    - Instalar ejecutable en el sistema"
	@echo "  uninstall  - Desinstalar ejecutable del sistema"
	@echo "  dist       - Crear distribución tar.gz"
	@echo "  help       - Mostrar esta ayuda"