#include "Tape.hpp"
#include <algorithm>
#include <sstream>

Tape::Tape(char blank_symbol) 
    : head_position_(0), blank_symbol_(blank_symbol) {
}

Tape::Tape(const std::string& input_string, char blank_symbol) 
    : head_position_(0), blank_symbol_(blank_symbol) {
  reset(input_string);
}

Tape::~Tape() {
  // Destructor por defecto
}

char Tape::read() const {
  // Si la posición no existe en el mapa, devuelve el símbolo blanco
  auto it = cells_.find(head_position_);
  if (it != cells_.end()) {
    return it->second;
  }
  return blank_symbol_;
}

void Tape::write(char symbol) {
  if (symbol == blank_symbol_) {
    // Si escribimos el símbolo blanco, eliminamos la entrada del mapa
    // para ahorrar memoria
    cells_.erase(head_position_);
  } else {
    // Escribimos el símbolo en la posición actual
    cells_[head_position_] = symbol;
  }
}

void Tape::move_left() {
  head_position_--;
}

void Tape::move_right() {
  head_position_++;
}

int Tape::get_head_position() const {
  return head_position_;
}

void Tape::set_head_position(int position) {
  head_position_ = position;
}

char Tape::get_blank_symbol() const {
  return blank_symbol_;
}

void Tape::reset(const std::string& input_string) {
  // Limpiar la cinta
  cells_.clear();
  head_position_ = 0;
  
  // Escribir la cadena de entrada en la cinta, empezando en la posición 0
  for (size_t i = 0; i < input_string.length(); ++i) {
    if (input_string[i] != blank_symbol_) {
      cells_[static_cast<int>(i)] = input_string[i];
    }
  }
}

std::string Tape::to_string(int window_size) const {
  std::ostringstream oss;
  
  // Determinar el rango de posiciones a mostrar
  int start = head_position_ - window_size;
  int end = head_position_ + window_size;
  
  // Mostrar las celdas
  for (int pos = start; pos <= end; ++pos) {
    if (pos == head_position_) {
      oss << "[";  // Marcar la posición del cabezal
    } else {
      oss << " ";
    }
    
    // Obtener el símbolo en la posición
    auto it = cells_.find(pos);
    if (it != cells_.end()) {
      oss << it->second;
    } else {
      oss << blank_symbol_;
    }
    
    if (pos == head_position_) {
      oss << "]";
    } else {
      oss << " ";
    }
  }
  
  return oss.str();
}

std::string Tape::get_content() const {
  if (cells_.empty()) {
    return "";
  }
  
  // Encontrar las posiciones mínima y máxima con contenido
  auto minmax = std::minmax_element(cells_.begin(), cells_.end(),
    [](const auto& a, const auto& b) {
      return a.first < b.first;
    });
  
  int min_pos = minmax.first->first;
  int max_pos = minmax.second->first;
  
  std::string result;
  result.reserve(max_pos - min_pos + 1);
  
  // Construir la cadena desde la posición mínima hasta la máxima
  for (int pos = min_pos; pos <= max_pos; ++pos) {
    auto it = cells_.find(pos);
    if (it != cells_.end()) {
      result += it->second;
    } else {
      result += blank_symbol_;
    }
  }
  
  return result;
}

bool Tape::is_empty() const {
  return cells_.empty();
}