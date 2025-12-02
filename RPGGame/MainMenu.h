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
