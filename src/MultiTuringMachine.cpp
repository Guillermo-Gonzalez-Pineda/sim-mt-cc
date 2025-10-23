#include "MultiTuringMachine.hpp"
#include "TuringMachine.hpp"
#include <sstream>
#include <stdexcept>

MultiTuringMachine::MultiTuringMachine(size_t num_tapes, char blank_symbol)
    : initial_state_(""), blank_symbol_(blank_symbol), num_tapes_(num_tapes) {
  if (num_tapes == 0) {
    throw std::invalid_argument("El número de cintas debe ser mayor que 0");
  }
  // El símbolo blanco siempre debe estar en el alfabeto de la cinta
  tape_alphabet_.insert(blank_symbol_);
}

MultiTuringMachine::~MultiTuringMachine() {
  // Destructor por defecto
}

void MultiTuringMachine::add_state(const std::string& state) {
  if (state.empty()) {
    throw std::invalid_argument("El nombre del estado no puede estar vacío");
  }
  states_.insert(state);
}

void MultiTuringMachine::add_input_symbol(char symbol) {
  if (symbol == blank_symbol_) {
    throw std::invalid_argument("El símbolo blanco no puede estar en el alfabeto de entrada");
  }
  input_alphabet_.insert(symbol);
  // Si está en el alfabeto de entrada, también debe estar en el de la cinta
  tape_alphabet_.insert(symbol);
}

void MultiTuringMachine::add_tape_symbol(char symbol) {
  tape_alphabet_.insert(symbol);
}

void MultiTuringMachine::set_initial_state(const std::string& state) {
  initial_state_ = state;
  // Asegurar que el estado inicial esté en el conjunto de estados
  add_state(state);
}

void MultiTuringMachine::add_accept_state(const std::string& state) {
  accept_states_.insert(state);
  // Asegurar que el estado de aceptación esté en el conjunto de estados
  add_state(state);
}

void MultiTuringMachine::set_blank_symbol(char symbol) {
  if (blank_symbol_ != symbol) {
    blank_symbol_ = symbol;
    tape_alphabet_.insert(blank_symbol_);
  }
}

void MultiTuringMachine::set_num_tapes(size_t num_tapes) {
  if (num_tapes == 0) {
    throw std::invalid_argument("El número de cintas debe ser mayor que 0");
  }
  num_tapes_ = num_tapes;
}

void MultiTuringMachine::add_transition(const MultiTransition& transition) {
  // Verificar que el número de cintas coincida
  if (transition.get_num_tapes() != num_tapes_) {
    throw std::invalid_argument("El número de cintas de la transición no coincide con la máquina");
  }
  
  // Añadir automáticamente los estados y símbolos involucrados
  add_state(transition.get_from_state());
  add_state(transition.get_to_state());
  
  for (size_t i = 0; i < num_tapes_; ++i) {
    add_tape_symbol(transition.get_read_symbol(i));
    add_tape_symbol(transition.get_write_symbol(i));
  }
  
  // Crear la clave para el mapa de transiciones
  std::pair<std::string, std::vector<char>> key = {transition.get_from_state(),
                                                   transition.get_read_symbols()};
  
  // Verificar si ya existe una transición para este estado y símbolos
  if (transitions_.find(key) != transitions_.end()) {
    std::ostringstream oss;
    oss << "Ya existe una transición para el estado '" << transition.get_from_state() 
        << "' y símbolos [";
    for (size_t i = 0; i < transition.get_read_symbols().size(); ++i) {
      if (i > 0) oss << ",";
      oss << "'" << transition.get_read_symbol(i) << "'";
    }
    oss << "]";
    throw std::invalid_argument(oss.str());
  }
  
  transitions_[key] = transition;
}

void MultiTuringMachine::add_transition(const std::string& from_state,
                                       const std::vector<char>& read_symbols,
                                       const std::string& to_state,
                                       const std::vector<char>& write_symbols,
                                       const std::vector<Movement>& movements) {
  MultiTransition transition(from_state, read_symbols, to_state, write_symbols, movements);
  add_transition(transition);
}

const std::unordered_set<std::string>& MultiTuringMachine::get_states() const {
  return states_;
}

const std::unordered_set<char>& MultiTuringMachine::get_input_alphabet() const {
  return input_alphabet_;
}

const std::unordered_set<char>& MultiTuringMachine::get_tape_alphabet() const {
  return tape_alphabet_;
}

const std::string& MultiTuringMachine::get_initial_state() const {
  return initial_state_;
}

const std::unordered_set<std::string>& MultiTuringMachine::get_accept_states() const {
  return accept_states_;
}

char MultiTuringMachine::get_blank_symbol() const {
  return blank_symbol_;
}

size_t MultiTuringMachine::get_num_tapes() const {
  return num_tapes_;
}

const MultiTransition* MultiTuringMachine::get_transition(const std::string& state,
                                                         const std::vector<char>& symbols) const {
  if (symbols.size() != num_tapes_) {
    return nullptr;
  }
  
  std::pair<std::string, std::vector<char>> key = {state, symbols};
  auto it = transitions_.find(key);
  if (it != transitions_.end()) {
    return &(it->second);
  }
  return nullptr;
}

std::vector<MultiTransition> MultiTuringMachine::get_all_transitions() const {
  std::vector<MultiTransition> result;
  result.reserve(transitions_.size());
  
  for (const auto& pair : transitions_) {
    result.push_back(pair.second);
  }
  
  return result;
}

bool MultiTuringMachine::is_valid() const {
  // Verificar que hay al menos un estado
  if (states_.empty()) {
    return false;
  }
  
  // Verificar que el estado inicial está definido y existe
  if (initial_state_.empty() || states_.find(initial_state_) == states_.end()) {
    return false;
  }
  
  // Verificar que hay al menos una cinta
  if (num_tapes_ == 0) {
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
    if (symbol == blank_symbol_) {
      return false;
    }
  }
  
  // Verificar que todas las transiciones están bien formadas
  for (const auto& pair : transitions_) {
    const MultiTransition& trans = pair.second;
    
    // Verificar número de cintas
    if (trans.get_num_tapes() != num_tapes_) {
      return false;
    }
    
    // Verificar estados
    if (states_.find(trans.get_from_state()) == states_.end() ||
        states_.find(trans.get_to_state()) == states_.end()) {
      return false;
    }
    
    // Verificar símbolos
    for (size_t i = 0; i < num_tapes_; ++i) {
      if (tape_alphabet_.find(trans.get_read_symbol(i)) == tape_alphabet_.end() ||
          tape_alphabet_.find(trans.get_write_symbol(i)) == tape_alphabet_.end()) {
        return false;
      }
    }
  }
  
  return true;
}

bool MultiTuringMachine::is_accept_state(const std::string& state) const {
  return accept_states_.find(state) != accept_states_.end();
}

bool MultiTuringMachine::is_input_symbol(char symbol) const {
  return input_alphabet_.find(symbol) != input_alphabet_.end();
}

bool MultiTuringMachine::is_tape_symbol(char symbol) const {
  return tape_alphabet_.find(symbol) != tape_alphabet_.end();
}

bool MultiTuringMachine::is_valid_input_word(const std::string& word) const {
  for (char symbol : word) {
    if (!is_input_symbol(symbol)) {
      return false;
    }
  }
  return true;
}

std::string MultiTuringMachine::get_info() const {
  std::ostringstream oss;
  
  oss << "=== Información de la Máquina de Turing Multicinta ===\n";
  oss << "Número de cintas: " << num_tapes_ << "\n";
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

void MultiTuringMachine::clear() {
  states_.clear();
  input_alphabet_.clear();
  tape_alphabet_.clear();
  initial_state_.clear();
  accept_states_.clear();
  transitions_.clear();
  // Mantener el símbolo blanco y número de cintas
  tape_alphabet_.insert(blank_symbol_);
}

size_t MultiTuringMachine::get_transition_count() const {
  return transitions_.size();
}

MultiTuringMachine MultiTuringMachine::from_mono_machine(const TuringMachine& mono_machine,
                                                        size_t num_tapes) {
  if (num_tapes == 0) {
    throw std::invalid_argument("El número de cintas debe ser mayor que 0");
  }
  
  MultiTuringMachine multi_machine(num_tapes, mono_machine.get_blank_symbol());
  
  // Copiar estados, alfabetos y configuración básica
  for (const std::string& state : mono_machine.get_states()) {
    multi_machine.add_state(state);
  }
  
  for (char symbol : mono_machine.get_input_alphabet()) {
    multi_machine.add_input_symbol(symbol);
  }
  
  for (char symbol : mono_machine.get_tape_alphabet()) {
    multi_machine.add_tape_symbol(symbol);
  }
  
  multi_machine.set_initial_state(mono_machine.get_initial_state());
  
  for (const std::string& state : mono_machine.get_accept_states()) {
    multi_machine.add_accept_state(state);
  }
  
  // Convertir transiciones monocinta a multicinta
  std::vector<Transition> mono_transitions = mono_machine.get_all_transitions();
  for (const Transition& mono_trans : mono_transitions) {
    MultiTransition multi_trans = MultiTransition::from_mono_transition(mono_trans, num_tapes, 0);
    multi_machine.add_transition(multi_trans);
  }
  
  return multi_machine;
}