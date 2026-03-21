# The Fallen Knight

<p align="center">
  <img src="https://img.shields.io/badge/Language-C++-blue.svg">
  <img src="https://img.shields.io/badge/Library-SFML-green.svg">
  <img src="https://img.shields.io/badge/Class-Programming%202-orange.svg">
</p>

**The Fallen Knight** es un videojuego RPG Roguelike 2D desarrollado enteramente en C++ utilizando la biblioteca multimedia **SFML**. El proyecto destaca por incorporar generación procedural de mazmorras por salas (estilo _The Binding of Isaac_), una fuerte orientación a objetos (OOP) y sistemas completos de juego (IA, inventario, guardado de partidas).

## Características Principales

- **Generación Procedural de Mazmorras**: Cada partida genera un mapa único con salas interconectadas de manera lógica, garantizando rejugabilidad.
- **Sistema de Entidades y Polimorfismo**: Construido sobre bases sólidas de OOP (`Entity` -> `Character` -> `Jugador` / `Enemigos`), permitiendo extender enemigos y proyectiles de forma elegante.
- **Inteligencia Artificial (IA) de Enemigos**:
  - **Duende**: Enemigo veloz con poco ataque que persigue al jugador.
  - **Orco**: Tanque físico que persigue implacablemente al jugador.
  - **Hada**: Enemigo de soporte frágil que huye del jugador y cura a otros enemigos cercanos.
  - **Hechicero**: Enemigo de daño a distancia que mantiene la distancia óptima para disparar proyectiles mágicos.
  - **Estatua**: Enemigo estático.
- **Sistema de Inventario y HUD**: Recolección de monedas, llaves y diferentes objetos ("pickups") que alteran el estado o generan ataques en el jugador. Interfaz gráfica en vivo.
- **Sistema de Guardado (Save/Load)**: Usa archivos **JSON** para persistir de manera segura la "seed" (semilla) de la mazmorra de la partida, las estadísticas, la posición y el inventario del jugador en varios slots (F5 para guardar, F9 para cargar).
- **Menú Principal Interactuable**: Permite iniciar nuevas partidas, cargar partidas antiguas y configurar las opciones gráficas o de pantalla.

## Controles

| Acción                            | Tecla                                     |
| :-------------------------------- | :---------------------------------------- |
| **Moverse**                       | `W` `A` `S` `D` o `Flechas Direccionales` |
| **Atacar**                        | `K`                                       |
| **Defender**                      | `L`                                       |
| **Menú de Pausa / Guardar Slot**  | `F5`                                      |
| **Cargar Partida Rápida (Save1)** | `F9`                                      |
| **Reiniciar (Game Over)**         | `R`                                       |

## Arquitectura del Proyecto

Para el desarrollo se aplicaron diferentes principios de programación orientada a objetos:

1. **Herencia y Polimorfismo**: Funciones virtuales `updateIA()` y `attack()` permiten manejar arreglos polimórficos de `Character*`, logrando que la clase manejadora (`Habitacion` o `Game_Class`) simplemente itere sobre una lista ordenándoles actuar.
2. **Encapsulamiento**: Las clases manejan sus propios sprites, animaciones (con `sf::IntRect`), timers y lógicas de físicas (velocidad, hitboxes) aislándolos del núcleo central `Game_Class`.
3. **Manejo de Memoria Dinámica**: Creación y de-alocación limpia de Habitaciones, Proyectiles, y Enemigos para prevenir "Memory Leaks", crucial en un roguelike con cuartos dinámicos.
4. **Almacenamiento Serializado**: El sistema extrae los datos puros a estructuras `GameData` que se serializan y parsean a texto JSON plano a través de la librería `SaveManager`.

## Requisitos y Configuración (Setup)

- Compilador C++ (MinGW64 recomendado si usas Windows) compatible con **C++17** o superior.
- Librería **SFML 2.5** (o superior hasta 2.6.2).
- Opcional: Entorno de desarrollo **Code::Blocks** (el repositorio ya incluye archivos de configuración).

**Para compilar con Code::Blocks o Make:**

1. Clona el repositorio.
2. Vincula los directorios `include` y `lib` de tu propia instalación de SFML en las opciones del proyecto o en el Linker.
3. Asegúrate de añadir (linkear) los módulos: `sfml-graphics`, `sfml-window` y `sfml-system`.
4. Build and run.
