#pragma once
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include "MultiTransition.hpp"

/**
 * @brief Estructura para crear hash de (estado, símbolos_leídos)
 */
struct StateSymbolsHash {
  size_t operator()(const std::pair<std::string, std::vector<char>>& p) const {
    size_t hash1 = std::hash<std::string>()(p.first);
    size_t hash2 = 0;
    for (size_t i = 0; i < p.second.size(); ++i) {
      hash2 ^= std::hash<char>()(p.second[i]) + 0x9e3779b9 + (hash2 << 6) + (hash2 >> 2);
    }
    return hash1 ^ (hash2 << 1);
  }
};

/**
 * @brief Clase que representa una Máquina de Turing multicinta
 * 
 * Una Máquina de Turing multicinta está formalmente definida por:
 * - Q: Conjunto finito de estados
 * - Σ: Alfabeto de entrada
 * - Γ: Alfabeto de la cinta (incluye Σ y el símbolo blanco)
 * - δ: Función de transición Q × Γ^k → Q × Γ^k × {L,R,S}^k (k = número de cintas)
 * - q₀: Estado inicial
 * - F: Conjunto de estados de aceptación
 * - ⊔: Símbolo blanco
 * - k: Número de cintas
 */
class MultiTuringMachine {
private:
  std::unordered_set<std::string> states_;           // Q: Estados de la máquina
  std::unordered_set<char> input_alphabet_;          // Σ: Alfabeto de entrada
  std::unordered_set<char> tape_alphabet_;           // Γ: Alfabeto de la cinta
  std::string initial_state_;                        // q₀: Estado inicial
  std::unordered_set<std::string> accept_states_;    // F: Estados de aceptación
  char blank_symbol_;                                // ⊔: Símbolo blanco
  size_t num_tapes_;                                 // k: Número de cintas
  
  // δ: Función de transición (estado, símbolos) → transición
  std::unordered_map<std::pair<std::string, std::vector<char>>, MultiTransition, StateSymbolsHash> transitions_;

public:
  /**
   * @brief Constructor con número de cintas especificado
   * @param num_tapes Número de cintas
   * @param blank_symbol Símbolo blanco de las cintas
   */
  explicit MultiTuringMachine(size_t num_tapes, char blank_symbol = '.');

  /**
   * @brief Destructor
   */
  ~MultiTuringMachine();

  // Métodos para configurar la máquina
  
  /**
   * @brief Añade un estado a la máquina
   * @param state Nombre del estado a añadir
   */
  void add_state(const std::string& state);

  /**
   * @brief Añade un símbolo al alfabeto de entrada
   * @param symbol Símbolo a añadir
   */
  void add_input_symbol(char symbol);

  /**
   * @brief Añade un símbolo al alfabeto de la cinta
   * @param symbol Símbolo a añadir
   */
  void add_tape_symbol(char symbol);

  /**
   * @brief Establece el estado inicial
   * @param state Estado inicial
   */
  void set_initial_state(const std::string& state);

  /**
   * @brief Añade un estado de aceptación
   * @param state Estado de aceptación
   */
  void add_accept_state(const std::string& state);

  /**
   * @brief Establece el símbolo blanco
   * @param symbol Símbolo blanco
   */
  void set_blank_symbol(char symbol);

  /**
   * @brief Establece el número de cintas
   * @param num_tapes Número de cintas
   */
  void set_num_tapes(size_t num_tapes);

  /**
   * @brief Añade una transición a la máquina
   * @param transition Transición a añadir
   */
  void add_transition(const MultiTransition& transition);

  /**
   * @brief Añade una transición especificando sus componentes
   * @param from_state Estado origen
   * @param read_symbols Símbolos leídos de cada cinta
   * @param to_state Estado destino
   * @param write_symbols Símbolos a escribir en cada cinta
   * @param movements Movimientos para cada cinta
   */
  void add_transition(const std::string& from_state,
                     const std::vector<char>& read_symbols,
                     const std::string& to_state,
                     const std::vector<char>& write_symbols,
                     const std::vector<Movement>& movements);

  // Métodos de consulta

  /**
   * @brief Obtiene el conjunto de estados
   * @return Conjunto de estados
   */
  const std::unordered_set<std::string>& get_states() const;

  /**
   * @brief Obtiene el alfabeto de entrada
   * @return Alfabeto de entrada
   */
  const std::unordered_set<char>& get_input_alphabet() const;

  /**
   * @brief Obtiene el alfabeto de la cinta
   * @return Alfabeto de la cinta
   */
  const std::unordered_set<char>& get_tape_alphabet() const;

  /**
   * @brief Obtiene el estado inicial
   * @return Estado inicial
   */
  const std::string& get_initial_state() const;

  /**
   * @brief Obtiene los estados de aceptación
   * @return Estados de aceptación
   */
  const std::unordered_set<std::string>& get_accept_states() const;

  /**
   * @brief Obtiene el símbolo blanco
   * @return Símbolo blanco
   */
  char get_blank_symbol() const;

  /**
   * @brief Obtiene el número de cintas
   * @return Número de cintas
   */
  size_t get_num_tapes() const;

  /**
   * @brief Obtiene la transición para un estado y símbolos dados
   * @param state Estado actual
   * @param symbols Símbolos leídos de cada cinta
   * @return Puntero a la transición (nullptr si no existe)
   */
  const MultiTransition* get_transition(const std::string& state,
                                       const std::vector<char>& symbols) const;

  /**
   * @brief Obtiene todas las transiciones de la máquina
   * @return Vector con todas las transiciones
   */
  std::vector<MultiTransition> get_all_transitions() const;

  // Métodos de validación

  /**
   * @brief Verifica si la máquina está bien formada
   * @return true si la máquina es válida
   */
  bool is_valid() const;

  /**
   * @brief Verifica si un estado es de aceptación
   * @param state Estado a verificar
   * @return true si es estado de aceptación
   */
  bool is_accept_state(const std::string& state) const;

  /**
   * @brief Verifica si un símbolo pertenece al alfabeto de entrada
   * @param symbol Símbolo a verificar
   * @return true si pertenece al alfabeto de entrada
   */
  bool is_input_symbol(char symbol) const;

  /**
   * @brief Verifica si un símbolo pertenece al alfabeto de la cinta
   * @param symbol Símbolo a verificar
   * @return true si pertenece al alfabeto de la cinta
   */
  bool is_tape_symbol(char symbol) const;

  /**
   * @brief Verifica si una palabra pertenece al alfabeto de entrada
   * @param word Palabra a verificar
   * @return true si todos los símbolos pertenecen al alfabeto de entrada
   */
  bool is_valid_input_word(const std::string& word) const;

  // Métodos de información

  /**
   * @brief Obtiene información resumida de la máquina
   * @return String con información de la máquina
   */
  std::string get_info() const;

  /**
   * @brief Limpia toda la definición de la máquina
   */
  void clear();

  /**
   * @brief Obtiene el número total de transiciones
   * @return Número de transiciones
   */
  size_t get_transition_count() const;

  /**
   * @brief Convierte una máquina monocinta a multicinta
   * @param mono_machine Máquina monocinta original
   * @param num_tapes Número de cintas para la nueva máquina
   * @return Máquina multicinta equivalente
   */
  static MultiTuringMachine from_mono_machine(const class TuringMachine& mono_machine,
                                             size_t num_tapes = 1);
};