#include "TuringMachine.hpp"
#include <sstream>
#include <stdexcept>

TuringMachine::TuringMachine(char blank_symbol) 
    : initial_state_(""), blank_symbol_(blank_symbol) {
  // El símbolo blanco siempre debe estar en el alfabeto de la cinta
  tape_alphabet_.insert(blank_symbol_);
}

TuringMachine::~TuringMachine() {
  // Destructor por defecto
}

void TuringMachine::add_state(const std::string& state) {
  if (state.empty()) {
    throw std::invalid_argument("El nombre del estado no puede estar vacío");
  }
  states_.insert(state);
}

void TuringMachine::add_input_symbol(char symbol) {
  if (symbol == blank_symbol_) {
    throw std::invalid_argument("El símbolo blanco no puede estar en el alfabeto de entrada");
  }
  input_alphabet_.insert(symbol);
  // Si está en el alfabeto de entrada, también debe estar en el de la cinta
  tape_alphabet_.insert(symbol);
}

void TuringMachine::add_tape_symbol(char symbol) {
  tape_alphabet_.insert(symbol);
}

void TuringMachine::set_initial_state(const std::string& state) {
  initial_state_ = state;
  // Asegurar que el estado inicial esté en el conjunto de estados
  add_state(state);
}

void TuringMachine::add_accept_state(const std::string& state) {
  accept_states_.insert(state);
  // Asegurar que el estado de aceptación esté en el conjunto de estados
  add_state(state);
}

void TuringMachine::set_blank_symbol(char symbol) {
  // Remover el símbolo blanco anterior del alfabeto de cinta (si no es usado en otras partes)
  if (blank_symbol_ != symbol) {
    blank_symbol_ = symbol;
    tape_alphabet_.insert(blank_symbol_);
  }
}

void TuringMachine::add_transition(const Transition& transition) {
  // Validar que los estados existen
  if (states_.find(transition.get_from_state()) == states_.end()) {
    throw std::invalid_argument("Estado origen '" + transition.get_from_state() + "' no declarado");
  }
  if (states_.find(transition.get_to_state()) == states_.end()) {
    throw std::invalid_argument("Estado destino '" + transition.get_to_state() + "' no declarado");
  }
  
  // Añadir automáticamente los símbolos de cinta si no existen
  add_tape_symbol(transition.get_read_symbol());
  add_tape_symbol(transition.get_write_symbol());
  
  // Crear la clave para el mapa de transiciones
  std::pair<std::string, char> key = {transition.get_from_state(), 
                                      transition.get_read_symbol()};
  
  // Verificar si ya existe una transición para este estado y símbolo
  if (transitions_.find(key) != transitions_.end()) {
    throw std::invalid_argument("Ya existe una transición para el estado '" + 
                               transition.get_from_state() + "' y símbolo '" + 
                               std::string(1, transition.get_read_symbol()) + "'");
  }
  
  transitions_[key] = transition;
}

void TuringMachine::add_transition(const std::string& from_state, char read_symbol,
                                  const std::string& to_state, char write_symbol,
                                  Movement movement) {
  Transition transition(from_state, read_symbol, to_state, write_symbol, movement);
  add_transition(transition);
}

const std::unordered_set<std::string>& TuringMachine::get_states() const {
  return states_;
}

const std::unordered_set<char>& TuringMachine::get_input_alphabet() const {
  return input_alphabet_;
}

const std::unordered_set<char>& TuringMachine::get_tape_alphabet() const {
  return tape_alphabet_;
}

const std::string& TuringMachine::get_initial_state() const {
  return initial_state_;
}

const std::unordered_set<std::string>& TuringMachine::get_accept_states() const {
  return accept_states_;
}

char TuringMachine::get_blank_symbol() const {
  return blank_symbol_;
}

const Transition* TuringMachine::get_transition(const std::string& state, 
                                               char symbol) const {
  std::pair<std::string, char> key = {state, symbol};
  auto it = transitions_.find(key);
  if (it != transitions_.end()) {
    return &(it->second);
  }
  return nullptr;
}

std::vector<Transition> TuringMachine::get_all_transitions() const {
  std::vector<Transition> result;
  result.reserve(transitions_.size());
  
  for (const auto& pair : transitions_) {
    result.push_back(pair.second);
  }
  
  return result;
}

bool TuringMachine::is_valid() const {
  // Verificar que hay al menos un estado
  if (states_.empty()) {
    return false;
  }
  
  // Verificar que el estado inicial está definido y existe
  if (initial_state_.empty() || states_.find(initial_state_) == states_.end()) {
    return false;
  }
  
  // Verificar que todos los estados de aceptación están en el conjunto de estados
  for (const std::string& accept_state : accept_states_) {
    if (states_.find(accept_state) == states_.end()) {
      return false;
    }
  }
  
  // Verificar que el símbolo blanco está en el alfabeto de la cinta
  if (tape_alphabet_.find(blank_symbol_) == tape_alphabet_.end()) {
    return false;
  }
  
  // Verificar que el alfabeto de entrada es subconjunto del alfabeto de cinta
  for (char symbol : input_alphabet_) {
    if (tape_alphabet_.find(symbol) == tape_alphabet_.end()) {
      return false;
    }
    // Verificar que el símbolo blanco no está en el alfabeto de entrada
    if (symbol == blank_symbol_) {
      return false;
    }
  }
  
  // Verificar que todas las transiciones involucran estados y símbolos válidos
  for (const auto& pair : transitions_) {
    const Transition& trans = pair.second;
    
    // Verificar estados
    if (states_.find(trans.get_from_state()) == states_.end() ||
        states_.find(trans.get_to_state()) == states_.end()) {
      return false;
    }
    
    // Verificar símbolos
    if (tape_alphabet_.find(trans.get_read_symbol()) == tape_alphabet_.end() ||
        tape_alphabet_.find(trans.get_write_symbol()) == tape_alphabet_.end()) {
      return false;
    }
  }
  
  return true;
}

bool TuringMachine::is_accept_state(const std::string& state) const {
  return accept_states_.find(state) != accept_states_.end();
}

bool TuringMachine::is_input_symbol(char symbol) const {
  return input_alphabet_.find(symbol) != input_alphabet_.end();
}

bool TuringMachine::is_tape_symbol(char symbol) const {
  return tape_alphabet_.find(symbol) != tape_alphabet_.end();
}

bool TuringMachine::is_valid_input_word(const std::string& word) const {
  for (char symbol : word) {
    if (!is_input_symbol(symbol)) {
      return false;
    }
  }
  return true;
}

std::string TuringMachine::get_info() const {
  std::ostringstream oss;
  
  oss << "=== Información de la Máquina de Turing ===\n";
  oss << "Estados (" << states_.size() << "): {";
  bool first = true;
  for (const std::string& state : states_) {
    if (!first) oss << ", ";
    oss << state;
    first = false;
  }
  oss << "}\n";
  
  oss << "Estado inicial: " << initial_state_ << "\n";
  
  oss << "Estados de aceptación (" << accept_states_.size() << "): {";
  first = true;
  for (const std::string& state : accept_states_) {
    if (!first) oss << ", ";
    oss << state;
    first = false;
  }
  oss << "}\n";
  
  oss << "Alfabeto de entrada (" << input_alphabet_.size() << "): {";
  first = true;
  for (char symbol : input_alphabet_) {
    if (!first) oss << ", ";
    oss << "'" << symbol << "'";
    first = false;
  }
  oss << "}\n";
  
  oss << "Alfabeto de cinta (" << tape_alphabet_.size() << "): {";
  first = true;
  for (char symbol : tape_alphabet_) {
    if (!first) oss << ", ";
    oss << "'" << symbol << "'";
    first = false;
  }
  oss << "}\n";
  
  oss << "Símbolo blanco: '" << blank_symbol_ << "'\n";
  oss << "Número de transiciones: " << transitions_.size() << "\n";
  oss << "Máquina válida: " << (is_valid() ? "Sí" : "No") << "\n";
  
  return oss.str();
}

void TuringMachine::clear() {
  states_.clear();
  input_alphabet_.clear();
  tape_alphabet_.clear();
  initial_state_.clear();
  accept_states_.clear();
  transitions_.clear();
  // Mantener el símbolo blanco
  tape_alphabet_.insert(blank_symbol_);
}

size_t TuringMachine::get_transition_count() const {
  return transitions_.size();
}