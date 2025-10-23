#include "MultiTape.hpp"
#include <stdexcept>
#include <sstream>

MultiTape::MultiTape(size_t num_tapes, char blank_symbol) 
    : num_tapes_(num_tapes) {
  if (num_tapes == 0) {
    throw std::invalid_argument("El número de cintas debe ser mayor que 0");
  }
  
  // Crear todas las cintas con el mismo símbolo blanco
  tapes_.reserve(num_tapes);
  for (size_t i = 0; i < num_tapes; ++i) {
    tapes_.emplace_back(blank_symbol);
  }
}

MultiTape::MultiTape(size_t num_tapes, const std::string& input_word, char blank_symbol)
    : num_tapes_(num_tapes) {
  if (num_tapes == 0) {
    throw std::invalid_argument("El número de cintas debe ser mayor que 0");
  }
  
  tapes_.reserve(num_tapes);
  
  // Primera cinta con la palabra de entrada
  tapes_.emplace_back(input_word, blank_symbol);
  
  // Resto de cintas vacías
  for (size_t i = 1; i < num_tapes; ++i) {
    tapes_.emplace_back(blank_symbol);
  }
}

MultiTape::MultiTape(const MultiTape& other) 
    : tapes_(other.tapes_), num_tapes_(other.num_tapes_) {
}

MultiTape& MultiTape::operator=(const MultiTape& other) {
  if (this != &other) {
    tapes_ = other.tapes_;
    num_tapes_ = other.num_tapes_;
  }
  return *this;
}

MultiTape::~MultiTape() {
  // Destructor por defecto
}

char MultiTape::read(size_t tape_index) const {
  if (!is_valid_tape_index(tape_index)) {
    throw std::out_of_range("Índice de cinta inválido: " + std::to_string(tape_index));
  }
  return tapes_[tape_index].read();
}

void MultiTape::write(size_t tape_index, char symbol) {
  if (!is_valid_tape_index(tape_index)) {
    throw std::out_of_range("Índice de cinta inválido: " + std::to_string(tape_index));
  }
  tapes_[tape_index].write(symbol);
}

void MultiTape::move(size_t tape_index, Movement movement) {
  if (!is_valid_tape_index(tape_index)) {
    throw std::out_of_range("Índice de cinta inválido: " + std::to_string(tape_index));
  }
  
  switch (movement) {
    case Movement::LEFT:
      tapes_[tape_index].move_left();
      break;
    case Movement::RIGHT:
      tapes_[tape_index].move_right();
      break;
    case Movement::STAY:
      // No mover
      break;
  }
}

size_t MultiTape::get_num_tapes() const {
  return num_tapes_;
}

int MultiTape::get_head_position(size_t tape_index) const {
  if (!is_valid_tape_index(tape_index)) {
    throw std::out_of_range("Índice de cinta inválido: " + std::to_string(tape_index));
  }
  return tapes_[tape_index].get_head_position();
}

void MultiTape::set_head_position(size_t tape_index, int position) {
  if (!is_valid_tape_index(tape_index)) {
    throw std::out_of_range("Índice de cinta inválido: " + std::to_string(tape_index));
  }
  tapes_[tape_index].set_head_position(position);
}

char MultiTape::get_blank_symbol() const {
  if (tapes_.empty()) {
    return '.';  // Valor por defecto
  }
  return tapes_[0].get_blank_symbol();
}

void MultiTape::reset(const std::string& input_word) {
  // Reiniciar la primera cinta con la palabra de entrada
  if (!tapes_.empty()) {
    tapes_[0].reset(input_word);
  }
  
  // Reiniciar el resto de cintas como vacías
  for (size_t i = 1; i < tapes_.size(); ++i) {
    tapes_[i].reset("");
  }
}

std::string MultiTape::to_string(int window_size) const {
  std::ostringstream oss;
  
  for (size_t i = 0; i < num_tapes_; ++i) {
    oss << "Cinta " << i << ": " << tapes_[i].to_string(window_size);
    if (i < num_tapes_ - 1) {
      oss << "\n";
    }
  }
  
  return oss.str();
}

std::string MultiTape::get_tape_content(size_t tape_index) const {
  if (!is_valid_tape_index(tape_index)) {
    throw std::out_of_range("Índice de cinta inválido: " + std::to_string(tape_index));
  }
  return tapes_[tape_index].get_content();
}

Tape& MultiTape::get_tape(size_t tape_index) {
  if (!is_valid_tape_index(tape_index)) {
    throw std::out_of_range("Índice de cinta inválido: " + std::to_string(tape_index));
  }
  return tapes_[tape_index];
}

const Tape& MultiTape::get_tape(size_t tape_index) const {
  if (!is_valid_tape_index(tape_index)) {
    throw std::out_of_range("Índice de cinta inválido: " + std::to_string(tape_index));
  }
  return tapes_[tape_index];
}

std::vector<char> MultiTape::read_all() const {
  std::vector<char> symbols;
  symbols.reserve(num_tapes_);
  
  for (size_t i = 0; i < num_tapes_; ++i) {
    symbols.push_back(tapes_[i].read());
  }
  
  return symbols;
}

bool MultiTape::is_valid_tape_index(size_t tape_index) const {
  return tape_index < num_tapes_;
}