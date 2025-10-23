#pragma once
#include "Transition.hpp"
#include <vector>
#include <string>

/**
 * @brief Clase que representa una transición para Máquina de Turing multicinta
 * 
 * Una transición multicinta especifica:
 * - Estado origen
 * - Símbolos leídos de cada cinta
 * - Estado destino  
 * - Símbolos a escribir en cada cinta
 * - Movimientos para cada cinta
 */
class MultiTransition {
private:
  std::string from_state_;                  // Estado origen
  std::vector<char> read_symbols_;          // Símbolos leídos de cada cinta
  std::string to_state_;                    // Estado destino
  std::vector<char> write_symbols_;         // Símbolos a escribir en cada cinta
  std::vector<Movement> movements_;         // Movimientos para cada cinta

public:
  /**
   * @brief Constructor de la transición multicinta
   * @param from_state Estado origen
   * @param read_symbols Vector de símbolos leídos (uno por cinta)
   * @param to_state Estado destino
   * @param write_symbols Vector de símbolos a escribir (uno por cinta)
   * @param movements Vector de movimientos (uno por cinta)
   */
  MultiTransition(const std::string& from_state,
                  const std::vector<char>& read_symbols,
                  const std::string& to_state,
                  const std::vector<char>& write_symbols,
                  const std::vector<Movement>& movements);

  /**
   * @brief Constructor por defecto
   */
  MultiTransition();

  /**
   * @brief Destructor
   */
  ~MultiTransition();

  /**
   * @brief Constructor de copia
   * @param other Transición a copiar
   */
  MultiTransition(const MultiTransition& other);

  /**
   * @brief Operador de asignación
   * @param other Transición a asignar
   * @return Referencia a esta transición
   */
  MultiTransition& operator=(const MultiTransition& other);

  // Métodos getter para acceso controlado
  const std::string& get_from_state() const;
  const std::vector<char>& get_read_symbols() const;
  const std::string& get_to_state() const;
  const std::vector<char>& get_write_symbols() const;
  const std::vector<Movement>& get_movements() const;

  /**
   * @brief Obtiene el símbolo leído de una cinta específica
   * @param tape_index Índice de la cinta
   * @return Símbolo leído
   */
  char get_read_symbol(size_t tape_index) const;

  /**
   * @brief Obtiene el símbolo a escribir en una cinta específica
   * @param tape_index Índice de la cinta
   * @return Símbolo a escribir
   */
  char get_write_symbol(size_t tape_index) const;

  /**
   * @brief Obtiene el movimiento para una cinta específica
   * @param tape_index Índice de la cinta
   * @return Movimiento
   */
  Movement get_movement(size_t tape_index) const;

  /**
   * @brief Obtiene el número de cintas que maneja esta transición
   * @return Número de cintas
   */
  size_t get_num_tapes() const;

  /**
   * @brief Verifica si la transición es aplicable dados un estado y símbolos actuales
   * @param current_state Estado actual de la máquina
   * @param current_symbols Símbolos actuales en cada cinta
   * @return true si la transición es aplicable
   */
  bool is_applicable(const std::string& current_state, 
                     const std::vector<char>& current_symbols) const;

  /**
   * @brief Obtiene una representación en cadena de la transición
   * Formato: "from_state read1,read2,...,readN to_state write1,write2,...,writeN move1,move2,...,moveN"
   * @return String con la representación de la transición
   */
  std::string to_string() const;

  /**
   * @brief Verifica si la transición está bien formada
   * Todos los vectores deben tener el mismo tamaño
   * @return true si está bien formada
   */
  bool is_well_formed() const;

  /**
   * @brief Crea una transición multicinta a partir de una transición monocinta
   * @param mono_transition Transición monocinta
   * @param num_tapes Número total de cintas (las otras no se modifican)
   * @param target_tape Índice de la cinta que se modifica
   * @return Transición multicinta equivalente
   */
  static MultiTransition from_mono_transition(const Transition& mono_transition,
                                              size_t num_tapes,
                                              size_t target_tape = 0);

  /**
   * @brief Convierte un vector de caracteres de movimiento a vector de Movement
   * @param movement_chars Vector de caracteres ('L', 'R', 'S')
   * @return Vector de movimientos
   */
  static std::vector<Movement> chars_to_movements(const std::vector<char>& movement_chars);

  /**
   * @brief Convierte un vector de Movement a vector de caracteres
   * @param movements Vector de movimientos
   * @return Vector de caracteres
   */
  static std::vector<char> movements_to_chars(const std::vector<Movement>& movements);
};