#pragma once
#include <string>
#include "Tape.hpp"

/**
 * @brief Clase que representa una configuración instantánea de la Máquina de Turing
 * 
 * Una configuración está compuesta por:
 * - El estado actual de la máquina
 * - El contenido de la cinta
 * - La posición del cabezal en la cinta
 */
class Configuration {
private:
  std::string current_state_;  // Estado actual de la máquina
  Tape tape_;                  // Cinta de la máquina
  size_t step_count_;          // Número de pasos ejecutados hasta llegar a esta configuración

public:
  /**
   * @brief Constructor de la configuración
   * @param initial_state Estado inicial de la máquina
   * @param input_string Cadena de entrada a colocar en la cinta
   * @param blank_symbol Símbolo blanco de la cinta
   */
  Configuration(const std::string& initial_state, 
                const std::string& input_string = "", 
                char blank_symbol = '.');

  /**
   * @brief Constructor de copia
   * @param other Configuración a copiar
   */
  Configuration(const Configuration& other);

  /**
   * @brief Operador de asignación
   * @param other Configuración a asignar
   * @return Referencia a esta configuración
   */
  Configuration& operator=(const Configuration& other);

  /**
   * @brief Destructor
   */
  ~Configuration();

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
   * @brief Obtiene una referencia a la cinta
   * @return Referencia a la cinta
   */
  Tape& get_tape();

  /**
   * @brief Obtiene una referencia constante a la cinta
   * @return Referencia constante a la cinta
   */
  const Tape& get_tape() const;

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
   * @param show_tape_window Si mostrar una ventana de la cinta
   * @param window_size Tamaño de la ventana alrededor del cabezal
   * @return String con la representación de la configuración
   */
  std::string to_string(bool show_tape_window = true, int window_size = 10) const;

  /**
   * @brief Obtiene una representación compacta de la configuración
   * Formato: "estado|posición_cabezal|contenido_cinta"
   * @return String con la representación compacta
   */
  std::string to_compact_string() const;

  /**
   * @brief Verifica si dos configuraciones son equivalentes
   * Dos configuraciones son equivalentes si tienen el mismo estado,
   * la misma posición del cabezal y el mismo contenido en la cinta
   * @param other Configuración a comparar
   * @return true si las configuraciones son equivalentes
   */
  bool is_equivalent(const Configuration& other) const;

  /**
   * @brief Operador de igualdad
   * @param other Configuración a comparar
   * @return true si las configuraciones son iguales
   */
  bool operator==(const Configuration& other) const;

  /**
   * @brief Reinicia la configuración con nuevos valores
   * @param initial_state Nuevo estado inicial
   * @param input_string Nueva cadena de entrada
   */
  void reset(const std::string& initial_state, const std::string& input_string = "");
};