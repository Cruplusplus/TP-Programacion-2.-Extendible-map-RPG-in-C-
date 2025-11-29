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
    this->seed = static_cast<unsigned>(time(0));
    this->dungeonGen = new DungeonGenerator(10, 10, 5); // 10x10 grid, 5 rooms
    this->dungeonGen->generate(this->seed);
    
    // Find start room
    for(int x=0; x<this->dungeonGen->getWidth(); x++) {
        for(int y=0; y<this->dungeonGen->getHeight(); y++) {
            if(this->dungeonGen->getRoom(x, y).type == START) {
                this->currentRoomCoords = sf::Vector2i(x, y);
                break;
            }
        }
    }

    this->habitacionActual = new Habitacion(&this->tileSheet, this->dungeonGen->getRoom(this->currentRoomCoords.x, this->currentRoomCoords.y));
    this->roomsMap[std::make_pair(this->currentRoomCoords.x, this->currentRoomCoords.y)] = this->habitacionActual;
}

Juego::Juego()
{
    this->initVariables();
    this->initWindow();
    this->initInput();
    this->initTileSheet();
    this->initFonts();
    this->initPersonajes();
    this->initHabitacion();
    this->hud = new HUD();
    this->mainMenu = new MainMenu(this->window->getSize().x, this->window->getSize().y);
    this->gameState = STATE_MENU;
}

Juego::~Juego()
{
    delete this->window;
    delete this->jugador;
    delete this->dungeonGen;
    delete this->hud;
    delete this->mainMenu;
    // delete this->habitacionActual; // Already in map
    
    for(auto const& [key, val] : this->roomsMap) {
        delete val;
    }
    this->roomsMap.clear();
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
        this->habitacionActual->getTileMap()->addTile(mouseX, mouseY, tipoTiles::ROCA);
    }
    else if(sf::Mouse::isButtonPressed(this->mouseMappings["BTN_REMOVE_TILE"]))
    {
        this->habitacionActual->getTileMap()->removeTile(mouseX, mouseY);
    }
    
    // Temp Save/Load
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::F5)) {
        this->saveGame(1);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::F9)) {
        this->loadGame(1);
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
                
            if(this->gameState == STATE_MENU) {
                if(this->ev.key.code == sf::Keyboard::W || this->ev.key.code == sf::Keyboard::Up) {
                    this->mainMenu->moveUp();
                }
                if(this->ev.key.code == sf::Keyboard::S || this->ev.key.code == sf::Keyboard::Down) {
                    this->mainMenu->moveDown();
                }
                if(this->ev.key.code == sf::Keyboard::Enter) {
                    int selected = this->mainMenu->getPressedItem();
                    MenuState mState = this->mainMenu->getState();
                    
                    if(mState == MENU_MAIN) {
                        if(selected == 0) { // New Game
                            this->gameState = STATE_PLAYING;
                            // Reset game logic if needed
                        }
                        else if(selected == 1) { // Load Game
                            this->mainMenu->setState(MENU_LOAD);
                        }
                        else if(selected == 2) { // Options
                            this->mainMenu->setState(MENU_OPTIONS);
                        }
                        else if(selected == 3) { // Exit
                            this->window->close();
                        }
                    }
                    else if(mState == MENU_LOAD) {
                        if(selected == 0) { this->loadGame(1); this->gameState = STATE_PLAYING; }
                        else if(selected == 1) { this->loadGame(2); this->gameState = STATE_PLAYING; }
                        else if(selected == 2) { this->loadGame(3); this->gameState = STATE_PLAYING; }
                        else if(selected == 3) { this->mainMenu->setState(MENU_MAIN); }
                    }
                    else if(mState == MENU_OPTIONS) {
                        if(selected == 3) { this->mainMenu->setState(MENU_MAIN); }
                        // Handle other options (res, fullscreen)
                        if(selected == 0) { 
                            this->window->setSize(sf::Vector2u(800, 600)); 
                            SaveManager::saveConfig(800, 600, false);
                        }
                        if(selected == 1) { 
                            this->window->setSize(sf::Vector2u(1024, 768)); 
                            SaveManager::saveConfig(1024, 768, false);
                        }
                    }
                }
            }
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
    //bordes de la ventana y transiciones
    //bordes de la ventana y transiciones
    bool roomChanged = false;
    sf::Vector2i nextRoom = this->currentRoomCoords;
    sf::Vector2f nextPlayerPos = this->jugador->getPosition();

    sf::FloatRect playerBounds = this->jugador->getHitboxBounds();
    float playerCenterX = playerBounds.left + playerBounds.width / 2.f;
    float playerCenterY = playerBounds.top + playerBounds.height / 2.f;

    // Check if enemies exist
    bool enemiesExist = !this->habitacionActual->getEnemigos().empty();

    // Bottom
    if(playerBounds.top + playerBounds.height > this->window->getSize().y)
    {
        if(this->habitacionActual->getRoomData().doors[2] && 
           playerCenterX > this->window->getSize().x/2 - 50 && 
           playerCenterX < this->window->getSize().x/2 + 50 &&
           !enemiesExist) {
            nextRoom.y++;
            nextPlayerPos.y = 10.f - 60.f; // Adjust for hitbox offset (y+60)
            roomChanged = true;
        } else {
            this->jugador->setPosition(
                this->jugador->getPosition().x,
                this->window->getSize().y - playerBounds.height - 60.f
            );
        }
    }

    // Top
    if(playerBounds.top < 0)
    {
        if(this->habitacionActual->getRoomData().doors[0] && 
           playerCenterX > this->window->getSize().x/2 - 50 && 
           playerCenterX < this->window->getSize().x/2 + 50 &&
           !enemiesExist) {
            nextRoom.y--;
            nextPlayerPos.y = this->window->getSize().y - playerBounds.height - 10.f - 60.f;
            roomChanged = true;
        } else {
            this->jugador->setPosition(
                this->jugador->getPosition().x, 0 - 60.f
            );
        }
    }

    // Right
    if(playerBounds.left + playerBounds.width > this->window->getSize().x)
    {
        if(this->habitacionActual->getRoomData().doors[1] && 
           playerCenterY > this->window->getSize().y/2 - 50 && 
           playerCenterY < this->window->getSize().y/2 + 50 &&
           !enemiesExist) {
            nextRoom.x++;
            nextPlayerPos.x = 10.f;
            roomChanged = true;
        } else {
            this->jugador->setPosition(
                this->window->getSize().x - playerBounds.width,
                this->jugador->getPosition().y
            );
        }
    }

    // Left
    if(playerBounds.left < 0)
    {
        if(this->habitacionActual->getRoomData().doors[3] && 
           playerCenterY > this->window->getSize().y/2 - 50 && 
           playerCenterY < this->window->getSize().y/2 + 50 &&
           !enemiesExist) {
            nextRoom.x--;
            nextPlayerPos.x = this->window->getSize().x - playerBounds.width - 10.f;
            roomChanged = true;
        } else {
            this->jugador->setPosition(
                0, this->jugador->getPosition().y
            );
        }
    }

    if(roomChanged) {
        // Do NOT delete habitacionActual, it is stored in the map
        this->currentRoomCoords = nextRoom;
        
        // Check if room exists in map
        auto it = this->roomsMap.find(std::make_pair(this->currentRoomCoords.x, this->currentRoomCoords.y));
        if(it != this->roomsMap.end()) {
            this->habitacionActual = it->second;
        } else {
            this->habitacionActual = new Habitacion(&this->tileSheet, this->dungeonGen->getRoom(this->currentRoomCoords.x, this->currentRoomCoords.y));
            this->roomsMap[std::make_pair(this->currentRoomCoords.x, this->currentRoomCoords.y)] = this->habitacionActual;
        }

        this->jugador->setPosition(nextPlayerPos.x, nextPlayerPos.y);
        return; // Skip collision check for this frame to avoid getting stuck
    }

    //colision con objetos del mapa
    TileMap* mapa = this->habitacionActual->getTileMap();
    if (mapa == nullptr) return; //ya veo que hacemos cagada

    std::vector<Character*> personajes;
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

}

void Juego::update()
{
    this->pollEvents();

    if(this->gameState == STATE_MENU) {
        // Menu Input
        if (this->ev.type == sf::Event::KeyPressed) {
             // Need to handle key press once per frame, pollEvents handles it but we need to check here or in pollEvents
             // Ideally move menu input to pollEvents or separate function
        }
        return;
    }

    if (!this->finalizarJuego && this->gameState == STATE_PLAYING)
    {
        this->updateInput();

        this->updatePersonajes();

        this->habitacionActual->update(this->jugador);

        this->updateCollision();

    }

    //cuando termina el juego
    if(this-> jugador->getHp() <= 0) {
        // this->finalizarJuego = true;
        this->gameState = STATE_GAMEOVER;
    }

}

void Juego::render()
{
    this->window->clear();

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
        this->window->setFramerateLimit(1);
        sf::sleep(sf::seconds(2));
        this->window->setFramerateLimit(60);
        this->resetGame();
    }

    if(this->gameState == STATE_MENU) {
        this->mainMenu->draw(*this->window);
    }
    else if(this->gameState == STATE_PLAYING || this->gameState == STATE_GAMEOVER) {
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
        }
        
        this->hud->update(this->jugador);
        this->hud->render(*this->window);
        
        if(this->gameState == STATE_GAMEOVER) {
            this->window->draw(this->gameOverText);
        }
    }

    this->window->display();
}

void Juego::saveGame(int slot) {
    if(!this->jugador || !this->dungeonGen) return;
    
    GameData data;
    data.hp = this->jugador->getHp();
    data.maxHp = this->jugador->getMaxHp();
    data.coins = this->jugador->getCoins();
    data.keys = this->jugador->getKeys();
    data.inventory = this->jugador->getInventoryAsInt();
    
    // We need to store the seed to regenerate the dungeon
    // For now, let's assume we stored the seed somewhere or we can just save the current room
    // Ideally DungeonGenerator should store its seed.
    // Let's add a getter for seed in DungeonGenerator or just store it in Game_Class when generating
    data.seed = this->seed; 
    data.currentRoomX = this->currentRoomCoords.x;
    data.currentRoomY = this->currentRoomCoords.y;
    
    SaveManager::saveGame(slot, data);
}

void Juego::loadGame(int slot) {
    if(!SaveManager::saveExists(slot)) return;
    
    GameData data = SaveManager::loadGame(slot);
    
    // Re-init dungeon with saved seed
    delete this->dungeonGen;
    this->dungeonGen = new DungeonGenerator(10, 10, 5);
    this->dungeonGen->generate(data.seed); // Use saved seed
    
    this->currentRoomCoords = sf::Vector2i(data.currentRoomX, data.currentRoomY);
    
    delete this->habitacionActual;
    this->habitacionActual = new Habitacion(&this->tileSheet, this->dungeonGen->getRoom(this->currentRoomCoords.x, this->currentRoomCoords.y));
    
    // Restore player stats
    // Restore player stats
    this->jugador->setStats(data.hp, data.maxHp, data.coins, data.keys, data.inventory);
}

void Juego::initFonts()
{
    if(!this->font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        std::cout << "ERROR: COULD NOT LOAD FONT" << std::endl;
    }

    this->gameOverText.setFont(this->font);
    this->gameOverText.setString("GAME OVER\nPress R to Restart");
    this->gameOverText.setCharacterSize(60);
    this->gameOverText.setFillColor(sf::Color::Red);
    this->gameOverText.setStyle(sf::Text::Bold);
    this->gameOverText.setPosition(
        this->window->getSize().x / 2.f - this->gameOverText.getGlobalBounds().width / 2.f,
        this->window->getSize().y / 2.f - this->gameOverText.getGlobalBounds().height / 2.f
    );
}

void Juego::resetGame()
{
    delete this->jugador;
    delete this->dungeonGen;
    
    // Clear rooms
    for(auto const& [key, val] : this->roomsMap) {
        delete val;
    }
    this->roomsMap.clear();

    this->initPersonajes();
    this->initHabitacion();
    
    this->gameState = STATE_PLAYING;
    this->finalizarJuego = false;
}
