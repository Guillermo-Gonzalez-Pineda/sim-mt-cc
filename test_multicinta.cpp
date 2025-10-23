#include <iostream>
#include "MultiTuringMachine.hpp"
#include "MultiTape.hpp"
#include "Parser.hpp"

int main() {
    std::cout << "=== Test de Máquina de Turing Multicinta ===\n";
    
    // Test 1: Crear máquina multicinta básica
    std::cout << "Test 1: Creando máquina multicinta básica...\n";
    try {
        MultiTuringMachine machine(2, '.');
        machine.add_state("q0");
        machine.add_state("q1");
        machine.set_initial_state("q0");
        machine.add_accept_state("q1");
        machine.add_input_symbol('1');
        machine.add_tape_symbol('1');
        
        std::cout << machine.get_info() << std::endl;
        std::cout << "✓ Test 1 pasado\n\n";
    } catch (const std::exception& e) {
        std::cout << "✗ Test 1 falló: " << e.what() << "\n\n";
    }
    
    // Test 2: Cargar desde archivo
    std::cout << "Test 2: Cargando máquina multicinta desde archivo...\n";
    try {
        MultiTuringMachine machine(1);
        if (Parser::load_multi_from_file("data/suma_multicinta.txt", machine)) {
            std::cout << machine.get_info() << std::endl;
            std::cout << "✓ Test 2 pasado\n\n";
        } else {
            std::cout << "✗ Test 2 falló: " << Parser::get_last_error() << "\n\n";
        }
    } catch (const std::exception& e) {
        std::cout << "✗ Test 2 falló: " << e.what() << "\n\n";
    }
    
    // Test 3: Test de MultiTape
    std::cout << "Test 3: Probando MultiTape...\n";
    try {
        MultiTape tapes(2, "110");
        std::cout << "Cintas iniciales:\n" << tapes.to_string() << std::endl;
        
        // Leer de ambas cintas
        std::cout << "Cinta 0 lee: '" << tapes.read(0) << "'\n";
        std::cout << "Cinta 1 lee: '" << tapes.read(1) << "'\n";
        
        // Escribir en cinta 1
        tapes.write(1, 'X');
        std::cout << "Después de escribir X en cinta 1:\n" << tapes.to_string() << std::endl;
        
        std::cout << "✓ Test 3 pasado\n\n";
    } catch (const std::exception& e) {
        std::cout << "✗ Test 3 falló: " << e.what() << "\n\n";
    }
    
    return 0;
}