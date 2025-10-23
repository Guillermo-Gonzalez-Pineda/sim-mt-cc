#include "Parser.hpp"
#include "MultiTuringMachine.hpp"
#include "MultiTransition.hpp"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <algorithm>

// Inicialización de la variable estática
std::string Parser::last_error_ = "";

std::string Parser::trim(const std::string& str) {
  size_t start = str.find_first_not_of(" \t\r\n");
  if (start == std::string::npos) {
    return "";
  }
  size_t end = str.find_last_not_of(" \t\r\n");
  return str.substr(start, end - start + 1);
}

std::vector<std::string> Parser::split(const std::string& str) {
  std::vector<std::string> tokens;
  std::istringstream iss(str);
  std::string token;
  
  while (iss >> token) {
    tokens.push_back(token);
  }
  
  return tokens;
}

bool Parser::is_comment(const std::string& line) {
  std::string trimmed = trim(line);
  return !trimmed.empty() && trimmed[0] == '#';
}

bool Parser::is_empty_line(const std::string& line) {
  return trim(line).empty();
}

char Parser::string_to_char(const std::string& str, const std::string& context) {
  if (str.empty()) {
    throw std::invalid_argument("Símbolo vacío en " + context);
  }
  
  if (str.length() == 1) {
    return str[0];
  }
  
  // Manejar casos especiales como espacios
  if (str == "espacio" || str == "space") {
    return ' ';
  }
  
  if (str.length() > 1) {
    throw std::invalid_argument("Símbolo inválido '" + str + "' en " + context + 
                               " (debe ser un solo carácter)");
  }
  
  return str[0];
}

Transition Parser::parse_transition(const std::string& line, int line_number) {
  std::vector<std::string> tokens = split(line);
  
  if (tokens.size() != 5) {
    throw std::invalid_argument("Línea " + std::to_string(line_number) + 
                               ": Transición debe tener 5 elementos: estado_origen símbolo_leído estado_destino símbolo_escrito movimiento");
  }
  
  std::string from_state = tokens[0];
  char read_symbol = string_to_char(tokens[1], "símbolo leído (línea " + std::to_string(line_number) + ")");
  std::string to_state = tokens[2];
  char write_symbol = string_to_char(tokens[3], "símbolo escrito (línea " + std::to_string(line_number) + ")");
  
  Movement movement;
  try {
    movement = Transition::char_to_movement(string_to_char(tokens[4], "movimiento (línea " + std::to_string(line_number) + ")"));
  } catch (const std::exception& e) {
    throw std::invalid_argument("Línea " + std::to_string(line_number) + ": " + std::string(e.what()));
  }
  
  return Transition(from_state, read_symbol, to_state, write_symbol, movement);
}

bool Parser::load_from_file(const std::string& filename, TuringMachine& machine) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    last_error_ = "No se puede abrir el archivo: " + filename;
    return false;
  }
  
  bool result = load_from_stream(file, machine);
  file.close();
  return result;
}

bool Parser::load_from_stream(std::istream& input, TuringMachine& machine) {
  try {
    machine.clear();
    std::string line;
    int line_number = 0;
    int section = 0;  // 0=estados, 1=alfabeto_entrada, 2=alfabeto_cinta, 3=inicial, 4=blanco, 5=aceptación, 6=transiciones
    
    while (std::getline(input, line)) {
      line_number++;
      
      // Saltar comentarios y líneas vacías
      if (is_comment(line) || is_empty_line(line)) {
        continue;
      }
      
      line = trim(line);
      
      switch (section) {
        case 0: {  // Estados
          std::vector<std::string> states = split(line);
          if (states.empty()) {
            throw std::invalid_argument("Línea " + std::to_string(line_number) + ": Debe haber al menos un estado");
          }
          for (const std::string& state : states) {
            machine.add_state(state);
          }
          section++;
          break;
        }
        
        case 1: {  // Alfabeto de entrada
          std::vector<std::string> symbols = split(line);
          for (const std::string& symbol_str : symbols) {
            char symbol = string_to_char(symbol_str, "alfabeto de entrada (línea " + std::to_string(line_number) + ")");
            machine.add_input_symbol(symbol);
          }
          section++;
          break;
        }
        
        case 2: {  // Alfabeto de cinta
          std::vector<std::string> symbols = split(line);
          for (const std::string& symbol_str : symbols) {
            char symbol = string_to_char(symbol_str, "alfabeto de cinta (línea " + std::to_string(line_number) + ")");
            machine.add_tape_symbol(symbol);
          }
          section++;
          break;
        }
        
        case 3: {  // Estado inicial
          std::vector<std::string> tokens = split(line);
          if (tokens.size() != 1) {
            throw std::invalid_argument("Línea " + std::to_string(line_number) + ": Debe haber exactamente un estado inicial");
          }
          machine.set_initial_state(tokens[0]);
          section++;
          break;
        }
        
        case 4: {  // Símbolo blanco
          std::vector<std::string> tokens = split(line);
          if (tokens.size() != 1) {
            throw std::invalid_argument("Línea " + std::to_string(line_number) + ": Debe haber exactamente un símbolo blanco");
          }
          char blank_symbol = string_to_char(tokens[0], "símbolo blanco (línea " + std::to_string(line_number) + ")");
          machine.set_blank_symbol(blank_symbol);
          section++;
          break;
        }
        
        case 5: {  // Estados de aceptación
          std::vector<std::string> accept_states = split(line);
          for (const std::string& state : accept_states) {
            machine.add_accept_state(state);
          }
          section++;
          break;
        }
        
        case 6: {  // Transiciones
          try {
            Transition transition = parse_transition(line, line_number);
            machine.add_transition(transition);
          } catch (const std::exception& e) {
            throw std::invalid_argument(std::string(e.what()));
          }
          break;
        }
        
        default:
          // Continuamos parseando transiciones
          if (section > 6) {
            try {
              Transition transition = parse_transition(line, line_number);
              machine.add_transition(transition);
            } catch (const std::exception& e) {
              throw std::invalid_argument(std::string(e.what()));
            }
          }
          break;
      }
    }
    
    // Verificar que hemos parseado al menos las secciones básicas
    if (section < 6) {
      throw std::invalid_argument("Archivo incompleto: faltan secciones obligatorias");
    }
    
    // Validar la máquina construida
    if (!machine.is_valid()) {
      throw std::invalid_argument("La máquina de Turing resultante no es válida");
    }
    
    last_error_ = "";
    return true;
    
  } catch (const std::exception& e) {
    last_error_ = std::string(e.what());
    return false;
  }
}

bool Parser::save_to_file(const std::string& filename, const TuringMachine& machine) {
  try {
    std::ofstream file(filename);
    if (!file.is_open()) {
      last_error_ = "No se puede crear el archivo: " + filename;
      return false;
    }
    
    file << "# Definición de Máquina de Turing\n";
    file << "# Estados\n";
    
    // Estados
    bool first = true;
    for (const std::string& state : machine.get_states()) {
      if (!first) file << " ";
      file << state;
      first = false;
    }
    file << "\n";
    
    // Alfabeto de entrada
    file << "# Alfabeto de entrada\n";
    first = true;
    for (char symbol : machine.get_input_alphabet()) {
      if (!first) file << " ";
      file << symbol;
      first = false;
    }
    file << "\n";
    
    // Alfabeto de cinta
    file << "# Alfabeto de cinta\n";
    first = true;
    for (char symbol : machine.get_tape_alphabet()) {
      if (!first) file << " ";
      file << symbol;
      first = false;
    }
    file << "\n";
    
    // Estado inicial
    file << "# Estado inicial\n";
    file << machine.get_initial_state() << "\n";
    
    // Símbolo blanco
    file << "# Símbolo blanco\n";
    file << machine.get_blank_symbol() << "\n";
    
    // Estados de aceptación
    file << "# Estados de aceptación\n";
    first = true;
    for (const std::string& state : machine.get_accept_states()) {
      if (!first) file << " ";
      file << state;
      first = false;
    }
    file << "\n";
    
    // Transiciones
    file << "# Transiciones\n";
    std::vector<Transition> transitions = machine.get_all_transitions();
    for (const Transition& trans : transitions) {
      file << trans.to_string() << "\n";
    }
    
    file.close();
    last_error_ = "";
    return true;
    
  } catch (const std::exception& e) {
    last_error_ = std::string(e.what());
    return false;
  }
}

bool Parser::validate_file_format(const std::string& filename) {
  TuringMachine temp_machine('.');  // Usar constructor explícito
  return load_from_file(filename, temp_machine);
}

std::string Parser::get_last_error() {
  return last_error_;
}

bool Parser::load_multi_from_file(const std::string& filename, MultiTuringMachine& machine) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    last_error_ = "No se puede abrir el archivo: " + filename;
    return false;
  }
  
  try {
    machine.clear();
    std::string line;
    int line_number = 0;
    int section = -1;  // -1=buscar MULTICINTA, 0=estados, 1=alfabeto_entrada, etc.
    size_t num_tapes = 1;
    
    while (std::getline(file, line)) {
      line_number++;
      
      // Saltar comentarios y líneas vacías
      if (is_comment(line) || is_empty_line(line)) {
        continue;
      }
      
      line = trim(line);
      
      // Buscar marcador MULTICINTA
      if (section == -1) {
        if (line.substr(0, 10) == "MULTICINTA") {
          std::vector<std::string> tokens = split(line);
          if (tokens.size() != 2) {
            throw std::invalid_argument("Línea " + std::to_string(line_number) + 
                                       ": Formato incorrecto para MULTICINTA. Esperado: MULTICINTA <num_cintas>");
          }
          num_tapes = static_cast<size_t>(std::stoi(tokens[1]));
          machine.set_num_tapes(num_tapes);
          section = 0;
          continue;
        } else {
          throw std::invalid_argument("Línea " + std::to_string(line_number) + 
                                     ": Se esperaba marcador MULTICINTA al inicio del archivo");
        }
      }
      
      switch (section) {
        case 0: {  // Estados
          std::vector<std::string> states = split(line);
          if (states.empty()) {
            throw std::invalid_argument("Línea " + std::to_string(line_number) + ": Debe haber al menos un estado");
          }
          for (const std::string& state : states) {
            machine.add_state(state);
          }
          section++;
          break;
        }
        
        case 1: {  // Alfabeto de entrada
          std::vector<std::string> symbols = split(line);
          for (const std::string& symbol_str : symbols) {
            char symbol = string_to_char(symbol_str, "alfabeto de entrada (línea " + std::to_string(line_number) + ")");
            machine.add_input_symbol(symbol);
          }
          section++;
          break;
        }
        
        case 2: {  // Alfabeto de cinta
          std::vector<std::string> symbols = split(line);
          for (const std::string& symbol_str : symbols) {
            char symbol = string_to_char(symbol_str, "alfabeto de cinta (línea " + std::to_string(line_number) + ")");
            machine.add_tape_symbol(symbol);
          }
          section++;
          break;
        }
        
        case 3: {  // Estado inicial
          std::vector<std::string> tokens = split(line);
          if (tokens.size() != 1) {
            throw std::invalid_argument("Línea " + std::to_string(line_number) + ": Debe haber exactamente un estado inicial");
          }
          machine.set_initial_state(tokens[0]);
          section++;
          break;
        }
        
        case 4: {  // Símbolo blanco
          std::vector<std::string> tokens = split(line);
          if (tokens.size() != 1) {
            throw std::invalid_argument("Línea " + std::to_string(line_number) + ": Debe haber exactamente un símbolo blanco");
          }
          char blank_symbol = string_to_char(tokens[0], "símbolo blanco (línea " + std::to_string(line_number) + ")");
          machine.set_blank_symbol(blank_symbol);
          section++;
          break;
        }
        
        case 5: {  // Estados de aceptación
          std::vector<std::string> accept_states = split(line);
          for (const std::string& state : accept_states) {
            machine.add_accept_state(state);
          }
          section++;
          break;
        }
        
        default: {  // Transiciones multicinta
          try {
            MultiTransition transition = parse_multi_transition(line, line_number, num_tapes);
            machine.add_transition(transition);
          } catch (const std::exception& e) {
            throw std::invalid_argument(std::string(e.what()));
          }
          break;
        }
      }
    }
    
    // Verificar que hemos parseado al menos las secciones básicas
    if (section < 6) {
      throw std::invalid_argument("Archivo incompleto: faltan secciones obligatorias");
    }
    
    // Validar la máquina construida
    if (!machine.is_valid()) {
      throw std::invalid_argument("La máquina de Turing multicinta resultante no es válida");
    }
    
    last_error_ = "";
    file.close();
    return true;
    
  } catch (const std::exception& e) {
    last_error_ = std::string(e.what());
    file.close();
    return false;
  }
}

MultiTransition Parser::parse_multi_transition(const std::string& line, int line_number, size_t num_tapes) {
  std::vector<std::string> tokens = split(line);
  
  if (tokens.size() != 5) {
    throw std::invalid_argument("Línea " + std::to_string(line_number) + 
                               ": Transición multicinta debe tener 5 elementos: estado_origen símbolos_leídos estado_destino símbolos_escritos movimientos");
  }
  
  std::string from_state = tokens[0];
  std::string to_state = tokens[2];
  
  // Parsear símbolos leídos (formato: símb1,símb2,...)
  std::vector<std::string> read_tokens;
  std::istringstream read_stream(tokens[1]);
  std::string read_token;
  while (std::getline(read_stream, read_token, ',')) {
    read_tokens.push_back(read_token);
  }
  
  if (read_tokens.size() != num_tapes) {
    throw std::invalid_argument("Línea " + std::to_string(line_number) + 
                               ": Número de símbolos leídos (" + std::to_string(read_tokens.size()) + 
                               ") no coincide con número de cintas (" + std::to_string(num_tapes) + ")");
  }
  
  std::vector<char> read_symbols;
  for (const std::string& token : read_tokens) {
    read_symbols.push_back(string_to_char(token, "símbolo leído (línea " + std::to_string(line_number) + ")"));
  }
  
  // Parsear símbolos escritos (formato: símb1,símb2,...)
  std::vector<std::string> write_tokens;
  std::istringstream write_stream(tokens[3]);
  std::string write_token;
  while (std::getline(write_stream, write_token, ',')) {
    write_tokens.push_back(write_token);
  }
  
  if (write_tokens.size() != num_tapes) {
    throw std::invalid_argument("Línea " + std::to_string(line_number) + 
                               ": Número de símbolos escritos (" + std::to_string(write_tokens.size()) + 
                               ") no coincide con número de cintas (" + std::to_string(num_tapes) + ")");
  }
  
  std::vector<char> write_symbols;
  for (const std::string& token : write_tokens) {
    write_symbols.push_back(string_to_char(token, "símbolo escrito (línea " + std::to_string(line_number) + ")"));
  }
  
  // Parsear movimientos (formato: mov1,mov2,...)
  std::vector<std::string> move_tokens;
  std::istringstream move_stream(tokens[4]);
  std::string move_token;
  while (std::getline(move_stream, move_token, ',')) {
    move_tokens.push_back(move_token);
  }
  
  if (move_tokens.size() != num_tapes) {
    throw std::invalid_argument("Línea " + std::to_string(line_number) + 
                               ": Número de movimientos (" + std::to_string(move_tokens.size()) + 
                               ") no coincide con número de cintas (" + std::to_string(num_tapes) + ")");
  }
  
  std::vector<Movement> movements;
  for (const std::string& token : move_tokens) {
    char move_char = string_to_char(token, "movimiento (línea " + std::to_string(line_number) + ")");
    try {
      movements.push_back(Transition::char_to_movement(move_char));
    } catch (const std::exception& e) {
      throw std::invalid_argument("Línea " + std::to_string(line_number) + ": " + std::string(e.what()));
    }
  }
  
  return MultiTransition(from_state, read_symbols, to_state, write_symbols, movements);
}

bool Parser::load_auto_detect(const std::string& filename,
                             TuringMachine& mono_machine,
                             MultiTuringMachine& multi_machine,
                             bool& is_multicinta) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    last_error_ = "No se puede abrir el archivo: " + filename;
    return false;
  }
  
  // Leer la primera línea no comentario para detectar el tipo
  std::string line;
  while (std::getline(file, line)) {
    if (!is_comment(line) && !is_empty_line(line)) {
      line = trim(line);
      if (line.substr(0, 10) == "MULTICINTA") {
        is_multicinta = true;
        file.close();
        return load_multi_from_file(filename, multi_machine);
      } else {
        is_multicinta = false;
        file.close();
        return load_from_file(filename, mono_machine);
      }
    }
  }
  
  file.close();
  last_error_ = "No se pudo determinar el tipo de máquina";
  return false;
}