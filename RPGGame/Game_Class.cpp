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
    this->seed = static_cast<unsigned int>(time(NULL));
    this->dungeonGen = new DungeonGenerator(10, 10, 10);
    this->dungeonGen->generate(this->seed);

    this->currentRoomCoords.x = this->dungeonGen->getWidth() / 2;
    this->currentRoomCoords.y = this->dungeonGen->getHeight() / 2;

    this->habitacionActual = new Habitacion(&this->tileSheet, this->dungeonGen->getRoom(this->currentRoomCoords.x, this->currentRoomCoords.y));
    this->roomsMap[std::make_pair(this->currentRoomCoords.x, this->currentRoomCoords.y)] = this->habitacionActual;
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
    delete this->dungeonGen;
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

    //Esto fue durisimo de refactorizar
    

    bool roomChanged = false;
    sf::Vector2i nextRoom = this->currentRoomCoords;
    sf::Vector2f nextPlayerPos = this->jugador->getPosition();
    bool enemiesExist = !this->habitacionActual->getEnemigos().empty();

    float winWidth = this->window->getSize().x;
    float winHeight = this->window->getSize().y;

    TileMap* mapa = this->habitacionActual->getTileMap();
    if (mapa == nullptr) return;

    std::vector<Character*> personajes;
    personajes.push_back(this->jugador);
    for (auto* enemigo : this->habitacionActual->getEnemigos())
    {
        personajes.push_back(enemigo);
    }

    for (auto* p : personajes)
    {
        bool isPlayer = (p == this->jugador);
        sf::Vector2f vel = p->getVelocidadVector();

        //movimiento en X
        p->mover(vel.x, 0.f);
        sf::FloatRect boundsX = p->getHitboxBounds();
        bool colX = mapa->checkCollision(boundsX);
        
        //bordes de la ventana en X
        if (boundsX.left < 0) {
            if (isPlayer && this->habitacionActual->getRoomData().doors[3] && 
                (boundsX.top + boundsX.height / 2.f) > winHeight / 2 - 50 && 
                (boundsX.top + boundsX.height / 2.f) < winHeight / 2 + 50 &&
                !enemiesExist) {
                nextRoom.x--;
                nextPlayerPos.x = winWidth - boundsX.width - 10.f;
                roomChanged = true;
            } else {
                colX = true;
            }
        } else if (boundsX.left + boundsX.width > winWidth) {
            if (isPlayer && this->habitacionActual->getRoomData().doors[1] && 
                (boundsX.top + boundsX.height / 2.f) > winHeight / 2 - 50 && 
                (boundsX.top + boundsX.height / 2.f) < winHeight / 2 + 50 &&
                !enemiesExist) {
                nextRoom.x++;
                nextPlayerPos.x = 10.f;
                roomChanged = true;
            } else {
                colX = true;
            }
        }

        if (colX) {
            p->mover(-vel.x, 0.f);
        }

        //movimiento en Y
        p->mover(0.f, vel.y);
        sf::FloatRect boundsY = p->getHitboxBounds();
        bool colY = mapa->checkCollision(boundsY);
        
        //bordes de la ventana en Y
        if (boundsY.top < 0) {
            if (isPlayer && this->habitacionActual->getRoomData().doors[0] && 
                (boundsY.left + boundsY.width / 2.f) > winWidth / 2 - 50 && 
                (boundsY.left + boundsY.width / 2.f) < winWidth / 2 + 50 &&
                !enemiesExist) {
                nextRoom.y--;
                nextPlayerPos.y = winHeight - boundsY.height - 10.f - 60.f;
                roomChanged = true;
            } else {
                colY = true;
            }
        } else if (boundsY.top + boundsY.height > winHeight) {
            if (isPlayer && this->habitacionActual->getRoomData().doors[2] && 
                (boundsY.left + boundsY.width / 2.f) > winWidth / 2 - 50 && 
                (boundsY.left + boundsY.width / 2.f) < winWidth / 2 + 50 &&
                !enemiesExist) {
                nextRoom.y++;
                nextPlayerPos.y = 10.f - 60.f;
                roomChanged = true;
            } else {
                colY = true;
            }
        }

        if (colY) {
            p->mover(0.f, -vel.y);
        }
    }

    if(roomChanged) {
        this->currentRoomCoords = nextRoom;
        
        auto it = this->roomsMap.find(std::make_pair(this->currentRoomCoords.x, this->currentRoomCoords.y));
        if(it != this->roomsMap.end()) {
            this->habitacionActual = it->second;
        } else {
            this->habitacionActual = new Habitacion(&this->tileSheet, this->dungeonGen->getRoom(this->currentRoomCoords.x, this->currentRoomCoords.y));
            this->roomsMap[std::make_pair(this->currentRoomCoords.x, this->currentRoomCoords.y)] = this->habitacionActual;
        }

        this->jugador->setPosition(nextPlayerPos.x, nextPlayerPos.y);
    }
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
    std::vector<Character*> personajesParaRender;
    personajesParaRender.push_back(this->jugador);

    for (auto* enemigo : this->habitacionActual->getEnemigos())
    {
        personajesParaRender.push_back(enemigo);
    }

    //orden
    std::sort(personajesParaRender.begin(), personajesParaRender.end(),
        [](Character* a, Character* b) {
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
