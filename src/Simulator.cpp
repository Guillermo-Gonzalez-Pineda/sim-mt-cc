#include "Simulator.hpp"
#include <iostream>
#include <sstream>

Simulator::Simulator(const TuringMachine* machine)
    : machine_(machine), current_config_("", "", '.'), 
      trace_enabled_(false), max_steps_(1000), last_error_("") {
  if (machine_ == nullptr) {
    last_error_ = "La máquina de Turing no puede ser nullptr";
  }
}

Simulator::~Simulator() {
  // Destructor por defecto
}

SimulationResult Simulator::simulate(const std::string& input_word, 
                                    bool enable_trace, 
                                    size_t max_steps) {
  // Verificar que la máquina sea válida
  if (machine_ == nullptr) {
    last_error_ = "No hay máquina de Turing asignada";
    return SimulationResult::ERROR;
  }
  
  if (!machine_->is_valid()) {
    last_error_ = "La máquina de Turing no es válida";
    return SimulationResult::ERROR;
  }
  
  // Verificar que la palabra de entrada sea válida
  if (!machine_->is_valid_input_word(input_word)) {
    last_error_ = "La palabra de entrada contiene símbolos no válidos";
    return SimulationResult::ERROR;
  }
  
  // Configurar la simulación
  trace_enabled_ = enable_trace;
  max_steps_ = max_steps;
  
  // Reiniciar el simulador
  reset(input_word);
  
  // Añadir configuración inicial a la traza
  add_to_trace();
  mark_configuration_as_visited();
  
  // Bucle principal de simulación
  while (true) {
    // Verificar límite de pasos
    if (max_steps_ > 0 && current_config_.get_step_count() >= max_steps_) {
      return SimulationResult::INFINITE;
    }
    
    // Verificar si estamos en un estado de aceptación
    if (is_accepting_state()) {
      return SimulationResult::ACCEPTED;
    }
    
    // Verificar si hay transición aplicable
    if (!has_applicable_transition()) {
      return SimulationResult::REJECTED;
    }
    
    // Ejecutar un paso
    if (!step()) {
      last_error_ = "Error al ejecutar paso de simulación";
      return SimulationResult::ERROR;
    }
    
    // Verificar bucle infinito por configuraciones repetidas
    if (is_configuration_visited()) {
      return SimulationResult::INFINITE;
    }
    
    // Marcar configuración como visitada y añadir a traza
    mark_configuration_as_visited();
    add_to_trace();
  }
}

bool Simulator::step() {
  if (machine_ == nullptr) {
    return false;
  }
  
  // Obtener transición aplicable
  std::string current_state = current_config_.get_current_state();
  char current_symbol = current_config_.get_tape().read();
  
  const Transition* transition = machine_->get_transition(current_state, current_symbol);
  if (transition == nullptr) {
    return false;
  }
  
  // Aplicar la transición
  // 1. Escribir el nuevo símbolo en la cinta
  current_config_.get_tape().write(transition->get_write_symbol());
  
  // 2. Mover el cabezal
  switch (transition->get_movement()) {
    case Movement::LEFT:
      current_config_.get_tape().move_left();
      break;
    case Movement::RIGHT:
      current_config_.get_tape().move_right();
      break;
    case Movement::STAY:
      // No mover el cabezal
      break;
  }
  
  // 3. Cambiar al nuevo estado
  current_config_.set_current_state(transition->get_to_state());
  
  // 4. Incrementar contador de pasos
  current_config_.increment_step_count();
  
  return true;
}

void Simulator::reset(const std::string& input_word) {
  if (machine_ != nullptr) {
    current_config_.reset(machine_->get_initial_state(), input_word);
    current_config_.get_tape().set_head_position(0);  // Cabezal en posición inicial
  }
  
  trace_.clear();
  visited_configurations_.clear();
  last_error_ = "";
}

bool Simulator::is_accepting_state() const {
  if (machine_ == nullptr) {
    return false;
  }
  
  return machine_->is_accept_state(current_config_.get_current_state());
}

bool Simulator::has_applicable_transition() const {
  if (machine_ == nullptr) {
    return false;
  }
  
  std::string current_state = current_config_.get_current_state();
  char current_symbol = current_config_.get_tape().read();
  
  return machine_->get_transition(current_state, current_symbol) != nullptr;
}

const Configuration& Simulator::get_current_configuration() const {
  return current_config_;
}

const std::vector<Configuration>& Simulator::get_trace() const {
  return trace_;
}

size_t Simulator::get_step_count() const {
  return current_config_.get_step_count();
}

void Simulator::set_trace_enabled(bool enable) {
  trace_enabled_ = enable;
}

void Simulator::set_max_steps(size_t max_steps) {
  max_steps_ = max_steps;
}

std::string Simulator::get_last_error() const {
  return last_error_;
}

std::string Simulator::result_to_string(SimulationResult result) {
  switch (result) {
    case SimulationResult::ACCEPTED:
      return "ACCEPT";
    case SimulationResult::REJECTED:
      return "REJECT";
    case SimulationResult::INFINITE:
      return "INFINITE";
    case SimulationResult::ERROR:
      return "ERROR";
    default:
      return "UNKNOWN";
  }
}

void Simulator::print_trace(bool show_tape_details) const {
  std::cout << "=== Traza de Ejecución ===\n";
  for (size_t i = 0; i < trace_.size(); ++i) {
    std::cout << trace_[i].to_string(show_tape_details) << "\n";
    if (show_tape_details && i < trace_.size() - 1) {
      std::cout << "\n";  // Línea en blanco entre pasos
    }
  }
}

void Simulator::print_current_configuration(bool show_tape_details) const {
  std::cout << current_config_.to_string(show_tape_details) << std::endl;
}

bool Simulator::is_infinite_loop_detected() const {
  return is_configuration_visited();
}

void Simulator::add_to_trace() {
  if (trace_enabled_) {
    trace_.push_back(current_config_);
  }
}

std::string Simulator::get_configuration_key() const {
  return current_config_.to_compact_string();
}

bool Simulator::is_configuration_visited() const {
  std::string key = get_configuration_key();
  return visited_configurations_.find(key) != visited_configurations_.end();
}

void Simulator::mark_configuration_as_visited() {
  std::string key = get_configuration_key();
  visited_configurations_.insert(key);
}

// ===== IMPLEMENTACIÓN DE MULTISIMULATOR =====

MultiSimulator::MultiSimulator(const MultiTuringMachine* machine)
    : machine_(machine), current_config_("", 1, "", '.'), 
      trace_enabled_(false), max_steps_(1000), last_error_("") {
  if (machine_ == nullptr) {
    last_error_ = "La máquina de Turing multicinta no puede ser nullptr";
  } else {
    // Inicializar configuración con el número correcto de cintas
    current_config_ = MultiConfiguration(
      machine_->get_initial_state(),
      machine_->get_num_tapes(),
      "",
      machine_->get_blank_symbol()
    );
  }
}

MultiSimulator::~MultiSimulator() {
  // Destructor por defecto
}

SimulationResult MultiSimulator::simulate(const std::string& input_word, 
                                         bool enable_trace, 
                                         size_t max_steps) {
  // Verificar que la máquina sea válida
  if (machine_ == nullptr) {
    last_error_ = "No hay máquina de Turing multicinta asignada";
    return SimulationResult::ERROR;
  }
  
  if (!machine_->is_valid()) {
    last_error_ = "La máquina de Turing multicinta no es válida";
    return SimulationResult::ERROR;
  }
  
  // Verificar que la palabra de entrada sea válida
  if (!machine_->is_valid_input_word(input_word)) {
    last_error_ = "La palabra de entrada contiene símbolos no válidos";
    return SimulationResult::ERROR;
  }
  
  // Configurar la simulación
  trace_enabled_ = enable_trace;
  max_steps_ = max_steps;
  
  // Reiniciar el simulador
  reset(input_word);
  
  // Añadir configuración inicial a la traza
  add_to_trace();
  mark_configuration_as_visited();
  
  // Bucle principal de simulación
  while (true) {
    // Verificar límite de pasos
    if (max_steps_ > 0 && current_config_.get_step_count() >= max_steps_) {
      return SimulationResult::INFINITE;
    }
    
    // Verificar si estamos en un estado de aceptación
    if (is_accepting_state()) {
      return SimulationResult::ACCEPTED;
    }
    
    // Verificar si hay transición aplicable
    if (!has_applicable_transition()) {
      return SimulationResult::REJECTED;
    }
    
    // Ejecutar un paso
    if (!step()) {
      last_error_ = "Error al ejecutar paso de simulación multicinta";
      return SimulationResult::ERROR;
    }
    
    // Verificar bucle infinito por configuraciones repetidas
    if (is_configuration_visited()) {
      return SimulationResult::INFINITE;
    }
    
    // Marcar configuración como visitada y añadir a traza
    mark_configuration_as_visited();
    add_to_trace();
  }
}

bool MultiSimulator::step() {
  if (machine_ == nullptr) {
    return false;
  }
  
  // Obtener símbolos actuales de todas las cintas
  std::vector<char> current_symbols;
  size_t num_tapes = current_config_.get_tapes().get_num_tapes();
  
  for (size_t i = 0; i < num_tapes; ++i) {
    current_symbols.push_back(current_config_.get_tapes().read(i));
  }
  
  // Obtener transición aplicable
  std::string current_state = current_config_.get_current_state();
  const MultiTransition* transition = machine_->get_transition(current_state, current_symbols);
  
  if (transition == nullptr) {
    return false;
  }
  
  // Aplicar la transición
  // 1. Escribir los nuevos símbolos en todas las cintas
  for (size_t i = 0; i < num_tapes; ++i) {
    current_config_.get_tapes().write(i, transition->get_write_symbols()[i]);
  }
  
  // 2. Mover todos los cabezales
  for (size_t i = 0; i < num_tapes; ++i) {
    current_config_.get_tapes().move(i, transition->get_movements()[i]);
  }
  
  // 3. Cambiar al nuevo estado
  current_config_.set_current_state(transition->get_to_state());
  
  // 4. Incrementar contador de pasos
  current_config_.increment_step_count();
  
  return true;
}

void MultiSimulator::reset(const std::string& input_word) {
  if (machine_ == nullptr) {
    return;
  }
  
  // Limpiar datos de simulación anterior
  trace_.clear();
  visited_configurations_.clear();
  last_error_.clear();
  
  // Crear nueva configuración inicial
  current_config_ = MultiConfiguration(
    machine_->get_initial_state(),
    machine_->get_num_tapes(),
    input_word,
    machine_->get_blank_symbol()
  );
}

bool MultiSimulator::is_accepting_state() const {
  return machine_->is_accept_state(current_config_.get_current_state());
}

bool MultiSimulator::has_applicable_transition() const {
  if (machine_ == nullptr) {
    return false;
  }
  
  // Obtener símbolos actuales de todas las cintas
  std::vector<char> current_symbols;
  size_t num_tapes = current_config_.get_tapes().get_num_tapes();
  
  for (size_t i = 0; i < num_tapes; ++i) {
    current_symbols.push_back(current_config_.get_tapes().read(i));
  }
  
  return machine_->get_transition(current_config_.get_current_state(), current_symbols) != nullptr;
}

const MultiConfiguration& MultiSimulator::get_current_configuration() const {
  return current_config_;
}

const std::vector<MultiConfiguration>& MultiSimulator::get_trace() const {
  return trace_;
}

size_t MultiSimulator::get_step_count() const {
  return current_config_.get_step_count();
}

void MultiSimulator::set_trace_enabled(bool enable) {
  trace_enabled_ = enable;
}

void MultiSimulator::set_max_steps(size_t max_steps) {
  max_steps_ = max_steps;
}

std::string MultiSimulator::get_last_error() const {
  return last_error_;
}

void MultiSimulator::print_trace(bool show_tape_details) const {
  if (trace_.empty()) {
    std::cout << "No hay traza disponible. Asegúrate de habilitar la traza durante la simulación." << std::endl;
    return;
  }
  
  std::cout << "=== TRAZA DE EJECUCIÓN MULTICINTA ===" << std::endl;
  for (const auto& config : trace_) {
    std::cout << config.to_string(show_tape_details) << std::endl;
  }
  std::cout << "=== FIN DE TRAZA ===" << std::endl;
}

void MultiSimulator::print_current_configuration(bool show_tape_details) const {
  std::cout << current_config_.to_string(show_tape_details) << std::endl;
}

bool MultiSimulator::is_infinite_loop_detected() const {
  return is_configuration_visited();
}

void MultiSimulator::add_to_trace() {
  if (trace_enabled_) {
    trace_.push_back(current_config_);
  }
}

std::string MultiSimulator::get_configuration_key() const {
  return current_config_.to_compact_string();
}

bool MultiSimulator::is_configuration_visited() const {
  std::string key = get_configuration_key();
  return visited_configurations_.find(key) != visited_configurations_.end();
}

void MultiSimulator::mark_configuration_as_visited() {
  std::string key = get_configuration_key();
  visited_configurations_.insert(key);
}