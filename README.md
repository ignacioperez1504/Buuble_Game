# THE MIND – Implementación en C++ con SFML
Este proyecto es una implementación del juego *The Mind* utilizando C++ y la librería gráfica SFML. Permite jugar de manera local simulando la dinámica cooperativa del juego original, donde los jugadores deben coordinarse sin comunicación explícita.

## Descripción general

El juego consiste en que varios jugadores deben jugar cartas en orden ascendente sin decir sus valores. Cada jugador recibe un conjunto de cartas y, mediante intuición y sincronización, deben colocarlas correctamente.

Esta versión implementa mejoras significativas:
* Sistema de niveles
* Vidas y comodines
* Interfaz gráfica con SFML
* Entrada por teclado para jugar cartas
* Visualización de cartas y jugadas
* Opción de Ver y Ocultar cartas
* Mensajes de estado para el usuario
* Carga y descarga de partidas

---

## Estructura del proyecto

El proyecto está organizado en varias clases principales:


### `Carta`

Representa una carta del juego.

**Atributos:**

* `int valor`: número de la carta (1–100)

**Métodos:**

* `Carta(int v)`: constructor
* `int getValor()`: retorna el valor de la carta

---

### `Jugador`

Gestiona la mano de cartas de cada jugador.

**Atributos:**

* `std::vector<Carta> mano`: cartas del jugador

**Métodos:**

* `recibirCarta(Carta c)`: añade una carta
* `ordenarMano()`: ordena la mano de menor a mayor
* `jugarCarta()`: elimina y retorna la carta superior
* `verCarta()`: retorna la carta superior sin eliminarla
* `tieneCartas()`: verifica si tiene cartas
* `eliminarCartaMasBaja()`: elimina la carta más pequeña (usado en comodines)

---

### `Menu`

Encargado de mostrar la pantalla inicial.

**Funciones:**

* Permite seleccionar el número de jugadores (2, 3 o 4)
* Renderiza texto en pantalla

---

### `JuegoVisual`

Es la clase principal del juego. Controla toda la lógica y la interfaz gráfica.

**Responsabilidades:**

* Manejo de eventos (teclado y mouse)
* Renderizado del juego
* Control de niveles, vidas y comodines
* Lógica de jugadas
* Sistema de mensajes visuales

**Atributos importantes:**

* `vidas`: número de errores permitidos
* `nivel`: dificultad actual
* `jugadores`: lista de jugadores
* `cartasJugadas`: historial visual
* `inputTexto`: entrada del usuario

**Métodos clave:**

* `iniciarNivel()`: reparte cartas y reinicia estado
* `jugarDesdeInput(string)`: procesa la jugada del usuario
* `getMinGlobal()`: obtiene la carta más baja entre todos los jugadores
* `verificarFinNivel()`: avanza de nivel si se cumplen condiciones
* `usarComodin()`: elimina cartas más bajas de todos los jugadores
* `dibujarJuego()`: renderiza el estado del juego
* `dibujarOverlay()`: muestra cartas de cada jugador

---

## Sintaxis del juego (cómo jugar)

### Ingreso de cartas

* Escribe el número de la carta (ej: `23`)
* Presiona **ENTER** para jugarla

### Controles

| Tecla         | Acción                          |
| ------------- | ------------------------------- |
| `2`, `3`, `4` | Seleccionar número de jugadores |
| `ENTER`       | Jugar carta                     |
| `V`           | Ver cartas de jugadores         |
| `SPACE`       | Cambiar de jugador en overlay   |
| `CLICK`       | Mostrar/ocultar carta           |
| `B`           | Usar comodín                    |

---

### Reglas implementadas

* Debes jugar la **carta más baja disponible globalmente**
* Si te equivocas:

  * Se eliminan todas las cartas menores
  * Pierdes vidas según cuántas cartas se omitieron
* Si aciertas:

  * La carta se juega correctamente
* Si todos los jugadores se quedan sin cartas:

  * Subes de nivel
  * Recibes una recompensa (vida o comodín de manera aleatoria)

---

## ¿Cómo ejecutar el proyecto?

### 1. Compilar

```bash
g++ src/*.cpp -Iinclude -o build/juego -lsfml-graphics -lsfml-window -lsfml-system
```

### 2. Ejecutar

```bash
.\build\juego
```

---

## Reglas completas del juego
[Video Youtube Reglas The Mind](https://www.youtube.com/watch?v=dFGmKid9O9k)


---

## Posibles mejoras futuras

* Multiplayer real (red/local)
* Animaciones más fluidas
* Sonido y efectos
* Sistema de puntuación
* Mejor manejo de inputs (validación y UX)
* Refactorización para evitar copias innecesarias de objetos

---

## Requisitos técnicos

* C++17 o superior
* SFML instalada correctamente
* Compilador compatible con `g++`

---

## Autores

Proyecto desarrollado como práctica de programación orientada a objetos y desarrollo gráfico en C++ para la clase de Estructura de datos del pregrado en Ingeniería en Ciencia de datos del ITM.
* Camilo Rafael Pérez Chaves
* Ignacio Joaquín Pérez Chaves
* Maria Paula
---
