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



