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

void Juego::initInput()
{
    //mouse
    this->mouseMappings["BTN_ADD_TILE"] = sf::Mouse::Button::Left;
    this->mouseMappings["BTN_REMOVE_TILE"] = sf::Mouse::Button::Right;

    //keyboard
    this->keyboardMappings["KEY_MOVE_LEFT"] = sf::Keyboard::Key::A;
    this->keyboardMappings["KEY_MOVE_RIGHT"] = sf::Keyboard::Key::D;
    this->keyboardMappings["KEY_MOVE_UP"] = sf::Keyboard::Key::W;
    this->keyboardMappings["KEY_MOVE_DOWN"] = sf::Keyboard::Key::S;
    this->keyboardMappings["KEY_MOVE_ATTACK"] = sf::Keyboard::Key::K;
}

void Juego::initTileSheet()
{
    if(!this->tileSheet.loadFromFile("Sprites ejemplo/dungeon_tile.png"))
    {
        std::cout << "ERROR CON LA CARGA DE TILES TEXTURE: Juego::initTileSheet";
    }
}

void Juego::initPersonajes()
{
    this->jugador = new Jugador(590.f, 230.f);
}

void Juego::initHabitacion()
{
    this->habitacionActual = new Habitacion(&this->tileSheet);
}

Juego::Juego()
{
    this->initVariables();
    this->initWindow();
    this->initInput();
    this->initTileSheet();
    this->initPersonajes();
    this->initHabitacion();
}

Juego::~Juego()
{
    delete this->window;
    delete this->jugador;
    delete this->habitacionActual;
}

//Accesors
const bool Juego::gameRunning() const { return this->window->isOpen(); }

const bool Juego::getFinalizarJuego() const { return this->finalizarJuego; }

const sf::RenderWindow& Juego::getWindow() const { return *this->window; }

//Functions

void Juego::updateInput()
{
    //Mouse
    /*
    std::cout << int(sf::Mouse::getPosition(this->getWindow()).x) / int(this->habitacionActual->getTileMap()->getTileSize())
                     << " " << int(sf::Mouse::getPosition(this->getWindow()).y) / int(this->habitacionActual->getTileMap()->getTileSize())
                     << std::endl;*/

    const int mouseX = int(sf::Mouse::getPosition(this->getWindow()).x) / int(this->habitacionActual->getTileMap()->getTileSize());
    const int mouseY = int(sf::Mouse::getPosition(this->getWindow()).y) / int(this->habitacionActual->getTileMap()->getTileSize());

    //Player movement
    if(sf::Keyboard::isKeyPressed(this->keyboardMappings["KEY_MOVE_LEFT"]))
    {

    }

    //Tile funcs
    if(sf::Mouse::isButtonPressed(this->mouseMappings["BTN_ADD_TILE"]))
    {
        this->habitacionActual->getTileMap()->addTile(mouseX, mouseY, 1);
    }
    else if(sf::Mouse::isButtonPressed(this->mouseMappings["BTN_REMOVE_TILE"]))
    {
        this->habitacionActual->getTileMap()->removeTile(mouseX, mouseY);
    }
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
    //bordes de la ventana
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

    //colision con objetos del mapa
    TileMap* mapa = this->habitacionActual->getTileMap();
    if (mapa == nullptr) return; //ya veo que hacemos cagada

    std::vector<Personajes*> personajes;
    personajes.push_back(this->jugador);
    for (auto* enemigo : this->habitacionActual->getEnemigos())
    {
        personajes.push_back(enemigo);
    }

    for (auto* p : personajes)
    {
        sf::Vector2f vel = p->getVelocidadVector();

        p->mover(vel.x, 0.f);
        if (mapa->checkCollision(p->getHitboxBounds()))
        {
            p->mover(-vel.x, 0.f);
        }

        p->mover(0.f, vel.y);
        if (mapa->checkCollision(p->getHitboxBounds()))
        {
            p->mover(0.f, -vel.y);
        }
    }
    /*
    this->jugador->mover(this->jugador->getVelocidadVector().x, 0.f);
    if (mapa->checkCollision(this->jugador->getHitboxBounds()))
    {
        this->jugador->mover(-this->jugador->getVelocidadVector().x, 0.f);
    }

    this->jugador->mover(0.f, this->jugador->getVelocidadVector().y);
    if (mapa->checkCollision(this->jugador->getHitboxBounds()))
    {
        this->jugador->mover(0.f, -this->jugador->getVelocidadVector().y);
    }*/

}

void Juego::update()
{
    this->pollEvents();

    if (!this->finalizarJuego)
    {
        this->updateInput();

        this->updatePersonajes();

        this->habitacionActual->update(this->jugador);

        this->updateCollision();

    }

    //cuando termina el juego
    if(this-> jugador->getHp() <= 0)
        this->finalizarJuego = true;

}

void Juego::render()
{
    this->window->clear();

    this->habitacionActual->renderFondo(*this->window);

    //todos en el vector
    std::vector<Personajes*> personajesParaRender;
    personajesParaRender.push_back(this->jugador);

    for (auto* enemigo : this->habitacionActual->getEnemigos())
    {
        personajesParaRender.push_back(enemigo);
    }

    //orden
    std::sort(personajesParaRender.begin(), personajesParaRender.end(),
        [](Personajes* a, Personajes* b) {
            return a->getPosition().y < b->getPosition().y;
        });

    //draw
    for (auto* personaje : personajesParaRender)
    {
        personaje->render(*this->window);

        //personaje->renderHitbox(*this->window);
    }

    this->window->display();
}
