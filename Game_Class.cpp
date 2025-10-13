#include "Game_Class.h"
#include "Game_Struct_Clases.h"

void Juego::initVariables()
{
//window
    this->window = nullptr;

//Logica del juego
    finalizarJuego = false;
    vida = 5;
}

void Juego::initWindow()
{
    this->videoMode.height = 600;
    this->videoMode.width = 600;

    this->window = new sf::RenderWindow(this->videoMode,
                    "The Fallen Knight", sf::Style::Titlebar | sf::Style::Close);

    this->window->setFramerateLimit(61);
}

void Juego::initPersonajes()
{
    Jugador();
}

Juego::Juego()
{
    this->initVariables();
    this->initWindow();
    this->initPersonajes();
}

Juego::~Juego()
{
    delete this->window;
}

//Accesors

const bool Juego::gameRunning() const {return this->window->isOpen();}

const bool Juego::getFinalizarJuego() const {return this->finalizarJuego;}

//Functions

void Juego::spawnEnemigo()
{

}

void Juego::pollEvents()
{
    while (this->window->pollEvent(this->ev))
    {
        switch(this->ev.type)
        {
        case sf::Event::Closed:
            this->window->close();
            break;
        case sf::Event::KeyPressed:
            if(this->ev.key.code == sf::Keyboard::Escape)
                this->window->close();
            break;
        }
    }
}

void Juego::updatePersonajes()
{
    jugador.update();
}

void Juego::update()
{
    this->pollEvents();

    if (!this->finalizarJuego)
    {
    this->updatePersonajes();
    }

    //cuando termina el juego
    if(this->vida<= 0)
        this->finalizarJuego = true;

}

void Juego::renderPersonajes(sf::RenderTarget& target)
{
    jugador.draw(target);
}
void Juego::render()
{
    this->window->clear();

    this->renderPersonajes(*this->window);

    this->window->display();
}
