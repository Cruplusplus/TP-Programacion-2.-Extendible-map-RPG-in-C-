#include "GameConfig.h"
#include "SaveManager.h"

VisualConfig::VisualConfig()
{
    int width, height;
    bool fullscreen;
    SaveManager::loadConfig(width, height, fullscreen);
    
    this->resolucionX = height; 
    this->resolucionY = width;
    
    this->frameRate = 60;
    this->volumenMusica = 100;
}

int VisualConfig::getResolucionX() const { return this->resolucionX; }
int VisualConfig::getResolucionY() const { return this->resolucionY; }
int VisualConfig::getFramerate() const { return this->frameRate; }

