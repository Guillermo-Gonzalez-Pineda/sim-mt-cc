#pragma once
#include "Tape.hpp"
#include "Transition.hpp"
#include <vector>
#include <string>

/**
 * @brief Clase que representa múltiples cintas para una Máquina de Turing multicinta
 * 
 * Esta clase gestiona un conjunto de cintas independientes, cada una con su propio
 * cabezal de lectura/escritura. Permite operaciones simultáneas en todas las cintas.
 */
class MultiTape {
private:
  std::vector<Tape> tapes_;        // Vector de cintas
  size_t num_tapes_;               // Número de cintas

public:
  /**
   * @brief Constructor que crea múltiples cintas
   * @param num_tapes Número de cintas a crear
   * @param blank_symbol Símbolo blanco para todas las cintas
   */
  explicit MultiTape(size_t num_tapes, char blank_symbol = '.');

  /**
   * @brief Constructor que inicializa la primera cinta con una palabra
   * @param num_tapes Número de cintas a crear
   * @param input_word Palabra inicial para la primera cinta
   * @param blank_symbol Símbolo blanco para todas las cintas
   */
  MultiTape(size_t num_tapes, const std::string& input_word, char blank_symbol = '.');

  /**
   * @brief Constructor de copia
   * @param other MultiTape a copiar
   */
  MultiTape(const MultiTape& other);

  /**
   * @brief Operador de asignación
   * @param other MultiTape a asignar
   * @return Referencia a este MultiTape
   */
  MultiTape& operator=(const MultiTape& other);

  /**
   * @brief Destructor
   */
  ~MultiTape();

  /**
   * @brief Lee el símbolo bajo el cabezal de una cinta específica
   * @param tape_index Índice de la cinta (0-based)
   * @return Símbolo leído
   */
  char read(size_t tape_index) const;

  /**
   * @brief Escribe un símbolo en una cinta específica
   * @param tape_index Índice de la cinta
   * @param symbol Símbolo a escribir
   */
  void write(size_t tape_index, char symbol);

  /**
   * @brief Mueve el cabezal de una cinta específica
   * @param tape_index Índice de la cinta
   * @param movement Movimiento a realizar
   */
  void move(size_t tape_index, Movement movement);

  /**
   * @brief Obtiene el número de cintas
   * @return Número de cintas
   */
  size_t get_num_tapes() const;

  /**
   * @brief Obtiene la posición del cabezal de una cinta
   * @param tape_index Índice de la cinta
   * @return Posición del cabezal
   */
  int get_head_position(size_t tape_index) const;

  /**
   * @brief Establece la posición del cabezal de una cinta
   * @param tape_index Índice de la cinta
   * @param position Nueva posición
   */
  void set_head_position(size_t tape_index, int position);

  /**
   * @brief Obtiene el símbolo blanco
   * @return Símbolo blanco
   */
  char get_blank_symbol() const;

  /**
   * @brief Reinicia todas las cintas
   * @param input_word Nueva palabra para la primera cinta (las demás quedan vacías)
   */
  void reset(const std::string& input_word = "");

  /**
   * @brief Obtiene representación visual de todas las cintas
   * @param window_size Tamaño de ventana alrededor de cada cabezal
   * @return String con el estado de todas las cintas
   */
  std::string to_string(int window_size = 10) const;

  /**
   * @brief Obtiene el contenido de una cinta específica
   * @param tape_index Índice de la cinta
   * @return Contenido de la cinta
   */
  std::string get_tape_content(size_t tape_index) const;

  /**
   * @brief Obtiene una referencia a una cinta específica
   * @param tape_index Índice de la cinta
   * @return Referencia a la cinta
   */
  Tape& get_tape(size_t tape_index);

  /**
   * @brief Obtiene una referencia constante a una cinta específica
   * @param tape_index Índice de la cinta
   * @return Referencia constante a la cinta
   */
  const Tape& get_tape(size_t tape_index) const;

  /**
   * @brief Lee los símbolos de todas las cintas
   * @return Vector con los símbolos leídos de cada cinta
   */
  std::vector<char> read_all() const;

  /**
   * @brief Verifica si el índice de cinta es válido
   * @param tape_index Índice a verificar
   * @return true si es válido
   */
  bool is_valid_tape_index(size_t tape_index) const;
};