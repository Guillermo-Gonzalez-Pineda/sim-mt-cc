#include "Transition.hpp"
#include <stdexcept>

Transition::Transition(const std::string& from_state, char read_symbol, 
                       const std::string& to_state, char write_symbol, 
                       Movement movement)
    : from_state_(from_state), read_symbol_(read_symbol), 
      to_state_(to_state), write_symbol_(write_symbol), 
      movement_(movement) {
}

Transition::Transition() 
    : from_state_(""), read_symbol_('\0'), 
      to_state_(""), write_symbol_('\0'), 
      movement_(Movement::STAY) {
}

Transition::~Transition() {
  // Destructor por defecto
}

const std::string& Transition::get_from_state() const {
  return from_state_;
}

char Transition::get_read_symbol() const {
  return read_symbol_;
}

const std::string& Transition::get_to_state() const {
  return to_state_;
}

char Transition::get_write_symbol() const {
  return write_symbol_;
}

Movement Transition::get_movement() const {
  return movement_;
}

char Transition::movement_to_char() const {
  switch (movement_) {
    case Movement::LEFT:
      return 'L';
    case Movement::RIGHT:
      return 'R';
    case Movement::STAY:
      return 'S';
    default:
      return 'S';  // Por defecto, sin movimiento
  }
}

Movement Transition::char_to_movement(char c) {
  switch (c) {
    case 'L':
    case 'l':
      return Movement::LEFT;
    case 'R':
    case 'r':
      return Movement::RIGHT;
    case 'S':
    case 's':
      return Movement::STAY;
    default:
      throw std::invalid_argument("Movimiento inválido: " + std::string(1, c));
  }
}

std::string Transition::to_string() const {
  return from_state_ + " " + std::string(1, read_symbol_) + " " + 
         to_state_ + " " + std::string(1, write_symbol_) + " " + 
         std::string(1, movement_to_char());
}

bool Transition::is_applicable(const std::string& current_state, 
                               char current_symbol) const {
  return from_state_ == current_state && read_symbol_ == current_symbol;
}