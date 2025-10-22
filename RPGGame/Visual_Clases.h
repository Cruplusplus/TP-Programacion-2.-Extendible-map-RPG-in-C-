#pragma once

#include <cstdlib>
#include <ctime>
#include <iostream>
#include "time.h"

#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

class VisualConfig
{
private:
    int resolucionX;
    int resolucionY;
    int frameRate;
    int volumenMusica;
public:
    VisualConfig();

    int getResolucionX() const;
    int getResolucionY() const;
    int getFramerate() const;

};

class RecursosVisuales  //{Sprites + TilesMapa +Animaciones}
{
private:
    int idRecurso;
    std::string rutaSprite;
    int inicioSpriteX;
    int inicioSpriteY;
    int frameWidth;
    int frameHeight;
    sf::Texture texture;
    sf::Sprite sprite;
public:
    RecursosVisuales();
    RecursosVisuales(int _idRecurso, std::string _rutaSprite, int _inicioSpriteX,
                     int _inicioSpriteY, int _frameWidth, int _frameHeight);

    int getIdRecurso() const;
    std::string getRutaSprite() const;
    int getInicioSpriteX() const;
    int getInicioSpriteY() const;
    int getFrameWidth() const;
    int getFrameHeight() const;
    sf::Texture& getTexture();
    sf::Sprite& getSprite();
};

