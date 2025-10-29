#include "Game_Class.h"

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
    this->videoMode.height = visualConfig.getResolucionX();
    this->videoMode.width = visualConfig.getResolucionY();

    this->window = new sf::RenderWindow(this->videoMode,
                                        "The Fallen Knight", sf::Style::Titlebar | sf::Style::Close);

    this->window->setFramerateLimit(visualConfig.getFramerate());
}

void Juego::initPersonajes()
{
    this->jugador = new Jugador();
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
    delete this->jugador;
}

//Accesors
const bool Juego::gameRunning() const
{
    return this->window->isOpen();
}

const bool Juego::getFinalizarJuego() const
{
    return this->finalizarJuego;
}


//Functions

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

        if(this->ev.type == sf::Event::KeyReleased &&
                (this->ev.key.code == sf::Keyboard::W ||
                 this->ev.key.code == sf::Keyboard::A ||
                 this->ev.key.code == sf::Keyboard::S ||
                 this->ev.key.code == sf::Keyboard::D ))
        {
            this->jugador->resetAnimTimer();
        }
    }
}

void Juego::updatePersonajes()
{
    jugador->update();
}

void Juego::updateCollision()
{
    if(this->jugador->getPosition().y + this->jugador->getGlobalBounds().height > this->window->getSize().y)
    {
        this->jugador->setPosition(
            this->jugador->getPosition().x,
            this->window->getSize().y - this->jugador->getGlobalBounds().height
        );
    }

    if(this->jugador->getPosition().y < 0)
    {
        this->jugador->setPosition(
            this->jugador->getPosition().x, 0
        );
    }

    if(this->jugador->getPosition().x + this->jugador->getGlobalBounds().width > this->window->getSize().x)
    {
        this->jugador->setPosition(
            this->window->getSize().x - this->jugador->getGlobalBounds().width,
            this->jugador->getPosition().y
        );
    }

    if(this->jugador->getPosition().x < 0)
    {
        this->jugador->setPosition(
            0, this->jugador->getPosition().y
        );
    }
}

void Juego::update()
{
    this->pollEvents();

    if (!this->finalizarJuego)
    {
        this->updatePersonajes();
        this->updateCollision();
    }

    //cuando termina el juego
    if(this-> jugador->getHp() <= 0)
        this->finalizarJuego = true;

}

void Juego::renderPersonajes(sf::RenderTarget& target)
{
    jugador->render(target);
}

void Juego::render()
{
    this->window->clear();

    this->renderPersonajes(*this->window);

    this->window->display();
}
