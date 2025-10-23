#pragma once
#include <string>

/**
 * @brief Enumeración para los movimientos del cabezal de la cinta
 */
enum class Movement {
  LEFT,   // Movimiento hacia la izquierda (L)
  RIGHT,  // Movimiento hacia la derecha (R)
  STAY    // Sin movimiento (S)
};

/**
 * @brief Clase que representa una transición de la Máquina de Turing
 * 
 * Una transición está definida por:
 * - Estado origen
 * - Símbolo leído de la cinta
 * - Estado destino
 * - Símbolo a escribir en la cinta
 * - Movimiento del cabezal
 */
class Transition {
private:
  std::string from_state_;    // Estado origen de la transición
  char read_symbol_;          // Símbolo leído de la cinta
  std::string to_state_;      // Estado destino de la transición
  char write_symbol_;         // Símbolo a escribir en la cinta
  Movement movement_;         // Movimiento del cabezal

public:
  /**
   * @brief Constructor de la transición
   * @param from_state Estado origen
   * @param read_symbol Símbolo leído de la cinta
   * @param to_state Estado destino
   * @param write_symbol Símbolo a escribir en la cinta
   * @param movement Movimiento del cabezal
   */
  Transition(const std::string& from_state, char read_symbol, 
             const std::string& to_state, char write_symbol, 
             Movement movement);

  /**
   * @brief Constructor por defecto
   */
  Transition();

  /**
   * @brief Destructor
   */
  ~Transition();

  // Métodos getter para acceso controlado a los atributos
  const std::string& get_from_state() const;
  char get_read_symbol() const;
  const std::string& get_to_state() const;
  char get_write_symbol() const;
  Movement get_movement() const;

  /**
   * @brief Convierte el movimiento a su representación como carácter
   * @return 'L' para LEFT, 'R' para RIGHT, 'S' para STAY
   */
  char movement_to_char() const;

  /**
   * @brief Convierte un carácter a movimiento
   * @param c Carácter que representa el movimiento ('L', 'R', 'S')
   * @return Movimiento correspondiente
   */
  static Movement char_to_movement(char c);

  /**
   * @brief Obtiene una representación en cadena de la transición
   * @return String con el formato "from_state read_symbol to_state write_symbol movement"
   */
  std::string to_string() const;

  /**
   * @brief Verifica si la transición es aplicable dado un estado y símbolo
   * @param current_state Estado actual de la máquina
   * @param current_symbol Símbolo actual en la cinta
   * @return true si la transición es aplicable
   */
  bool is_applicable(const std::string& current_state, char current_symbol) const;
};