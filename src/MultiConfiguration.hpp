#pragma once
#include <string>
#include "MultiTape.hpp"

/**
 * @brief Clase que representa una configuración instantánea de la Máquina de Turing multicinta
 * 
 * Una configuración multicinta está compuesta por:
 * - El estado actual de la máquina
 * - El contenido de todas las cintas
 * - Las posiciones de todos los cabezales
 */
class MultiConfiguration {
private:
  std::string current_state_;  // Estado actual de la máquina
  MultiTape tapes_;           // Múltiples cintas de la máquina
  size_t step_count_;         // Número de pasos ejecutados hasta llegar a esta configuración

public:
  /**
   * @brief Constructor de la configuración multicinta
   * @param initial_state Estado inicial de la máquina
   * @param num_tapes Número de cintas
   * @param input_string Cadena de entrada a colocar en la primera cinta
   * @param blank_symbol Símbolo blanco de las cintas
   */
  MultiConfiguration(const std::string& initial_state, 
                     size_t num_tapes,
                     const std::string& input_string = "", 
                     char blank_symbol = '.');

  /**
   * @brief Constructor de copia
   * @param other Configuración a copiar
   */
  MultiConfiguration(const MultiConfiguration& other);

  /**
   * @brief Operador de asignación
   * @param other Configuración a asignar
   * @return Referencia a esta configuración
   */
  MultiConfiguration& operator=(const MultiConfiguration& other);

  /**
   * @brief Destructor
   */
  ~MultiConfiguration();

  /**
   * @brief Obtiene el estado actual de la máquina
   * @return Estado actual
   */
  const std::string& get_current_state() const;

  /**
   * @brief Establece el estado actual de la máquina
   * @param state Nuevo estado
   */
  void set_current_state(const std::string& state);

  /**
   * @brief Obtiene una referencia a las cintas
   * @return Referencia a las cintas
   */
  MultiTape& get_tapes();

  /**
   * @brief Obtiene una referencia constante a las cintas
   * @return Referencia constante a las cintas
   */
  const MultiTape& get_tapes() const;

  /**
   * @brief Obtiene el número de pasos ejecutados
   * @return Número de pasos
   */
  size_t get_step_count() const;

  /**
   * @brief Incrementa el contador de pasos
   */
  void increment_step_count();

  /**
   * @brief Establece el número de pasos
   * @param steps Nuevo número de pasos
   */
  void set_step_count(size_t steps);

  /**
   * @brief Obtiene una representación en cadena de la configuración
   * @param show_tape_window Si mostrar una ventana de las cintas
   * @param window_size Tamaño de la ventana alrededor de cada cabezal
   * @return String con la representación de la configuración
   */
  std::string to_string(bool show_tape_window = true, int window_size = 12) const;

  /**
   * @brief Obtiene una representación compacta de la configuración
   * Formato: "estado|pos1,pos2,...|contenido1|contenido2|..."
   * @return String con la representación compacta
   */
  std::string to_compact_string() const;

  /**
   * @brief Verifica si dos configuraciones son equivalentes
   * @param other Configuración a comparar
   * @return true si las configuraciones son equivalentes
   */
  bool is_equivalent(const MultiConfiguration& other) const;

  /**
   * @brief Operador de igualdad
   * @param other Configuración a comparar
   * @return true si las configuraciones son iguales
   */
  bool operator==(const MultiConfiguration& other) const;

  /**
   * @brief Reinicia la configuración con nuevos valores
   * @param initial_state Nuevo estado inicial
   * @param input_string Nueva cadena de entrada para la primera cinta
   */
  void reset(const std::string& initial_state, const std::string& input_string = "");
};