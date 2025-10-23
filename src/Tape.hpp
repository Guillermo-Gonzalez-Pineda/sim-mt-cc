#pragma once
#include <string>
#include <unordered_map>

/**
 * @brief Clase que representa la cinta infinita de la Máquina de Turing
 * 
 * La cinta se implementa usando un mapa disperso para optimizar el uso de memoria.
 * Solo se almacenan las posiciones que contienen símbolos diferentes al símbolo blanco.
 * Las posiciones no especificadas se consideran que contienen el símbolo blanco.
 */
class Tape {
private:
  std::unordered_map<int, char> cells_;  // Celdas de la cinta (posición -> símbolo)
  int head_position_;                    // Posición actual del cabezal
  char blank_symbol_;                    // Símbolo blanco de la cinta

public:
  /**
   * @brief Constructor de la cinta
   * @param blank_symbol Símbolo que representa una celda vacía (por defecto '.')
   */
  explicit Tape(char blank_symbol = '.');

  /**
   * @brief Constructor que inicializa la cinta con una cadena
   * @param input_string Cadena inicial a escribir en la cinta
   * @param blank_symbol Símbolo blanco (por defecto '.')
   */
  Tape(const std::string& input_string, char blank_symbol = '.');

  /**
   * @brief Destructor
   */
  ~Tape();

  /**
   * @brief Lee el símbolo en la posición actual del cabezal
   * @return Símbolo en la posición del cabezal
   */
  char read() const;

  /**
   * @brief Escribe un símbolo en la posición actual del cabezal
   * @param symbol Símbolo a escribir
   */
  void write(char symbol);

  /**
   * @brief Mueve el cabezal una posición hacia la izquierda
   */
  void move_left();

  /**
   * @brief Mueve el cabezal una posición hacia la derecha
   */
  void move_right();

  /**
   * @brief Obtiene la posición actual del cabezal
   * @return Posición del cabezal
   */
  int get_head_position() const;

  /**
   * @brief Establece la posición del cabezal
   * @param position Nueva posición del cabezal
   */
  void set_head_position(int position);

  /**
   * @brief Obtiene el símbolo blanco de la cinta
   * @return Símbolo blanco
   */
  char get_blank_symbol() const;

  /**
   * @brief Reinicia la cinta con una nueva cadena de entrada
   * @param input_string Nueva cadena de entrada
   */
  void reset(const std::string& input_string);

  /**
   * @brief Obtiene una representación visual de la cinta
   * @param window_size Tamaño de la ventana alrededor del cabezal
   * @return String con la representación visual de la cinta
   */
  std::string to_string(int window_size = 10) const;

  /**
   * @brief Obtiene el contenido de la cinta como cadena
   * Solo incluye los símbolos no blancos, desde la posición más a la izquierda
   * hasta la más a la derecha que contengan símbolos.
   * @return Contenido de la cinta como string
   */
  std::string get_content() const;

  /**
   * @brief Verifica si la cinta está vacía (solo contiene símbolos blancos)
   * @return true si la cinta está vacía
   */
  bool is_empty() const;
};