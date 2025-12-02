#include "MainMenu.h"
#include <iostream>

//README: ESTO FUE GENERADO CON UNA IA (Google Gemini)
// ¿Por que? Para ahorrar tiempo en lo que no es tan importante como el juego en si

MainMenu::MainMenu(float width, float height) {
    if(!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        // Handle error
    }
    
    state = MENU_MAIN;
    selectedIndex = 0;
    
    // Main Options
    std::vector<std::string> mainOpts = {"New Game", "Load Game", "Options", "Exit"};
    for(size_t i=0; i<mainOpts.size(); i++) {
        sf::Text text;
        text.setFont(font);
        text.setString(mainOpts[i]);
        text.setFillColor(sf::Color::White);
        text.setPosition(sf::Vector2f(width / 2 - 50, height / (mainOpts.size() + 1) * (i + 1)));
        mainOptions.push_back(text);
    }
    mainOptions[0].setFillColor(sf::Color::Red); // Selected
    
    // Load Options
    std::vector<std::string> loadOpts = {"Slot 1", "Slot 2", "Slot 3", "Back"};
    for(size_t i=0; i<loadOpts.size(); i++) {
        sf::Text text;
        text.setFont(font);
        text.setString(loadOpts[i]);
        text.setFillColor(sf::Color::White);
        text.setPosition(sf::Vector2f(width / 2 - 50, height / (loadOpts.size() + 1) * (i + 1)));
        loadOptions.push_back(text);
    }
    
    // Save Options
    std::vector<std::string> saveOpts = {"Save Slot 1", "Save Slot 2", "Save Slot 3", "Back"};
    for(size_t i=0; i<saveOpts.size(); i++) {
        sf::Text text;
        text.setFont(font);
        text.setString(saveOpts[i]);
        text.setFillColor(sf::Color::White);
        text.setPosition(sf::Vector2f(width / 2 - 50, height / (saveOpts.size() + 1) * (i + 1)));
        saveOptions.push_back(text);
    }
    
    // Options Options
    std::vector<std::string> optOpts = {"800x600", "1024x768", "Fullscreen: Toggle", "Back"};
    for(size_t i=0; i<optOpts.size(); i++) {
        sf::Text text;
        text.setFont(font);
        text.setString(optOpts[i]);
        text.setFillColor(sf::Color::White);
        text.setPosition(sf::Vector2f(width / 2 - 50, height / (optOpts.size() + 1) * (i + 1)));
        optionsOptions.push_back(text);
    }
}

MainMenu::~MainMenu() {}

void MainMenu::draw(sf::RenderWindow& window) {
    std::vector<sf::Text>* currentOptions = &mainOptions;
    if(state == MENU_LOAD) currentOptions = &loadOptions;
    else if(state == MENU_SAVE) currentOptions = &saveOptions;
    else if(state == MENU_OPTIONS) currentOptions = &optionsOptions;
    
    for(auto& text : *currentOptions) {
        window.draw(text);
    }
}

void MainMenu::moveUp() {
    std::vector<sf::Text>* currentOptions = &mainOptions;
    if(state == MENU_LOAD) currentOptions = &loadOptions;
    else if(state == MENU_SAVE) currentOptions = &saveOptions;
    else if(state == MENU_OPTIONS) currentOptions = &optionsOptions;

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
    else if(state == MENU_OPTIONS) currentOptions = &optionsOptions;

    if(selectedIndex + 1 < currentOptions->size()) {
        (*currentOptions)[selectedIndex].setFillColor(sf::Color::White);
        selectedIndex++;
        (*currentOptions)[selectedIndex].setFillColor(sf::Color::Red);
    }
}

void MainMenu::setState(MenuState s) {
    state = s;
    selectedIndex = 0;
    
    // Reset colors
    for(auto& t : mainOptions) t.setFillColor(sf::Color::White);
    for(auto& t : loadOptions) t.setFillColor(sf::Color::White);
    for(auto& t : saveOptions) t.setFillColor(sf::Color::White);
    for(auto& t : optionsOptions) t.setFillColor(sf::Color::White);
    
    if(state == MENU_MAIN) mainOptions[0].setFillColor(sf::Color::Red);
    else if(state == MENU_LOAD) loadOptions[0].setFillColor(sf::Color::Red);
    else if(state == MENU_SAVE) saveOptions[0].setFillColor(sf::Color::Red);
    else if(state == MENU_OPTIONS) optionsOptions[0].setFillColor(sf::Color::Red);
}
