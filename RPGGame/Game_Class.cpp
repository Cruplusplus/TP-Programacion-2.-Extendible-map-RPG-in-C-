#include "Game_Class.h"

void Juego::initVariables()
{
    // window
    this->window = nullptr;
    // Logica del juego
    finalizarJuego = false;
    vida = 5;
    this->gameState = STATE_MENU;

  //muejejejeje
    this->konamiSequence = {
      sf::Keyboard::Up, sf::Keyboard::Up, sf::Keyboard::Down, sf::Keyboard::Down, 
      sf::Keyboard::Left, sf::Keyboard::Right, sf::Keyboard::Left, sf::Keyboard::Right, 
      sf::Keyboard::B, sf::Keyboard::A
    };
    this->konamiIndex = 0;
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
    // mouse
    this->mouseMappings["BTN_ADD_TILE"] = sf::Mouse::Button::Left;
    this->mouseMappings["BTN_REMOVE_TILE"] = sf::Mouse::Button::Right;

    // keyboard
    this->keyboardMappings["KEY_MOVE_LEFT"] = sf::Keyboard::Key::A;
    this->keyboardMappings["KEY_MOVE_RIGHT"] = sf::Keyboard::Key::D;
    this->keyboardMappings["KEY_MOVE_UP"] = sf::Keyboard::Key::W;
    this->keyboardMappings["KEY_MOVE_DOWN"] = sf::Keyboard::Key::S;
    this->keyboardMappings["KEY_MOVE_ATTACK"] = sf::Keyboard::Key::K;
}

void Juego::initTileSheet()
{
    if (!this->tileSheet.loadFromFile("Sprites ejemplo/dungeon_tile.png"))
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

    this->habitacionActual = new Habitacion(&this->tileSheet, this->dungeonGen->getRoom(this->currentRoomCoords.x, this->currentRoomCoords.y), this->jugador->getLevelPiso());
    this->roomsMap[std::make_pair(this->currentRoomCoords.x, this->currentRoomCoords.y)] = this->habitacionActual;
}

void Juego::initFonts()
{
    // Carga fuente por defecto del sistema
    this->font.loadFromFile("C:/Windows/Fonts/arial.ttf");

    this->gameOverText.setFont(this->font);
    this->gameOverText.setCharacterSize(60);
    this->gameOverText.setFillColor(sf::Color::Red);
    this->gameOverText.setString("GAME OVER\nPress R to Restart\nPress Esc to Exit");

    // Centrar
    sf::FloatRect textRect = this->gameOverText.getLocalBounds();
    this->gameOverText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    this->gameOverText.setPosition(this->videoMode.width / 2.0f, this->videoMode.height / 2.0f);
}

Juego::Juego()
{
    this->initVariables();
    this->initWindow();
    this->initFonts();
    this->hud = new HUD();
    this->mainMenu = new MainMenu(static_cast<float>(this->videoMode.width), static_cast<float>(this->videoMode.height));
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
    delete this->hud;
    delete this->mainMenu;
}

// Accesors
const bool Juego::gameRunning() const { return this->window->isOpen(); }

const bool Juego::getFinalizarJuego() const { return this->finalizarJuego; }

const sf::RenderWindow &Juego::getWindow() const { return *this->window; }

// Functions

void Juego::updateInput()
{
    // Mouse
    /*
    std::cout << int(sf::Mouse::getPosition(this->getWindow()).x) / int(this->habitacionActual->getTileMap()->getTileSize())
                     << " " << int(sf::Mouse::getPosition(this->getWindow()).y) / int(this->habitacionActual->getTileMap()->getTileSize())
                     << std::endl;*/

    const int mouseX = int(sf::Mouse::getPosition(this->getWindow()).x) / int(this->habitacionActual->getTileMap()->getTileSize());
    const int mouseY = int(sf::Mouse::getPosition(this->getWindow()).y) / int(this->habitacionActual->getTileMap()->getTileSize());

    // Player movement
    if (sf::Keyboard::isKeyPressed(this->keyboardMappings["KEY_MOVE_LEFT"]))
    {
    }

    // Tile funcs
    // debug
    if (sf::Mouse::isButtonPressed(this->mouseMappings["BTN_ADD_TILE"]))
    {
        this->habitacionActual->getTileMap()->addTile(mouseX, mouseY, 1);
    }
    else if (sf::Mouse::isButtonPressed(this->mouseMappings["BTN_REMOVE_TILE"]))
    {
        this->habitacionActual->getTileMap()->removeTile(mouseX, mouseY);
    }
}

void Juego::pollEvents()
{
    while (this->window->pollEvent(this->ev))
    {
        switch (this->ev.type)
        {
        case sf::Event::Closed:
            this->window->close();
            break;
        case sf::Event::KeyPressed:
            if (this->ev.key.code == sf::Keyboard::Escape)
                this->window->close();

            if (this->ev.key.code == sf::Keyboard::R)
            {
                this->resetGame();
            }
            
            if (this->gameState == STATE_MENU || this->gameState == STATE_PAUSE_MENU)
            {
                this->handleMenuInput(this->ev.key.code);
            }
            else if (this->gameState == STATE_PLAYING && this->ev.key.code == sf::Keyboard::F5)
            {
                this->gameState = STATE_PAUSE_MENU;
                this->mainMenu->setState(MENU_MAIN);
            }

            // super secreto
            if(!this->gameState == STATE_PLAYING) break;

            if (this->ev.key.code == this->konamiSequence[this->konamiIndex])
            {
                this->konamiIndex++;
                if (this->konamiIndex == this->konamiSequence.size())
                {
                    std::cout << "¡CODIGO KONAMI ACTIVADO!" << std::endl;
                    std::vector<int> fullInventory;
                    
                    for (int i = 0; i<4; i++){
                        fullInventory.push_back(0);
                    }
                    
                    this->jugador->setStats(99999, 99999, 9999, 67, fullInventory);
                    this->konamiIndex = 0;
                }
            }
            else
            {
                if (this->ev.key.code == this->konamiSequence[0])
                {
                    this->konamiIndex = 1;
                }
                else
                {
                    this->konamiIndex = 0;
                }
            }
            break;
        }

        if (this->gameState == STATE_PLAYING && this->ev.type == sf::Event::KeyReleased &&
            (this->ev.key.code == sf::Keyboard::W ||
             this->ev.key.code == sf::Keyboard::A ||
             this->ev.key.code == sf::Keyboard::S ||
             this->ev.key.code == sf::Keyboard::D))
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

    // Check de escotilla
    sf::FloatRect trapdoorBounds = this->habitacionActual->getTrapdoorBounds();
    if (trapdoorBounds.width > 0.f && this->jugador->getHitboxBounds().intersects(trapdoorBounds))
    {
        this->jugador->addLevelPiso();
        this->nextDungeonFloor();

        return;
    }

    bool roomChanged = false;
    sf::Vector2i nextRoom = this->currentRoomCoords;
    sf::Vector2f nextPlayerPos = this->jugador->getPosition();
    bool enemiesExist = !this->habitacionActual->getEnemigos().empty();

    float winWidth = this->window->getSize().x;
    float winHeight = this->window->getSize().y;

    TileMap *mapa = this->habitacionActual->getTileMap();
    if (mapa == nullptr)
        return;

    std::vector<Character *> personajes;
    personajes.push_back(this->jugador);
    for (auto *enemigo : this->habitacionActual->getEnemigos())
    {
        personajes.push_back(enemigo);
    }

    for (auto *p : personajes)
    {
        bool isPlayer = (p == this->jugador);
        sf::Vector2f vel = p->getVelocidadVector();

        // movimiento en X
        p->mover(vel.x, 0.f);
        sf::FloatRect boundsX = p->getHitboxBounds();
        bool colX = mapa->checkCollision(boundsX);

        // bordes de la ventana en X
        if (boundsX.left < 0)
        {
            if (isPlayer && this->habitacionActual->getRoomData().doors[3] &&
                (boundsX.top + boundsX.height / 2.f) > winHeight / 2 - 50 &&
                (boundsX.top + boundsX.height / 2.f) < winHeight / 2 + 50 &&
                !enemiesExist)
            {
                nextRoom.x--;
                nextPlayerPos.x = winWidth - boundsX.width - 10.f;
                roomChanged = true;
            }
            else
            {
                colX = true;
            }
        }
        else if (boundsX.left + boundsX.width > winWidth)
        {
            if (isPlayer && this->habitacionActual->getRoomData().doors[1] &&
                (boundsX.top + boundsX.height / 2.f) > winHeight / 2 - 50 &&
                (boundsX.top + boundsX.height / 2.f) < winHeight / 2 + 50 &&
                !enemiesExist)
            {
                nextRoom.x++;
                nextPlayerPos.x = 10.f;
                roomChanged = true;
            }
            else
            {
                colX = true;
            }
        }

        if (colX)
        {
            p->mover(-vel.x, 0.f);
        }

        // movimiento en Y
        p->mover(0.f, vel.y);
        sf::FloatRect boundsY = p->getHitboxBounds();
        bool colY = mapa->checkCollision(boundsY);

        // bordes de la ventana en Y
        if (boundsY.top < 0)
        {
            if (isPlayer && this->habitacionActual->getRoomData().doors[0] &&
                (boundsY.left + boundsY.width / 2.f) > winWidth / 2 - 50 &&
                (boundsY.left + boundsY.width / 2.f) < winWidth / 2 + 50 &&
                !enemiesExist)
            {
                nextRoom.y--;
                nextPlayerPos.y = winHeight - boundsY.height - 10.f - 60.f;
                roomChanged = true;
            }
            else
            {
                colY = true;
            }
        }
        else if (boundsY.top + boundsY.height > winHeight)
        {
            if (isPlayer && this->habitacionActual->getRoomData().doors[2] &&
                (boundsY.left + boundsY.width / 2.f) > winWidth / 2 - 50 &&
                (boundsY.left + boundsY.width / 2.f) < winWidth / 2 + 50 &&
                !enemiesExist)
            {
                nextRoom.y++;
                nextPlayerPos.y = 10.f - 60.f;
                roomChanged = true;
            }
            else
            {
                colY = true;
            }
        }

        if (colY)
        {
            p->mover(0.f, -vel.y);
        }
    }

    if (roomChanged)
    {
        this->currentRoomCoords = nextRoom;

        auto it = this->roomsMap.find(std::make_pair(this->currentRoomCoords.x, this->currentRoomCoords.y));
        if (it != this->roomsMap.end())
        {
            this->habitacionActual = it->second;
        }
        else
        {
            this->habitacionActual = new Habitacion(&this->tileSheet, this->dungeonGen->getRoom(this->currentRoomCoords.x, this->currentRoomCoords.y), this->jugador->getLevelPiso());
            if (this->clearedRooms.count(std::make_pair(this->currentRoomCoords.x, this->currentRoomCoords.y))) {
                this->habitacionActual->clearEnemies();
            }
            this->roomsMap[std::make_pair(this->currentRoomCoords.x, this->currentRoomCoords.y)] = this->habitacionActual;
        }

        this->jugador->setPosition(nextPlayerPos.x, nextPlayerPos.y);
    }
}

void Juego::update()
{
    this->pollEvents();

    if (!this->finalizarJuego && this->gameState == STATE_PLAYING)
    {
        this->updateInput();
        this->updatePersonajes();
        this->habitacionActual->update(this->jugador);
        this->updateCollision();
        this->hud->update(this->jugador);
    }

    // cuando termina el juego
    if (this->jugador->getHp() <= 0)
        this->gameState = STATE_GAMEOVER;
}

void Juego::render()
{
    this->window->clear();

    if (this->gameState == STATE_MENU)
    {
        this->mainMenu->draw(*this->window);
    }
    else if (this->gameState == STATE_PLAYING || this->gameState == STATE_PAUSE_MENU)
    {
        this->habitacionActual->renderFondo(*this->window);

        // todos en el vector
        std::vector<Character *> personajesParaRender;
        personajesParaRender.push_back(this->jugador);

        for (auto *enemigo : this->habitacionActual->getEnemigos())
        {
            personajesParaRender.push_back(enemigo);
        }

        // orden
        std::sort(personajesParaRender.begin(), personajesParaRender.end(),
                  [](Character *a, Character *b)
                  {
                      return a->getPosition().y < b->getPosition().y;
                  });

        // draw
        for (auto *personaje : personajesParaRender)
        {
            personaje->render(*this->window);
            // personaje->renderHitbox(*this->window);
        }

        this->hud->render(*this->window);

        if (this->gameState == STATE_PAUSE_MENU)
        {
            this->mainMenu->draw(*this->window);
        }
    }
    else if (this->gameState == STATE_GAMEOVER)
    {
        this->window->draw(this->gameOverText);
    }

    this->window->display();
}

int Juego::getPuntos()
{
    return 0; // para que es esto??????
}

void Juego::handleMenuInput(sf::Keyboard::Key key)
{
    if (key == sf::Keyboard::Up || key == sf::Keyboard::W)
    {
        this->mainMenu->moveUp();
    }
    else if (key == sf::Keyboard::Down || key == sf::Keyboard::S)
    {
        this->mainMenu->moveDown();
    }
    else if (key == sf::Keyboard::Enter || key == sf::Keyboard::Space)
    {
        int pressed = this->mainMenu->getPressedItem();
        MenuState menustate = this->mainMenu->getState();

        switch (menustate)
        {
        case MENU_MAIN:
            switch (pressed)
            {
            case 0:
                this->gameState = STATE_PLAYING;
                break;
            case 1:
                this->mainMenu->setState(MENU_LOAD);
                break;
            case 2:
                this->mainMenu->setState(MENU_SAVE);
                break;
            case 3:
                this->window->close();
                break;
            }
            break;
        case MENU_LOAD:
            if (pressed == 3)
            {
                this->mainMenu->setState(MENU_MAIN);
            }
            else
            {
                this->loadGame(pressed + 1);
            }
            break;
        case MENU_SAVE:
            if (pressed == 3)
            {
                this->mainMenu->setState(MENU_MAIN);
            }
            else
            {
                this->saveGame(pressed + 1);
                if (this->gameState == STATE_PAUSE_MENU)
                    this->gameState = STATE_PLAYING;
            }
            break;
        default:
            std::cout << "Error in Game_Class.cpp Juego::handleMenuInput()" << std::endl;
            this->window->close();
            break;
        }
    }
}

void Juego::saveGame(int slot)
{
    for (auto& pair : this->roomsMap) {
        if (pair.second->getEnemigos().empty() && pair.second->getRoomData().type != START) {
            this->clearedRooms.insert(pair.first);
        }
    }

    GameData gd;
    gd.hp = this->jugador->getHp();
    gd.maxHp = this->jugador->getMaxHp();
    gd.coins = this->jugador->getCoins();
    gd.statsUp = this->jugador->getStatsUp();
    gd.seed = this->seed;
    gd.currentRoomX = this->currentRoomCoords.x;
    gd.currentRoomY = this->currentRoomCoords.y;
    gd.floor = this->jugador->getLevelPiso();
    gd.clearedRooms = std::vector<std::pair<int, int>>(this->clearedRooms.begin(), this->clearedRooms.end());
    gd.inventory = this->jugador->getInventoryAsInt();
    SaveManager::saveGame(slot, gd);
}

void Juego::loadGame(int slot)
{
    if (SaveManager::saveExists(slot))
    {
        GameData gd = SaveManager::loadGame(slot);
        this->jugador->setStats(gd.hp, gd.maxHp, gd.coins, gd.statsUp, gd.inventory);
        this->jugador->setLevelPiso(gd.floor);
        this->seed = gd.seed;
        this->currentRoomCoords.x = gd.currentRoomX;
        this->currentRoomCoords.y = gd.currentRoomY;

        this->clearedRooms.clear();
        for (auto p : gd.clearedRooms) {
            this->clearedRooms.insert(p);
        }

        if (this->dungeonGen != nullptr)
            delete this->dungeonGen;
        this->dungeonGen = new DungeonGenerator(10, 10, 10);
        this->dungeonGen->generate(this->seed);

        for (auto &pair : this->roomsMap)
        {
            delete pair.second;
        }
        this->roomsMap.clear();

        this->habitacionActual = new Habitacion(&this->tileSheet,
                                                this->dungeonGen->getRoom(this->currentRoomCoords.x, this->currentRoomCoords.y),
                                                this->jugador->getLevelPiso());
        if (this->clearedRooms.count(std::make_pair(this->currentRoomCoords.x, this->currentRoomCoords.y))) {
            this->habitacionActual->clearEnemies();
        }
        this->roomsMap[std::make_pair(
            this->currentRoomCoords.x, this->currentRoomCoords.y)] = this->habitacionActual;
        this->gameState = STATE_PLAYING;
    }
}

void Juego::resetGame()
{
    if (this->jugador != nullptr)
    {
        delete this->jugador;
        this->jugador = nullptr;
    }

    if (this->dungeonGen != nullptr)
    {
        delete this->dungeonGen;
        this->dungeonGen = nullptr;
    }

    for (auto &pair : this->roomsMap)
    {
        delete pair.second;
    }
    this->roomsMap.clear();
    this->clearedRooms.clear();

    this->initPersonajes();
    this->initHabitacion();

    this->gameState = STATE_PLAYING;
    this->finalizarJuego = false;
}

void Juego::nextDungeonFloor()
{
    this->seed = static_cast<unsigned int>(rand());

    if (this->dungeonGen != nullptr)
    {
        delete this->dungeonGen;
        this->dungeonGen = nullptr;
    }

    for (auto &pair : this->roomsMap)
    {
        delete pair.second;
    }
    this->roomsMap.clear();
    this->clearedRooms.clear();

    this->dungeonGen = new DungeonGenerator(10, 10, 10); // Genera uno completamente nuevo
    this->dungeonGen->generate(this->seed);

    this->currentRoomCoords.x = this->dungeonGen->getWidth() / 2;
    this->currentRoomCoords.y = this->dungeonGen->getHeight() / 2;

    this->habitacionActual = new Habitacion(&this->tileSheet, this->dungeonGen->getRoom(this->currentRoomCoords.x, this->currentRoomCoords.y), this->jugador->getLevelPiso());
    this->roomsMap[std::make_pair(this->currentRoomCoords.x, this->currentRoomCoords.y)] = this->habitacionActual;

    // Posicionar en el centro de la nueva START_ROOM sin perder su data
    this->jugador->setPosition(400.f, 300.f);
}
