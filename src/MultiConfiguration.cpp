#include "MultiConfiguration.hpp"
#include <sstream>

MultiConfiguration::MultiConfiguration(const std::string& initial_state, 
                                       size_t num_tapes,
                                       const std::string& input_string, 
                                       char blank_symbol)
    : current_state_(initial_state), 
      tapes_(num_tapes, input_string, blank_symbol),
      step_count_(0) {
}

MultiConfiguration::MultiConfiguration(const MultiConfiguration& other)
    : current_state_(other.current_state_), 
      tapes_(other.tapes_),
      step_count_(other.step_count_) {
}

MultiConfiguration& MultiConfiguration::operator=(const MultiConfiguration& other) {
  if (this != &other) {
    current_state_ = other.current_state_;
    tapes_ = other.tapes_;
    step_count_ = other.step_count_;
  }
  return *this;
}

MultiConfiguration::~MultiConfiguration() {
  // Destructor por defecto
}

const std::string& MultiConfiguration::get_current_state() const {
  return current_state_;
}

void MultiConfiguration::set_current_state(const std::string& state) {
  current_state_ = state;
}

MultiTape& MultiConfiguration::get_tapes() {
  return tapes_;
}

const MultiTape& MultiConfiguration::get_tapes() const {
  return tapes_;
}

size_t MultiConfiguration::get_step_count() const {
  return step_count_;
}

void MultiConfiguration::increment_step_count() {
  step_count_++;
}

void MultiConfiguration::set_step_count(size_t steps) {
  step_count_ = steps;
}

std::string MultiConfiguration::to_string(bool show_tape_window, int window_size) const {
  std::ostringstream oss;
  
  oss << "Paso " << step_count_ << ": ";
  oss << "Estado: " << current_state_;
  
  // Mostrar símbolos actuales de cada cinta
  oss << ", Símbolos actuales: [";
  for (size_t i = 0; i < tapes_.get_num_tapes(); ++i) {
    if (i > 0) oss << ", ";
    oss << "'" << tapes_.read(i) << "'";
  }
  oss << "]";
  
  // Mostrar posiciones de cabezales
  oss << ", Posiciones: [";
  for (size_t i = 0; i < tapes_.get_num_tapes(); ++i) {
    if (i > 0) oss << ", ";
    oss << tapes_.get_head_position(i);
  }
  oss << "]";
  
  if (show_tape_window) {
    oss << "\n" << tapes_.to_string(window_size);
  }
  
  return oss.str();
}

std::string MultiConfiguration::to_compact_string() const {
  std::ostringstream oss;
  oss << current_state_ << "|";
  
  // Posiciones de cabezales
  for (size_t i = 0; i < tapes_.get_num_tapes(); ++i) {
    if (i > 0) oss << ",";
    oss << tapes_.get_head_position(i);
  }
  oss << "|";
  
  // Contenidos de cintas
  for (size_t i = 0; i < tapes_.get_num_tapes(); ++i) {
    oss << tapes_.get_tape_content(i);
    if (i < tapes_.get_num_tapes() - 1) {
      oss << "|";
    }
  }
  
  return oss.str();
}

bool MultiConfiguration::is_equivalent(const MultiConfiguration& other) const {
  // Dos configuraciones son equivalentes si tienen:
  // 1. El mismo estado actual
  // 2. Las mismas posiciones de cabezales
  // 3. El mismo contenido en todas las cintas
  
  if (current_state_ != other.current_state_) {
    return false;
  }
  
  if (tapes_.get_num_tapes() != other.tapes_.get_num_tapes()) {
    return false;
  }
  
  // Comparar posiciones de cabezales y contenidos
  for (size_t i = 0; i < tapes_.get_num_tapes(); ++i) {
    if (tapes_.get_head_position(i) != other.tapes_.get_head_position(i)) {
      return false;
    }
    
    if (tapes_.get_tape_content(i) != other.tapes_.get_tape_content(i)) {
      return false;
    }
  }
  
  return true;
}

bool MultiConfiguration::operator==(const MultiConfiguration& other) const {
  return is_equivalent(other);
}

void MultiConfiguration::reset(const std::string& initial_state, 
                              const std::string& input_string) {
  current_state_ = initial_state;
  tapes_.reset(input_string);
  step_count_ = 0;
}