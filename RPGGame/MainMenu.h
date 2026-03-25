#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

enum MenuState {
    MENU_MAIN,
    MENU_LOAD,
    MENU_SAVE,
};



class MainMenu {
public:
    enum class MainOption { NewGame = 0, LoadGame, Options, Exit };
    enum class LoadOption { Slot1 = 0, Slot2, Slot3, Back };
    enum class SaveOption { Slot1 = 0, Slot2, Slot3, Back };

private:
    sf::Font font;
    std::vector<sf::Text> mainOptions;
    std::vector<sf::Text> loadOptions;
    std::vector<sf::Text> saveOptions;
    
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
