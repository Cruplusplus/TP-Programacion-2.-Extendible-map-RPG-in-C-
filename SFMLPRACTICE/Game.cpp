#include "Game.h"

void Juego::initVariables()
{
    this->window = nullptr;
}

void Juego::initWindow()
{
    this->videoMode.height = 600;
    this->videoMode.width = 800;

    this->window = new sf::RenderWindow(this->videoMode, "Game 1", sf::Style::Titlebar | sf::Style::Close);
}

Juego::Juego()
{
    this->initVariables();
    this->initWindow();
}

Juego::~Juego()
{
    delete this->window;
}

//Accesors

const bool Juego::gameRunning() const
{
    return this->window->isOpen();
}

//Functions

void Juego::pollEvents()
{
    //event polling
    while(this->window->pollEvent(this->ev) )  //mientras exista pollevent, lo guarda en event ev
    {
        switch(this->ev.type)
        {
        case sf::Event::Closed:
            this->window->close();
            break;
        case sf::Event::KeyPressed:
            if(this->ev.key.code == sf::Keyboard::Escape)
                this->window->close();
        }

    }
}

void Juego::update()
{
    this->pollEvents();
}

void Juego::render()
{
    this->window->clear(sf::Color(255, 0, 0, 255) );

    this->window->display();
}
