#include "Configuration.hpp"
#include <sstream>

Configuration::Configuration(const std::string& initial_state, 
                             const std::string& input_string, 
                             char blank_symbol)
    : current_state_(initial_state), 
      tape_(input_string, blank_symbol),
      step_count_(0) {
}

Configuration::Configuration(const Configuration& other)
    : current_state_(other.current_state_), 
      tape_(other.tape_),
      step_count_(other.step_count_) {
}

Configuration& Configuration::operator=(const Configuration& other) {
  if (this != &other) {
    current_state_ = other.current_state_;
    tape_ = other.tape_;
    step_count_ = other.step_count_;
  }
  return *this;
}

Configuration::~Configuration() {
  // Destructor por defecto
}

const std::string& Configuration::get_current_state() const {
  return current_state_;
}

void Configuration::set_current_state(const std::string& state) {
  current_state_ = state;
}

Tape& Configuration::get_tape() {
  return tape_;
}

const Tape& Configuration::get_tape() const {
  return tape_;
}

size_t Configuration::get_step_count() const {
  return step_count_;
}

void Configuration::increment_step_count() {
  step_count_++;
}

void Configuration::set_step_count(size_t steps) {
  step_count_ = steps;
}

std::string Configuration::to_string(bool show_tape_window, int window_size) const {
  std::ostringstream oss;
  
  oss << "Paso " << step_count_ << ": ";
  oss << "Estado: " << current_state_ << ", ";
  oss << "Posición cabezal: " << tape_.get_head_position() << ", ";
  oss << "Símbolo actual: '" << tape_.read() << "'";
  
  if (show_tape_window) {
    oss << "\n  Cinta: " << tape_.to_string(window_size);
  }
  
  return oss.str();
}

std::string Configuration::to_compact_string() const {
  std::ostringstream oss;
  oss << current_state_ << "|" 
      << tape_.get_head_position() << "|" 
      << tape_.get_content();
  return oss.str();
}

bool Configuration::is_equivalent(const Configuration& other) const {
  // Dos configuraciones son equivalentes si tienen:
  // 1. El mismo estado actual
  // 2. La misma posición del cabezal
  // 3. El mismo contenido en la cinta
  
  if (current_state_ != other.current_state_) {
    return false;
  }
  
  if (tape_.get_head_position() != other.tape_.get_head_position()) {
    return false;
  }
  
  // Comparar el contenido de las cintas
  std::string content1 = tape_.get_content();
  std::string content2 = other.tape_.get_content();
  
  return content1 == content2;
}

bool Configuration::operator==(const Configuration& other) const {
  return is_equivalent(other);
}

void Configuration::reset(const std::string& initial_state, 
                         const std::string& input_string) {
  current_state_ = initial_state;
  tape_.reset(input_string);
  step_count_ = 0;
}