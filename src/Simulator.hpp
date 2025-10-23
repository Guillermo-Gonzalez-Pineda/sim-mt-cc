#pragma once
#include <string>
#include <vector>
#include <unordered_set>
#include "TuringMachine.hpp"
#include "Configuration.hpp"
#include "MultiTuringMachine.hpp"
#include "MultiConfiguration.hpp"

/**
 * @brief Enumeración para los posibles resultados de la simulación
 */
enum class SimulationResult {
  ACCEPTED,    // La palabra fue aceptada
  REJECTED,    // La palabra fue rechazada (estado no de aceptación)
  INFINITE,    // La simulación alcanzó el límite de pasos (posible bucle infinito)
  ERROR        // Error durante la simulación
};

/**
 * @brief Clase para simular la ejecución de una Máquina de Turing
 * 
 * El simulador ejecuta la máquina paso a paso y puede detectar:
 * - Aceptación (estado final de aceptación)
 * - Rechazo (estado final no de aceptación)
 * - Bucles infinitos (mediante límite de pasos o detección de configuraciones repetidas)
 */
class Simulator {
private:
  const TuringMachine* machine_;     // Máquina de Turing a simular
  Configuration current_config_;     // Configuración actual
  std::vector<Configuration> trace_; // Traza de ejecución (solo si está habilitada)
  bool trace_enabled_;               // Si la traza está habilitada
  size_t max_steps_;                 // Límite máximo de pasos (0 = sin límite)
  std::string last_error_;           // Último error ocurrido
  
  // Para detección de bucles infinitos
  std::unordered_set<std::string> visited_configurations_;

public:
  /**
   * @brief Constructor del simulador
   * @param machine Puntero a la máquina de Turing a simular
   */
  explicit Simulator(const TuringMachine* machine);

  /**
   * @brief Destructor
   */
  ~Simulator();

  /**
   * @brief Simula la ejecución de la máquina con una palabra de entrada
   * @param input_word Palabra de entrada
   * @param enable_trace Si habilitar la traza de ejecución
   * @param max_steps Límite máximo de pasos (0 = sin límite)
   * @return Resultado de la simulación
   */
  SimulationResult simulate(const std::string& input_word, 
                           bool enable_trace = false, 
                           size_t max_steps = 1000);

  /**
   * @brief Ejecuta un solo paso de la simulación
   * @return true si se pudo ejecutar el paso, false si no hay transición aplicable
   */
  bool step();

  /**
   * @brief Reinicia el simulador con una nueva palabra de entrada
   * @param input_word Nueva palabra de entrada
   */
  void reset(const std::string& input_word = "");

  /**
   * @brief Verifica si la configuración actual es de aceptación
   * @return true si el estado actual es de aceptación
   */
  bool is_accepting_state() const;

  /**
   * @brief Verifica si hay transición aplicable desde la configuración actual
   * @return true si hay transición aplicable
   */
  bool has_applicable_transition() const;

  /**
   * @brief Obtiene la configuración actual
   * @return Referencia constante a la configuración actual
   */
  const Configuration& get_current_configuration() const;

  /**
   * @brief Obtiene la traza completa de ejecución
   * Solo disponible si se habilitó la traza durante la simulación
   * @return Vector con todas las configuraciones visitadas
   */
  const std::vector<Configuration>& get_trace() const;

  /**
   * @brief Obtiene el número de pasos ejecutados en la simulación actual
   * @return Número de pasos
   */
  size_t get_step_count() const;

  /**
   * @brief Habilita o deshabilita la traza de ejecución
   * @param enable Si habilitar la traza
   */
  void set_trace_enabled(bool enable);

  /**
   * @brief Establece el límite máximo de pasos
   * @param max_steps Nuevo límite (0 = sin límite)
   */
  void set_max_steps(size_t max_steps);

  /**
   * @brief Obtiene el último error ocurrido
   * @return String con el mensaje de error
   */
  std::string get_last_error() const;

  /**
   * @brief Convierte el resultado de simulación a string
   * @param result Resultado a convertir
   * @return String representando el resultado
   */
  static std::string result_to_string(SimulationResult result);

  /**
   * @brief Imprime la traza de ejecución
   * @param show_tape_details Si mostrar detalles de la cinta
   */
  void print_trace(bool show_tape_details = true) const;

  /**
   * @brief Imprime la configuración actual
   * @param show_tape_details Si mostrar detalles de la cinta
   */
  void print_current_configuration(bool show_tape_details = true) const;

  /**
   * @brief Verifica si se detectó un posible bucle infinito
   * Esto ocurre cuando se visita una configuración que ya se había visitado antes
   * @return true si se detectó bucle infinito
   */
  bool is_infinite_loop_detected() const;

private:
  /**
   * @brief Añade la configuración actual a la traza (si está habilitada)
   */
  void add_to_trace();

  /**
   * @brief Genera una clave única para la configuración actual
   * Usada para detectar configuraciones repetidas
   * @return String con la clave de la configuración
   */
  std::string get_configuration_key() const;

  /**
   * @brief Verifica si la configuración actual ya fue visitada
   * @return true si ya fue visitada
   */
  bool is_configuration_visited() const;

  /**
   * @brief Marca la configuración actual como visitada
   */
  void mark_configuration_as_visited();
};

/**
 * @brief Clase para simular la ejecución de una Máquina de Turing Multicinta
 * 
 * Extiende la funcionalidad del simulador básico para soportar máquinas
 * con múltiples cintas independientes.
 */
class MultiSimulator {
private:
  const MultiTuringMachine* machine_;     // Máquina de Turing multicinta a simular
  MultiConfiguration current_config_;     // Configuración actual
  std::vector<MultiConfiguration> trace_; // Traza de ejecución (solo si está habilitada)
  bool trace_enabled_;                    // Si la traza está habilitada
  size_t max_steps_;                      // Límite máximo de pasos (0 = sin límite)
  std::string last_error_;                // Último error ocurrido
  
  // Para detección de bucles infinitos
  std::unordered_set<std::string> visited_configurations_;

public:
  /**
   * @brief Constructor del simulador multicinta
   * @param machine Puntero a la máquina de Turing multicinta a simular
   */
  explicit MultiSimulator(const MultiTuringMachine* machine);

  /**
   * @brief Destructor
   */
  ~MultiSimulator();

  /**
   * @brief Simula la ejecución de la máquina multicinta con una palabra de entrada
   * @param input_word Palabra de entrada (se coloca en la primera cinta)
   * @param enable_trace Si habilitar la traza de ejecución
   * @param max_steps Límite máximo de pasos (0 = sin límite)
   * @return Resultado de la simulación
   */
  SimulationResult simulate(const std::string& input_word, 
                           bool enable_trace = false, 
                           size_t max_steps = 1000);

  /**
   * @brief Ejecuta un solo paso de la simulación multicinta
   * @return true si se pudo ejecutar el paso, false si no hay transición aplicable
   */
  bool step();

  /**
   * @brief Reinicia el simulador con una nueva palabra de entrada
   * @param input_word Nueva palabra de entrada
   */
  void reset(const std::string& input_word = "");

  /**
   * @brief Verifica si la configuración actual es de aceptación
   * @return true si el estado actual es de aceptación
   */
  bool is_accepting_state() const;

  /**
   * @brief Verifica si hay transición aplicable desde la configuración actual
   * @return true si hay transición aplicable
   */
  bool has_applicable_transition() const;

  /**
   * @brief Obtiene la configuración actual
   * @return Referencia constante a la configuración actual
   */
  const MultiConfiguration& get_current_configuration() const;

  /**
   * @brief Obtiene la traza completa de ejecución
   * Solo disponible si se habilitó la traza durante la simulación
   * @return Vector con todas las configuraciones visitadas
   */
  const std::vector<MultiConfiguration>& get_trace() const;

  /**
   * @brief Obtiene el número de pasos ejecutados en la simulación actual
   * @return Número de pasos
   */
  size_t get_step_count() const;

  /**
   * @brief Habilita o deshabilita la traza de ejecución
   * @param enable Si habilitar la traza
   */
  void set_trace_enabled(bool enable);

  /**
   * @brief Establece el límite máximo de pasos
   * @param max_steps Nuevo límite (0 = sin límite)
   */
  void set_max_steps(size_t max_steps);

  /**
   * @brief Obtiene el último error ocurrido
   * @return String con el mensaje de error
   */
  std::string get_last_error() const;

  /**
   * @brief Imprime la traza de ejecución
   * @param show_tape_details Si mostrar detalles de las cintas
   */
  void print_trace(bool show_tape_details = true) const;

  /**
   * @brief Imprime la configuración actual
   * @param show_tape_details Si mostrar detalles de las cintas
   */
  void print_current_configuration(bool show_tape_details = true) const;

  /**
   * @brief Verifica si se detectó un posible bucle infinito
   * @return true si se detectó bucle infinito
   */
  bool is_infinite_loop_detected() const;

private:
  /**
   * @brief Añade la configuración actual a la traza (si está habilitada)
   */
  void add_to_trace();

  /**
   * @brief Genera una clave única para la configuración actual
   * Usada para detectar configuraciones repetidas
   * @return String con la clave de la configuración
   */
  std::string get_configuration_key() const;

  /**
   * @brief Verifica si la configuración actual ya fue visitada
   * @return true si ya fue visitada
   */
  bool is_configuration_visited() const;

  /**
   * @brief Marca la configuración actual como visitada
   */
  void mark_configuration_as_visited();
};