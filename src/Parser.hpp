#pragma once
#include <string>
#include <fstream>
#include <vector>
#include "TuringMachine.hpp"
#include "MultiTuringMachine.hpp"

/**
 * @brief Clase para parsear archivos de definición de Máquinas de Turing
 * 
 * El formato esperado del archivo es:
 * # Comentarios (líneas que empiezan con #)
 * estados_separados_por_espacios
 * alfabeto_entrada_separado_por_espacios
 * alfabeto_cinta_separado_por_espacios
 * estado_inicial
 * símbolo_blanco
 * estados_aceptación_separados_por_espacios
 * transiciones (una por línea): estado_origen símbolo_leído estado_destino símbolo_escrito movimiento
 */
class Parser {
private:
  /**
   * @brief Elimina espacios en blanco al inicio y final de una cadena
   * @param str Cadena a procesar
   * @return Cadena sin espacios al inicio y final
   */
  static std::string trim(const std::string& str);

  /**
   * @brief Divide una cadena en tokens usando espacios como separadores
   * @param str Cadena a dividir
   * @return Vector de tokens
   */
  static std::vector<std::string> split(const std::string& str);

  /**
   * @brief Verifica si una línea es un comentario (empieza con #)
   * @param line Línea a verificar
   * @return true si es comentario
   */
  static bool is_comment(const std::string& line);

  /**
   * @brief Verifica si una línea está vacía (solo espacios)
   * @param line Línea a verificar
   * @return true si está vacía
   */
  static bool is_empty_line(const std::string& line);

  /**
   * @brief Parsea una línea de transición monocinta
   * @param line Línea con formato: estado_origen símbolo_leído estado_destino símbolo_escrito movimiento
   * @param line_number Número de línea (para errores)
   * @return Transición parseada
   */
  static Transition parse_transition(const std::string& line, int line_number);

  /**
   * @brief Parsea una línea de transición multicinta
   * @param line Línea con formato: estado_origen símb1,símb2,... estado_destino esc1,esc2,... mov1,mov2,...
   * @param line_number Número de línea (para errores)
   * @param num_tapes Número de cintas esperado
   * @return Transición multicinta parseada
   */
  static MultiTransition parse_multi_transition(const std::string& line, int line_number, size_t num_tapes);

  /**
   * @brief Convierte un string de un carácter a char, manejando casos especiales
   * @param str String a convertir
   * @param context Contexto para mensajes de error
   * @return Carácter convertido
   */
  static char string_to_char(const std::string& str, const std::string& context);

public:
  /**
   * @brief Carga una máquina de Turing desde un archivo
   * @param filename Ruta del archivo a cargar
   * @param machine Máquina de Turing donde cargar la definición
   * @return true si la carga fue exitosa
   */
  static bool load_from_file(const std::string& filename, TuringMachine& machine);

  /**
   * @brief Carga una máquina de Turing multicinta desde un archivo
   * @param filename Ruta del archivo a cargar
   * @param machine Máquina de Turing multicinta donde cargar la definición
   * @return true si la carga fue exitosa
   */
  static bool load_multi_from_file(const std::string& filename, MultiTuringMachine& machine);

  /**
   * @brief Detecta automáticamente el tipo de máquina y carga la apropiada
   * @param filename Ruta del archivo a cargar
   * @param mono_machine Máquina monocinta (se usa si es el tipo correcto)
   * @param multi_machine Máquina multicinta (se usa si es el tipo correcto)
   * @param is_multicinta Salida: true si se cargó como multicinta
   * @return true si la carga fue exitosa
   */
  static bool load_auto_detect(const std::string& filename,
                              TuringMachine& mono_machine,
                              MultiTuringMachine& multi_machine,
                              bool& is_multicinta);

  /**
   * @brief Carga una máquina de Turing desde un flujo de entrada
   * @param input Flujo de entrada
   * @param machine Máquina de Turing donde cargar la definición
   * @return true si la carga fue exitosa
   */
  static bool load_from_stream(std::istream& input, TuringMachine& machine);

  /**
   * @brief Guarda una máquina de Turing en un archivo
   * @param filename Ruta del archivo donde guardar
   * @param machine Máquina de Turing a guardar
   * @return true si el guardado fue exitoso
   */
  static bool save_to_file(const std::string& filename, const TuringMachine& machine);

  /**
   * @brief Valida el formato de un archivo antes de parsearlo
   * @param filename Ruta del archivo a validar
   * @return true si el formato es válido
   */
  static bool validate_file_format(const std::string& filename);

  /**
   * @brief Obtiene información de error del último parseo fallido
   * @return String con información del error
   */
  static std::string get_last_error();

private:
  static std::string last_error_;  // Último error ocurrido durante el parseo
};