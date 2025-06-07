# Makefile Definitivo y Corregido

# --- Compilador y Banderas ---
CXX = g++
CXXFLAGS = -Iinclude -std=c++17 -Wall -Wextra -g
LDFLAGS = -pthread

# --- Archivos y Directorios ---
TARGET = restaurante_sim
OBJ_DIR = obj
BIN_DIR = bin

# Lista explícita de todos los archivos de objeto necesarios
OBJECTS = \
	$(OBJ_DIR)/Bebida.o \
	$(OBJ_DIR)/Cajero.o \
	$(OBJ_DIR)/Cliente.o \
	$(OBJ_DIR)/Cocinero.o \
	$(OBJ_DIR)/Electrodomestico.o \
	$(OBJ_DIR)/Mesa.o \
	$(OBJ_DIR)/Mesero.o \
	$(OBJ_DIR)/Pedido.o \
	$(OBJ_DIR)/Platillo.o \
	$(OBJ_DIR)/Restaurante.o \
	$(OBJ_DIR)/Trabajador.o \
	$(OBJ_DIR)/main.o

# --- Reglas de Compilación ---

all: $(BIN_DIR)/$(TARGET)

# Regla para enlazar TODOS los archivos objeto y crear el ejecutable en bin/
$(BIN_DIR)/$(TARGET): $(OBJECTS) | $(BIN_DIR)
	@echo "Enlazando para crear el ejecutable: $@"
	$(CXX) $^ -o $@ $(LDFLAGS)
	@echo "¡Compilación exitosa! Ejecutable creado en la carpeta $(BIN_DIR)"

# Regla para crear los directorios si no existen
$(OBJ_DIR) $(BIN_DIR):
	@mkdir -p $@

# Regla de compilación genérica para archivos en src/
$(OBJ_DIR)/%.o: src/%.cpp | $(OBJ_DIR)
	@echo "Compilando $< -> $@"
	$(CXX) $(CXXFLAGS) -c $< -o $@

# CORRECCIÓN: Regla explícita para compilar main.cpp
$(OBJ_DIR)/main.o: main.cpp | $(OBJ_DIR)
	@echo "Compilando $< -> $@"
	$(CXX) $(CXXFLAGS) -c $< -o $@

# --- Reglas de Utilidad ---
run: all
	./$(BIN_DIR)/$(TARGET)

clean:
	@echo "Limpiando archivos generados..."
	rm -rf $(OBJ_DIR) $(BIN_DIR)
	@echo "Limpieza completa."

.PHONY: all clean run