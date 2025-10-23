#include "MultiTransition.hpp"
#include <stdexcept>
#include <sstream>

MultiTransition::MultiTransition(const std::string& from_state,
                                 const std::vector<char>& read_symbols,
                                 const std::string& to_state,
                                 const std::vector<char>& write_symbols,
                                 const std::vector<Movement>& movements)
    : from_state_(from_state), read_symbols_(read_symbols),
      to_state_(to_state), write_symbols_(write_symbols),
      movements_(movements) {
  
  if (!is_well_formed()) {
    throw std::invalid_argument("Transición multicinta mal formada: los vectores deben tener el mismo tamaño");
  }
}

MultiTransition::MultiTransition() 
    : from_state_(""), to_state_("") {
}

MultiTransition::~MultiTransition() {
  // Destructor por defecto
}

MultiTransition::MultiTransition(const MultiTransition& other)
    : from_state_(other.from_state_), read_symbols_(other.read_symbols_),
      to_state_(other.to_state_), write_symbols_(other.write_symbols_),
      movements_(other.movements_) {
}

MultiTransition& MultiTransition::operator=(const MultiTransition& other) {
  if (this != &other) {
    from_state_ = other.from_state_;
    read_symbols_ = other.read_symbols_;
    to_state_ = other.to_state_;
    write_symbols_ = other.write_symbols_;
    movements_ = other.movements_;
  }
  return *this;
}

const std::string& MultiTransition::get_from_state() const {
  return from_state_;
}

const std::vector<char>& MultiTransition::get_read_symbols() const {
  return read_symbols_;
}

const std::string& MultiTransition::get_to_state() const {
  return to_state_;
}

const std::vector<char>& MultiTransition::get_write_symbols() const {
  return write_symbols_;
}

const std::vector<Movement>& MultiTransition::get_movements() const {
  return movements_;
}

char MultiTransition::get_read_symbol(size_t tape_index) const {
  if (tape_index >= read_symbols_.size()) {
    throw std::out_of_range("Índice de cinta fuera de rango");
  }
  return read_symbols_[tape_index];
}

char MultiTransition::get_write_symbol(size_t tape_index) const {
  if (tape_index >= write_symbols_.size()) {
    throw std::out_of_range("Índice de cinta fuera de rango");
  }
  return write_symbols_[tape_index];
}

Movement MultiTransition::get_movement(size_t tape_index) const {
  if (tape_index >= movements_.size()) {
    throw std::out_of_range("Índice de cinta fuera de rango");
  }
  return movements_[tape_index];
}

size_t MultiTransition::get_num_tapes() const {
  return read_symbols_.size();
}

bool MultiTransition::is_applicable(const std::string& current_state,
                                   const std::vector<char>& current_symbols) const {
  if (from_state_ != current_state) {
    return false;
  }
  
  if (current_symbols.size() != read_symbols_.size()) {
    return false;
  }
  
  // Verificar que todos los símbolos coincidan
  for (size_t i = 0; i < read_symbols_.size(); ++i) {
    if (current_symbols[i] != read_symbols_[i]) {
      return false;
    }
  }
  
  return true;
}

std::string MultiTransition::to_string() const {
  std::ostringstream oss;
  
  oss << from_state_ << " ";
  
  // Símbolos leídos
  for (size_t i = 0; i < read_symbols_.size(); ++i) {
    if (i > 0) oss << ",";
    oss << read_symbols_[i];
  }
  oss << " " << to_state_ << " ";
  
  // Símbolos escritos
  for (size_t i = 0; i < write_symbols_.size(); ++i) {
    if (i > 0) oss << ",";
    oss << write_symbols_[i];
  }
  oss << " ";
  
  // Movimientos
  for (size_t i = 0; i < movements_.size(); ++i) {
    if (i > 0) oss << ",";
    switch (movements_[i]) {
      case Movement::LEFT:  oss << "L"; break;
      case Movement::RIGHT: oss << "R"; break;
      case Movement::STAY:  oss << "S"; break;
    }
  }
  
  return oss.str();
}

bool MultiTransition::is_well_formed() const {
  size_t num_tapes = read_symbols_.size();
  
  return (write_symbols_.size() == num_tapes) &&
         (movements_.size() == num_tapes) &&
         (num_tapes > 0);
}

MultiTransition MultiTransition::from_mono_transition(const Transition& mono_transition,
                                                     size_t num_tapes,
                                                     size_t target_tape) {
  if (target_tape >= num_tapes) {
    throw std::out_of_range("Índice de cinta destino fuera de rango");
  }
  
  std::vector<char> read_symbols(num_tapes, '.');  // Por defecto, leer símbolo blanco
  std::vector<char> write_symbols(num_tapes, '.'); // Por defecto, escribir símbolo blanco
  std::vector<Movement> movements(num_tapes, Movement::STAY); // Por defecto, no mover
  
  // Configurar la cinta específica con la transición monocinta
  read_symbols[target_tape] = mono_transition.get_read_symbol();
  write_symbols[target_tape] = mono_transition.get_write_symbol();
  movements[target_tape] = mono_transition.get_movement();
  
  return MultiTransition(mono_transition.get_from_state(),
                        read_symbols,
                        mono_transition.get_to_state(),
                        write_symbols,
                        movements);
}

std::vector<Movement> MultiTransition::chars_to_movements(const std::vector<char>& movement_chars) {
  std::vector<Movement> movements;
  movements.reserve(movement_chars.size());
  
  for (char c : movement_chars) {
    movements.push_back(Transition::char_to_movement(c));
  }
  
  return movements;
}

std::vector<char> MultiTransition::movements_to_chars(const std::vector<Movement>& movements) {
  std::vector<char> chars;
  chars.reserve(movements.size());
  
  for (Movement mov : movements) {
    switch (mov) {
      case Movement::LEFT:  chars.push_back('L'); break;
      case Movement::RIGHT: chars.push_back('R'); break;
      case Movement::STAY:  chars.push_back('S'); break;
    }
  }
  
  return chars;
}