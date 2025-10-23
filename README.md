# Simulador de Máquina de Turing (Monocinta y Multicinta)

Este proyecto implementa un simulador completo de Máquinas de Turing en C++ que soporta tanto **máquinas monocinta** como **máquinas multicinta**, siguiendo principios de programación orientada a objetos.

## Estructura del Proyecto

```
MaquinaTuring/
├── src/                    # Código fuente
│   ├── main.cpp           # Programa principal
│   ├── TuringMachine.*    # Definición formal de MT monocinta
│   ├── MultiTuringMachine.* # Definición formal de MT multicinta
│   ├── Transition.*       # Representación de transiciones monocinta
│   ├── MultiTransition.*  # Representación de transiciones multicinta
│   ├── Tape.*             # Implementación de cinta individual
│   ├── MultiTape.*        # Implementación de múltiples cintas
│   ├── Configuration.*    # Configuraciones instantáneas
│   ├── Parser.*           # Lector de archivos (monocinta y multicinta)
│   └── Simulator.*        # Motor de simulación
├── data/                  # Archivos de definición de máquinas
├── tests/                 # Archivos de prueba con palabras
├── build/                 # Archivos generados por la compilación
├── Makefile              # Sistema de compilación
└── README.md             # Este archivo
```

## Características

- **Máquinas Monocinta y Multicinta**: Soporte completo para ambos tipos
- **Orientación a Objetos**: Diseño modular con clases bien definidas
- **Cintas Infinitas**: Implementación eficiente usando mapa disperso
- **Detección de Bucles**: Identifica bucles infinitos por configuraciones repetidas
- **Trazas de Ejecución**: Visualización paso a paso de la simulación
- **Visualización de Cintas Finales**: Muestra automáticamente el estado de las cintas al terminar cada simulación
- **Formato Estándar**: Compatible con formatos de archivos de definición estándar
- **Validación Exhaustiva**: Verificación estricta de definiciones de máquinas
  - Detecta si el estado inicial no está declarado en el conjunto de estados
  - Detecta si el símbolo blanco no está en el alfabeto de cinta
  - Valida símbolos en transiciones y palabras de entrada
- **Interfaz de Línea de Comandos**: Similar a herramientas académicas estándar
- **Conversión Automática**: Puede convertir máquinas monocinta a multicinta

## Compilación

### Requisitos
- Compilador C++ compatible con C++17 (g++ recomendado)
- Make (para usar el Makefile)

### Comandos de Compilación

```bash
# Compilación estándar
make

# Compilación en modo debug
make debug

# Compilación optimizada
make release

# Limpiar archivos generados
make clean

# Mostrar información del proyecto
make info
```

## Uso

### Sintaxis Básica
```bash
./build/mt-sim <archivo_maquina> [opciones]
```

### Opciones Disponibles
- `--trace`: Muestra la traza paso a paso de la ejecución
- `--words <archivo>`: Lee palabras desde un archivo en lugar de stdin
- `--strict`: Modo estricto - error si hay símbolos fuera del alfabeto
- `--max-steps <N>`: Límite de pasos para evitar bucles infinitos (0 = sin límite)
- `--info`: Muestra información de la máquina y termina
- `--help`: Muestra ayuda

### Ejemplos de Uso

```bash
# Simular con entrada desde teclado
./build/mt-sim data/cadenas_impar_ceros.txt

# Simular con traza habilitada
./build/mt-sim data/cadenas_impar_ceros.txt --trace

# Procesar múltiples palabras desde archivo
./build/mt-sim data/cadenas_impar_ceros.txt --words tests/palabras_impar_ceros.txt

# Mostrar información de una máquina
./build/mt-sim data/a_n_b_n.txt --info

# Modo estricto con límite de pasos
./build/mt-sim data/doble_numero.txt --strict --max-steps 100

# Ver el estado final de la cinta después de procesar
echo "111" | ./build/mt-sim data/doble_numero.txt
# Output:
# ACCEPT
# Cinta final: . . . . [1] 1 1 1 1 1 . . . .
```

## Formato de Archivos de Definición

### Máquinas Monocinta

Los archivos de definición siguen este formato:

```
# Comentarios empiezan con #
estados_separados_por_espacios
alfabeto_entrada_separado_por_espacios
alfabeto_cinta_separado_por_espacios
estado_inicial
símbolo_blanco
estados_aceptación_separados_por_espacios
# Transiciones (una por línea):
estado_origen símbolo_leído estado_destino símbolo_escrito movimiento
```

#### Ejemplo Monocinta:
```
q0 q1 q2
0 1
0 1 .
q0
.
q2
q0 0 q1 0 R
q0 1 q0 1 R
q1 0 q0 0 R
q1 1 q1 1 R
q1 . q2 . R
```

### Máquinas Multicinta

Los archivos de definición multicinta siguen este formato:

```
MULTICINTA <número_de_cintas>
estados_separados_por_espacios
alfabeto_entrada_separado_por_espacios
alfabeto_cinta_separado_por_espacios
estado_inicial
símbolo_blanco
estados_aceptación_separados_por_espacios
# Transiciones multicinta (una por línea):
estado_origen símb1,símb2,... estado_destino esc1,esc2,... mov1,mov2,...
```

#### Ejemplo Multicinta (Suma de dos números):
```
MULTICINTA 2
q0 q1 q2 q3 q4
1 0
1 0 .
q0
.
q4
q0 1,. q1 1,1 R,R
q1 1,. q1 1,1 R,R
q1 0,. q2 0,. R,S
q2 1,. q2 .,1 R,R
q2 .,. q3 .,. L,S
q3 0,. q3 0,. L,S
q3 1,. q3 1,. L,S
q3 .,. q4 .,. S,S
```

## Máquinas de Ejemplo Incluidas

### Máquinas Monocinta

### 1. `cadenas_impar_ceros.txt`
Reconoce cadenas binarias con número impar de 0s.
- **Alfabeto**: {0, 1}
- **Ejemplo**: "101" → ACCEPT, "1001" → REJECT

### 2. `doble_numero.txt`
Calcula el doble de un número representado como 1^n.
- **Alfabeto**: {1}
- **Ejemplo**: "111" (3) → resultado "111111" (6)

### 3. `a_n_b_n.txt`
Reconoce el lenguaje {a^n b^n | n ≥ 1}.
- **Alfabeto**: {a, b}
- **Ejemplo**: "aabb" → ACCEPT, "aab" → REJECT

### 4. `acepta_todo.txt`
Acepta cualquier cadena del alfabeto dado.
- **Alfabeto**: {a, b, c}

### Máquinas Multicinta

### 5. `suma_multicinta.txt`
Suma dos números en representación unaria usando 2 cintas.
- **Entrada**: "1110111" (3+3) en cinta 1
- **Salida**: "111111" (6) en cinta 2
- **Alfabeto**: {1, 0}

### 6. `anbn_multicinta.txt`
Reconoce el lenguaje {a^n b^n | n ≥ 1} usando 2 cintas.
- **Alfabeto**: {a, b}
- **Ejemplo**: "aabb" → ACCEPT (verifica con 2 cintas)

### 7. `copia_multicinta.txt`
Copia el contenido de la cinta 1 a la cinta 2.
- **Entrada**: cualquier palabra en cinta 1
- **Salida**: palabra copiada en cinta 2

## Archivos de Error de Prueba

El proyecto incluye archivos de ejemplo con errores intencionales para probar la validación:

- **`error1_mt.txt`**: Estado inicial 'qInicial' no declarado en el conjunto de estados
- **`error2_mt.txt`**: Símbolo blanco 'Z' no incluido en el alfabeto de cinta
- **`error3_multicinta.txt`**: Error de estado inicial en máquina multicinta
- **`error4_multicinta.txt`**: Error de símbolo blanco en máquina multicinta

Estos archivos permiten verificar que el parser detecta correctamente configuraciones inválidas.


## Resultados de Simulación

El simulador muestra dos tipos de información al procesar cada palabra:

### Resultado de Aceptación
- **ACCEPT**: La palabra fue aceptada por la máquina
- **REJECT**: La palabra fue rechazada
- **INFINITE**: Bucle infinito detectado o límite de pasos alcanzado
- **ERROR**: Error durante la simulación

### Visualización de Cintas Finales

Después de cada resultado, el simulador muestra automáticamente el estado final de las cintas:

**Para máquinas monocinta:**
```
ACCEPT
Cinta final: . . . X X Y Y [.] . . .
```

**Para máquinas multicinta:**
```
ACCEPT
Cintas finales:
  Cinta 1: . . a a b b [.] . .
  Cinta 2: . . . . . . [.] . .
```

Los corchetes `[símbolo]` indican la posición actual del cabezal de lectura/escritura.

## Detección de Bucles Infinitos

El simulador detecta bucles infinitos mediante dos mecanismos:
1. **Límite de pasos**: Configurable con `--max-steps`
2. **Configuraciones repetidas**: Detecta cuando se repite una configuración (estado + posición cabezal + contenido cinta)

## Arquitectura del Código

### Clases Principales

#### Máquinas Monocinta
- **`TuringMachine`**: Definición formal de la máquina (Q, Σ, Γ, δ, q₀, F)
- **`Transition`**: Representación de una transición individual
- **`Tape`**: Cinta infinita implementada con mapa disperso
- **`Configuration`**: Estado instantáneo de la máquina

#### Máquinas Multicinta
- **`MultiTuringMachine`**: Definición formal de máquina multicinta
- **`MultiTransition`**: Transición que especifica operaciones en todas las cintas
- **`MultiTape`**: Gestión de múltiples cintas independientes

#### Componentes Comunes
- **`Parser`**: Carga y guarda definiciones (monocinta y multicinta)
- **`Simulator`**: Motor de simulación con detección de bucles

### Principios de Diseño

- Separación clara de responsabilidades
- Encapsulación de datos con getters/setters
- Manejo robusto de errores
- Documentación estilo Doxygen
- Código legible y mantenible


## Autor

Guillermo González Pineda