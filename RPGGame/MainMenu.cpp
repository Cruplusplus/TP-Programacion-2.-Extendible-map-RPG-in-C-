#include "MainMenu.h"
#include <iostream>

MainMenu::MainMenu(float width, float height) {
    if(!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cout << "Error al cargar la fuente!! MainMenu.cpp MainMenu()" << std::endl;
    }
    
    state = MENU_MAIN;
    selectedIndex = 0;
    
    std::vector<std::string> mainOpts = {"Play Game", "Load Game", "Save Game", "Exit"};
    for(size_t i=0; i<mainOpts.size(); i++) {
        sf::Text text;
        text.setFont(font);
        text.setString(mainOpts[i]);
        text.setFillColor(sf::Color::White);
        text.setPosition(sf::Vector2f(width / 2 - 50, height / (mainOpts.size() + 1) * (i + 1)));
        mainOptions.push_back(text);
    }
    mainOptions[0].setFillColor(sf::Color::Red); // Selected
    
    std::vector<std::string> loadOpts = {"Slot 1", "Slot 2", "Slot 3", "Back"};
    for(size_t i=0; i<loadOpts.size(); i++) {
        sf::Text text;
        text.setFont(font);
        text.setString(loadOpts[i]);
        text.setFillColor(sf::Color::White);
        text.setPosition(sf::Vector2f(width / 2 - 50, height / (loadOpts.size() + 1) * (i + 1)));
        loadOptions.push_back(text);
    }
    
    std::vector<std::string> saveOpts = {"Save Slot 1", "Save Slot 2", "Save Slot 3", "Back"};
    for(size_t i=0; i<saveOpts.size(); i++) {
        sf::Text text;
        text.setFont(font);
        text.setString(saveOpts[i]);
        text.setFillColor(sf::Color::White);
        text.setPosition(sf::Vector2f(width / 2 - 50, height / (saveOpts.size() + 1) * (i + 1)));
        saveOptions.push_back(text);
    }
}

MainMenu::~MainMenu() {}

void MainMenu::draw(sf::RenderWindow& window) {
    std::vector<sf::Text>* currentOptions = &mainOptions;
    if(state == MENU_LOAD) currentOptions = &loadOptions;
    else if(state == MENU_SAVE) currentOptions = &saveOptions;
    
    for(auto& text : *currentOptions) {
        window.draw(text);
    }
}

void MainMenu::moveUp() {
    std::vector<sf::Text>* currentOptions = &mainOptions;
    if(state == MENU_LOAD) currentOptions = &loadOptions;
    else if(state == MENU_SAVE) currentOptions = &saveOptions;

    if(selectedIndex - 1 >= 0) {
        (*currentOptions)[selectedIndex].setFillColor(sf::Color::White);
        selectedIndex--;
        (*currentOptions)[selectedIndex].setFillColor(sf::Color::Red);
    }
}

void MainMenu::moveDown() {
    std::vector<sf::Text>* currentOptions = &mainOptions;
    if(state == MENU_LOAD) currentOptions = &loadOptions;
    else if(state == MENU_SAVE) currentOptions = &saveOptions;

    if(selectedIndex + 1 < currentOptions->size()) {
        (*currentOptions)[selectedIndex].setFillColor(sf::Color::White);
        selectedIndex++;
        (*currentOptions)[selectedIndex].setFillColor(sf::Color::Red);
    }
}

void MainMenu::setState(MenuState s) {
    state = s;
    selectedIndex = 0;
    
    for(auto& t : mainOptions) t.setFillColor(sf::Color::White);
    for(auto& t : loadOptions) t.setFillColor(sf::Color::White);
    for(auto& t : saveOptions) t.setFillColor(sf::Color::White);
    
    if(state == MENU_MAIN) mainOptions[0].setFillColor(sf::Color::Red);
    else if(state == MENU_LOAD) loadOptions[0].setFillColor(sf::Color::Red);
    else if(state == MENU_SAVE) saveOptions[0].setFillColor(sf::Color::Red);
}
