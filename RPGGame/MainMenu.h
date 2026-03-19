#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

enum MenuState {
    MENU_MAIN,
    MENU_LOAD,
    MENU_SAVE,
    MENU_OPTIONS
};

enum MENU_MAIN_OPITONS {NEW_GAME = 0, LOAD_GAME, OPTIONS, EXIT};
enum MENU_LOAD_OPITONS {LOAD_GAME_1 = 0, LOAD_GAME_2, LOAD_GAME_3};
enum MENU_SAVE_OPITONS {SAVE_GAME_1 = 0, SAVE_GAME_2, SAVE_GAME_3};
enum MENU_OPTIONS_OPITONS {RES_DEF = 0, RES_1024, FULLSCREEN};
const int BACK = 3;

class MainMenu {
private:
    sf::Font font;
    std::vector<sf::Text> mainOptions;
    std::vector<sf::Text> loadOptions;
    std::vector<sf::Text> saveOptions;
    std::vector<sf::Text> optionsOptions;
    
    int selectedIndex;
    MenuState state;
    
    void initFonts();
    void initText();

public:
    MainMenu(float width, float height);
    ~MainMenu();

    void draw(sf::RenderWindow& window);
    void moveUp();
    void moveDown();
    
    int getPressedItem() const { return selectedIndex; }
    MenuState getState() const { return state; }
    void setState(MenuState s);
};
