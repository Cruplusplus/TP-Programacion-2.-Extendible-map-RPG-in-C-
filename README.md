# Extendible Map RPG Roguelike (C++)

<p align="center">
  <img src="https://img.shields.io/badge/Language-C++-blue.svg">
  <img src="https://img.shields.io/badge/Library-SFML-green.svg">
</p>

Un simulador de mapa extensible y lógica RPG desarrollado en C++ utilizando la librería **SFML**. Este proyecto fue creado como trabajo integrador para la materia Programación II (UTN), enfocado en la aplicación de conceptos avanzados de programación orientada a objetos y gestión de recursos.

## Tecnologías utilizadas
- **Lenguaje:** C++17 o superior.
- **Multimedia:** SFML (Simple and Fast Multimedia Library).
- **Serialización:** JSON for Modern C++.
- **Control de Versiones:** Git / GitHub.

## Screenshots
<img width="1366" height="768" alt="image" src="https://github.com/user-attachments/assets/7a483dd9-7b82-4dfd-a422-39abcb3bbae0" />
<img width="799" height="625" alt="image" src="https://github.com/user-attachments/assets/a437ea76-4ff2-4f9b-9b1a-18141c98d9bf" />
<img width="505" height="547" alt="image" src="https://github.com/user-attachments/assets/dd25c089-76e2-4d3f-a197-ed6029ad0ed9" />


## Características Principales

- **Generación Procedural de Mazmorras**: Cada partida genera un mapa único con salas interconectadas de manera lógica, garantizando rejugabilidad.
- **Sistema de Entidades y Polimorfismo**: Construido sobre bases sólidas de OOP (`Entity` -> `Character` -> `Jugador` / `Enemigos`), permitiendo extender enemigos y proyectiles de forma elegante.
- **Mapa Extensible:** Generación y gestión de mapas dinámicos.
- **Persistencia de Datos:** Sistema de guardado y carga mediante archivos **JSON** (usando `nlohmann/json`).
- **Renderizado Eficiente:** Uso de la librería SFML para el manejo de gráficos por hardware y detección de colisiones en tiempo real.
- **Arquitectura Robusta:** Aplicación de Patrones de Diseño (State, Factory) y principios POO.
- **Inteligencia Artificial (IA) de Enemigos**:
  - **Duende**: Enemigo veloz con poco ataque que persigue al jugador.
  - **Orco**: Tanque físico que persigue implacablemente al jugador.
  - **Hada**: Enemigo de soporte frágil que huye del jugador y cura a otros enemigos cercanos.
  - **Hechicero**: Enemigo de daño a distancia que mantiene la distancia óptima para disparar proyectiles mágicos.
  - **Estatua**: Enemigo estático.
- **Sistema de Inventario y HUD**: Recolección de monedas, llaves y diferentes objetos ("pickups") que alteran el estado o generan ataques en el jugador. Interfaz gráfica en vivo.
- **Menú Principal Interactuable**: Permite iniciar nuevas partidas y cargar partidas antiguas.

## Controles

| Acción                            | Tecla                                     |
| :-------------------------------- | :---------------------------------------- |
| **Moverse**                       | `W` `A` `S` `D`                           |
| **Atacar**                        | `K`                                       |
| **Defender o Parry**              | `L`                                       |
| **Menú de Pausa Guardar/Cargar**  | `F5`                                      |
| **Cargar Partida Rápida (Save1)** | `F9`                                      |
| **Reiniciar (Game Over)**         | `R`                                       |

## Arquitectura del Proyecto

Para el desarrollo se aplicaron diferentes principios de programación orientada a objetos:

1. **Herencia y Polimorfismo**: Funciones virtuales `updateIA()` y `attack()` permiten manejar arreglos polimórficos de `Character*`, logrando que la clase manejadora (`Habitacion` o `Game_Class`) simplemente itere sobre una lista ordenándoles actuar.
2. **Encapsulamiento**: Las clases manejan sus propios sprites, animaciones (con `sf::IntRect`), timers y lógicas de físicas (velocidad, hitboxes) aislándolos del núcleo central `Game_Class`.
3. **Manejo de Memoria Dinámica**: Creación y de-alocación limpia de Habitaciones, Proyectiles, y Enemigos para prevenir "Memory Leaks", crucial en un roguelike con cuartos dinámicos.
4. **Almacenamiento Serializado**: El sistema extrae los datos puros a estructuras `GameData` que se serializan y parsean a texto JSON plano a través de la librería `SaveManager`.

# Requisitos y Configuración (Setup)

- Compilador C++ (MinGW64 recomendado si usas Windows) compatible con **C++17** o superior.
- Librería **SFML 2.5** (o superior hasta 2.6.2).
- Opcional: Entorno de desarrollo **Code::Blocks** (el repositorio ya incluye archivos de configuración).

## Para compilar con Code::Blocks o Make:
Para ejecutar este proyecto localmente, asegúrate de tener instalada la librería SFML.

1. Clona el repositorio:

   ```bash
   git clone [https://github.com/Cruplusplus/TP-Programacion-2.-Extendible-map-RPG-in-C-.git](https://github.com/Cruplusplus/TP-Programacion-2.-Extendible-map-RPG-in-C-.git)

2. Vincula los directorios `include` y `lib` de tu propia instalación de SFML en las opciones del proyecto o en el Linker.
3. Elije el compilador `mingw64` que está en la carpeta Librerias
4. Asegúrate de añadir (linkear) los módulos: `sfml-graphics`, `sfml-window` y `sfml-system`.
5. Build and run.

Desarrollado por Juan Cruz Dominguez Pistoia y Lautaro Lorenzo Silva (Trabajo práctico para Programación 2 - Tecnicatura Universitaria en Programación)
