#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

//{Motor grafico del juego
class Juego{
private:
    //variables
    //window
    sf::RenderWindow* window; //puntero a la ventana main
    sf::VideoMode videoMode;
    sf::Event ev; //tiene inicializacion default

    void initVariables();
    void initWindow();
public:
    //construc destruc
    Juego();
    virtual ~Juego();

    //Accesors
    const bool gameRunning() const;

    //Func
    void pollEvents();
    void update();
    void render();
};
//}
