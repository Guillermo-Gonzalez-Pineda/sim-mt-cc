#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>
#include <memory>

#include "TuringMachine.hpp"
#include "MultiTuringMachine.hpp"
#include "Parser.hpp"
#include "Simulator.hpp"

/**
 * @brief Elimina espacios en blanco al inicio y final de una cadena
 * @param s Cadena a procesar
 * @return Cadena sin espacios
 */
static std::string strip_spaces(const std::string& s) {
  std::string t;
  for (char c : s) {
    if (!std::isspace(static_cast<unsigned char>(c))) {
      t.push_back(c);
    }
  }
  return t;
}

/**
 * @brief Verifica si una palabra contiene solo símbolos válidos del alfabeto
 * @param w Palabra a verificar
 * @param machine Máquina de Turing con el alfabeto
 * @param bad Puntero para almacenar el símbolo inválido (opcional)
 * @return true si la palabra es válida
 */
static bool word_in_alphabet(const std::string& w, const TuringMachine& machine,
                             std::string* bad = nullptr) {
  for (char c : w) {
    if (!machine.is_input_symbol(c)) {
      if (bad) {
        *bad = std::string(1, c);
      }
      return false;
    }
  }
  return true;
}

/**
 * @brief Verifica si una palabra contiene solo símbolos válidos del alfabeto (multicinta)
 * @param w Palabra a verificar
 * @param machine Máquina de Turing multicinta con el alfabeto
 * @param bad Puntero para almacenar el símbolo inválido (opcional)
 * @return true si la palabra es válida
 */
static bool word_in_alphabet_multi(const std::string& w, const MultiTuringMachine& machine,
                                   std::string* bad = nullptr) {
  for (char c : w) {
    if (!machine.is_input_symbol(c)) {
      if (bad) {
        *bad = std::string(1, c);
      }
      return false;
    }
  }
  return true;
}

/**
 * @brief Muestra el mensaje de ayuda
 * @param program_name Nombre del programa
 */
static void show_help(const char* program_name) {
  std::cout << "Uso: " << program_name << " <fichero_maquina> [opciones]\n"
            << "Opciones:\n"
            << "  --trace              Muestra traza paso a paso\n"
            << "  --words <fichero>    Lee palabras de un fichero (una por línea)\n"
            << "  --strict             Error si la palabra contiene símbolos fuera del alfabeto\n"
            << "  --max-steps <N>      Límite de pasos de la simulación (0 = sin límite)\n"
            << "  --info               Muestra información de la máquina y termina\n"
            << "  --help               Muestra esta ayuda\n\n"
            << "Si no se especifica --words, lee palabras desde la entrada estándar.\n"
            << "Una línea vacía representa la palabra vacía (épsilon).\n";
}

int main(int argc, char** argv) {
  if (argc < 2) {
    show_help(argv[0]);
    return 1;
  }
  
  std::string machine_path = argv[1];

  // Opciones de línea de comandos
  bool trace = false;
  bool strict_mode = false;
  bool show_info = false;
  std::optional<std::string> words_path;
  size_t max_steps = 1000;  // Por defecto, límite de 1000 pasos

  // Parseo de opciones
  for (int i = 2; i < argc; ++i) {
    std::string arg = argv[i];
    
    if (arg == "--trace") {
      trace = true;
    } else if (arg == "--strict") {
      strict_mode = true;
    } else if (arg == "--info") {
      show_info = true;
    } else if (arg == "--help") {
      show_help(argv[0]);
      return 0;
    } else if (arg == "--words") {
      if (i + 1 >= argc) {
        std::cerr << "[Error] Falta ruta después de --words\n";
        return 1;
      }
      words_path = argv[++i];
    } else if (arg == "--max-steps") {
      if (i + 1 >= argc) {
        std::cerr << "[Error] Falta N después de --max-steps\n";
        return 1;
      }
      try {
        long long v = std::stoll(argv[++i]);
        if (v < 0) {
          throw std::invalid_argument("negativo");
        }
        max_steps = static_cast<size_t>(v);
      } catch (...) {
        std::cerr << "[Error] --max-steps requiere un entero >= 0\n";
        return 1;
      }
    } else {
      std::cerr << "[Aviso] Opción desconocida: " << arg << "\n";
    }
  }

  // Cargar la Máquina de Turing (detectar automáticamente si es multicinta)
  TuringMachine machine;
  std::unique_ptr<MultiTuringMachine> multi_machine;
  bool is_multi_tape = false;
  
  try {
    // Intentar cargar como máquina multicinta primero
    multi_machine = std::make_unique<MultiTuringMachine>(1); // Temporal, se reemplazará al cargar
    if (Parser::load_multi_from_file(machine_path, *multi_machine)) {
      is_multi_tape = true;
    } else {
      // Si falla, intentar cargar como máquina de una cinta
      multi_machine.reset(); // Limpiar el puntero
      if (!Parser::load_from_file(machine_path, machine)) {
        std::cerr << "[Error carga] " << Parser::get_last_error() << "\n";
        return 2;
      }
    }
  } catch (const std::exception& e) {
    std::cerr << "[Error carga] " << e.what() << "\n";
    return 2;
  }

  // Si se solicita información, mostrarla y terminar
  if (show_info) {
    if (is_multi_tape) {
      std::cout << "=== MÁQUINA DE TURING MULTICINTA ===" << std::endl;
      std::cout << multi_machine->get_info() << std::endl;
    } else {
      std::cout << "=== MÁQUINA DE TURING MONOCINTA ===" << std::endl;
      std::cout << machine.get_info() << std::endl;
    }
    return 0;
  }

  // Crear el simulador apropiado
  std::unique_ptr<Simulator> simulator;
  std::unique_ptr<MultiSimulator> multi_simulator;
  
  if (is_multi_tape) {
    multi_simulator = std::make_unique<MultiSimulator>(multi_machine.get());
  } else {
    simulator = std::make_unique<Simulator>(&machine);
  }

  // Fuente de palabras: fichero o stdin
  std::unique_ptr<std::istream> file_in;
  std::istream* in = &std::cin;
  
  if (words_path.has_value()) {
    file_in = std::make_unique<std::ifstream>(words_path.value());
    if (!*file_in) {
      std::cerr << "[Error] No se puede abrir fichero de palabras: "
                << words_path.value() << "\n";
      return 3;
    }
    in = file_in.get();
  }

  // Procesar palabras
  std::string line;
  while (std::getline(*in, line)) {
    // Permitir espacios alrededor, línea vacía = palabra vacía (épsilon)
    std::string word = strip_spaces(line);

    // Validación del alfabeto según el tipo de máquina
    std::string bad_symbol;
    bool valid_word = false;
    
    if (is_multi_tape) {
      valid_word = word_in_alphabet_multi(word, *multi_machine, &bad_symbol);
    } else {
      valid_word = word_in_alphabet(word, machine, &bad_symbol);
    }
    
    if (!valid_word) {
      if (strict_mode) {
        std::cerr << "[Error palabra] símbolo fuera del alfabeto: '" 
                  << bad_symbol << "' en \"" << word << "\"\n";
        std::cout << "REJECT\n";
        continue;
      } else {
        // Modo no estricto: la palabra simplemente no pertenece al lenguaje
        std::cout << "REJECT\n";
        continue;
      }
    }

    // Simular la máquina con la palabra
    try {
      SimulationResult result;
      
      if (is_multi_tape) {
        result = multi_simulator->simulate(word, trace, max_steps);
      } else {
        result = simulator->simulate(word, trace, max_steps);
      }
      
      // Mostrar el resultado
      std::cout << Simulator::result_to_string(result) << "\n";
      
      // Mostrar el estado final de la cinta/cintas
      if (is_multi_tape) {
        const auto& config = multi_simulator->get_current_configuration();
        const auto& tapes = config.get_tapes();
        std::cout << "Cintas finales:\n";
        for (size_t i = 0; i < tapes.get_num_tapes(); ++i) {
          std::cout << "  Cinta " << (i+1) << ": " << tapes.get_tape(i).to_string(20) << "\n";
        }
      } else {
        const auto& config = simulator->get_current_configuration();
        std::cout << "Cinta final: " << config.get_tape().to_string(20) << "\n";
      }
      
      // Si está habilitada la traza, mostrarla
      if (trace) {
        std::cout << "\n=== Traza de ejecución para \"" << word << "\" ===\n";
        if (is_multi_tape) {
          multi_simulator->print_trace(true);
        } else {
          simulator->print_trace(true);
        }
        std::cout << "=== Fin de traza ===\n\n";
      }
      
      // Mostrar información adicional para casos especiales
      if (result == SimulationResult::INFINITE) {
        std::cout << "[Info] Simulación detenida: ";
        bool loop_detected = is_multi_tape ? 
                            multi_simulator->is_infinite_loop_detected() :
                            simulator->is_infinite_loop_detected();
        if (loop_detected) {
          std::cout << "bucle infinito detectado (configuración repetida)\n";
        } else {
          std::cout << "límite de pasos alcanzado (" << max_steps << ")\n";
        }
      } else if (result == SimulationResult::ERROR) {
        std::string error_msg = is_multi_tape ? 
                               multi_simulator->get_last_error() :
                               simulator->get_last_error();
        std::cerr << "[Error simulación] " << error_msg << "\n";
      }
      
    } catch (const std::exception& e) {
      std::cerr << "[Error simulación] " << e.what() << "\n";
      std::cout << "ERROR\n";
    }
  }

  return 0;
}