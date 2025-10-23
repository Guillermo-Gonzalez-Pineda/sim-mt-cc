#!/bin/bash
# Script de pruebas automatizadas para el simulador de Máquinas de Turing
# Ejecuta todas las máquinas con sus conjuntos de pruebas y casos límite

set -e

# Colores para salida
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

PASSED=0
FAILED=0
TOTAL=0

# Función para imprimir encabezados
print_header() {
    echo -e "\n${BLUE}========================================${NC}"
    echo -e "${BLUE}$1${NC}"
    echo -e "${BLUE}========================================${NC}"
}

# Función para ejecutar un test
run_test() {
    local test_name="$1"
    local machine="$2"
    local input="$3"
    local expected="$4"
    local extra_flags="${5:-}"
    
    TOTAL=$((TOTAL + 1))
    
    result=$(echo "$input" | ./build/mt-sim "$machine" $extra_flags 2>/dev/null | head -1)
    
    if [ "$result" = "$expected" ]; then
        echo -e "${GREEN}✓${NC} $test_name: $expected (input: '$input')"
        PASSED=$((PASSED + 1))
    else
        echo -e "${RED}✗${NC} $test_name: esperado '$expected', obtenido '$result' (input: '$input')"
        FAILED=$((FAILED + 1))
    fi
}

# Función para ejecutar tests con archivo de palabras
run_test_file() {
    local test_name="$1"
    local machine="$2"
    local words_file="$3"
    local expected_pattern="$4"
    
    TOTAL=$((TOTAL + 1))
    
    result=$(./build/mt-sim "$machine" --words "$words_file" 2>/dev/null | head -5 | grep -c "$expected_pattern" || echo "0")
    
    if [ "$result" -gt 0 ]; then
        echo -e "${GREEN}✓${NC} $test_name: encontró $result ocurrencias de '$expected_pattern'"
        PASSED=$((PASSED + 1))
    else
        echo -e "${RED}✗${NC} $test_name: no se encontró el patrón esperado '$expected_pattern'"
        FAILED=$((FAILED + 1))
    fi
}

# Compilar el proyecto
print_header "COMPILACIÓN"
make clean > /dev/null 2>&1
make > /dev/null 2>&1
echo -e "${GREEN}✓${NC} Proyecto compilado correctamente"

# ============================================================
# TESTS DE MÁQUINAS MONOCINTA
# ============================================================

print_header "TESTS: cadenas_impar_ceros.txt (Monocinta)"
run_test "Test 1: '0' (1 cero - impar)" "data/cadenas_impar_ceros.txt" "0" "ACCEPT"
run_test "Test 2: '00' (2 ceros - par)" "data/cadenas_impar_ceros.txt" "00" "REJECT"
run_test "Test 3: '000' (3 ceros - impar)" "data/cadenas_impar_ceros.txt" "000" "ACCEPT"
run_test "Test 4: '101' (1 cero - impar)" "data/cadenas_impar_ceros.txt" "101" "ACCEPT"
run_test "Test 5: '1001' (2 ceros - par)" "data/cadenas_impar_ceros.txt" "1001" "REJECT"
run_test "Test 6: '10101' (2 ceros - par)" "data/cadenas_impar_ceros.txt" "10101" "REJECT"

print_header "TESTS: a_n_b_n.txt (Monocinta)"
run_test "Test 1: 'ab'" "data/a_n_b_n.txt" "ab" "ACCEPT"
run_test "Test 2: 'aabb'" "data/a_n_b_n.txt" "aabb" "ACCEPT"
run_test "Test 3: 'aaabbb'" "data/a_n_b_n.txt" "aaabbb" "ACCEPT"
run_test "Test 4: 'aab' (desbalanceado)" "data/a_n_b_n.txt" "aab" "REJECT"
run_test "Test 5: 'abb' (desbalanceado)" "data/a_n_b_n.txt" "abb" "REJECT"
run_test "Test 6: 'a' (solo a)" "data/a_n_b_n.txt" "a" "REJECT"

print_header "TESTS: acepta_todo.txt (Monocinta)"
run_test "Test 1: 'abc'" "data/acepta_todo.txt" "abc" "ACCEPT"
run_test "Test 2: 'a'" "data/acepta_todo.txt" "a" "ACCEPT"
run_test "Test 3: '' (epsilon)" "data/acepta_todo.txt" "" "ACCEPT"

# ============================================================
# TESTS DE MÁQUINAS MULTICINTA
# ============================================================

print_header "TESTS: anbn_multicinta.txt (Multicinta 2 cintas)"
run_test "Test 1: 'ab'" "data/anbn_multicinta.txt" "ab" "ACCEPT"
run_test "Test 2: 'aabb'" "data/anbn_multicinta.txt" "aabb" "ACCEPT"
run_test "Test 3: 'aaabbb'" "data/anbn_multicinta.txt" "aaabbb" "ACCEPT"
run_test "Test 4: 'aab' (desbalanceado)" "data/anbn_multicinta.txt" "aab" "REJECT"

print_header "TESTS: suma_multicinta.txt (Multicinta 2 cintas)"
run_test "Test 1: '110111' (2+3=5)" "data/suma_multicinta.txt" "110111" "ACCEPT"
run_test "Test 2: '1110111' (3+3=6)" "data/suma_multicinta.txt" "1110111" "ACCEPT"
run_test "Test 3: '101' (1+1=2)" "data/suma_multicinta.txt" "101" "ACCEPT"

print_header "TESTS: copia_multicinta.txt (Multicinta 2 cintas)"
run_test "Test 1: 'ab'" "data/copia_multicinta.txt" "ab" "ACCEPT"
run_test "Test 2: 'aabb'" "data/copia_multicinta.txt" "aabb" "ACCEPT"
run_test "Test 3: 'aba'" "data/copia_multicinta.txt" "aba" "ACCEPT"

# ============================================================
# TESTS DE CASOS LÍMITE
# ============================================================

print_header "TESTS: Casos Límite - Palabra Vacía"
run_test "Epsilon en impar_ceros" "data/cadenas_impar_ceros.txt" "" "REJECT"
run_test "Epsilon en a_n_b_n" "data/a_n_b_n.txt" "" "REJECT"
run_test "Epsilon en acepta_todo" "data/acepta_todo.txt" "" "ACCEPT"

print_header "TESTS: Casos Límite - Símbolos Fuera del Alfabeto"
TOTAL=$((TOTAL + 1))
result=$(echo "102" | ./build/mt-sim data/cadenas_impar_ceros.txt --strict 2>&1 | grep -c "fuera del alfabeto" || echo "0")
if [ "$result" -gt 0 ]; then
    echo -e "${GREEN}✓${NC} Modo --strict detecta símbolos inválidos"
    PASSED=$((PASSED + 1))
else
    echo -e "${RED}✗${NC} Modo --strict NO detecta símbolos inválidos"
    FAILED=$((FAILED + 1))
fi

TOTAL=$((TOTAL + 1))
result=$(echo "102" | ./build/mt-sim data/cadenas_impar_ceros.txt 2>/dev/null | head -1)
if [ "$result" = "REJECT" ]; then
    echo -e "${GREEN}✓${NC} Modo no estricto rechaza símbolos inválidos"
    PASSED=$((PASSED + 1))
else
    echo -e "${RED}✗${NC} Modo no estricto maneja mal símbolos inválidos"
    FAILED=$((FAILED + 1))
fi

print_header "TESTS: Casos Límite - Bucle Infinito"
TOTAL=$((TOTAL + 1))
result=$(echo "aaa" | ./build/mt-sim data/bucle_infinito.txt --max-steps 50 2>/dev/null | head -1)
if [ "$result" = "INFINITE" ]; then
    echo -e "${GREEN}✓${NC} Detección de bucle infinito funciona"
    PASSED=$((PASSED + 1))
else
    echo -e "${RED}✗${NC} Detección de bucle infinito falla (obtenido: '$result')"
    FAILED=$((FAILED + 1))
fi

print_header "TESTS: Casos Límite - Máquinas Malformadas"
TOTAL=$((TOTAL + 1))
result=$(./build/mt-sim data/malformado1.txt --info 2>&1 | grep -c "Error" || echo "0")
if [ "$result" -gt 0 ]; then
    echo -e "${GREEN}✓${NC} Detecta máquina malformada 1"
    PASSED=$((PASSED + 1))
else
    echo -e "${RED}✗${NC} No detecta máquina malformada 1"
    FAILED=$((FAILED + 1))
fi

TOTAL=$((TOTAL + 1))
result=$(./build/mt-sim data/malformado2.txt --info 2>&1 | grep -c "Error" || echo "0")
if [ "$result" -gt 0 ]; then
    echo -e "${GREEN}✓${NC} Detecta máquina malformada 2"
    PASSED=$((PASSED + 1))
else
    echo -e "${RED}✗${NC} No detecta máquina malformada 2"
    FAILED=$((FAILED + 1))
fi

# ============================================================
# TESTS CON ARCHIVOS DE PALABRAS
# ============================================================

print_header "TESTS: Archivos de Palabras Completos"
run_test_file "palabras_impar_ceros.txt" "data/cadenas_impar_ceros.txt" "tests/palabras_impar_ceros.txt" "ACCEPT"
run_test_file "palabras_anbn.txt (monocinta)" "data/a_n_b_n.txt" "tests/palabras_anbn.txt" "ACCEPT"
run_test_file "palabras_anbn.txt (multicinta)" "data/anbn_multicinta.txt" "tests/palabras_anbn.txt" "ACCEPT"

# ============================================================
# RESUMEN FINAL
# ============================================================

print_header "RESUMEN DE PRUEBAS"
echo -e "Total de pruebas: $TOTAL"
echo -e "${GREEN}Pasadas: $PASSED${NC}"
echo -e "${RED}Fallidas: $FAILED${NC}"

if [ $FAILED -eq 0 ]; then
    echo -e "\n${GREEN}✓✓✓ TODAS LAS PRUEBAS PASARON ✓✓✓${NC}\n"
    exit 0
else
    echo -e "\n${YELLOW}⚠ Algunas pruebas fallaron ⚠${NC}\n"
    exit 1
fi
