#include "GameConfig.h"
#include "SaveManager.h"

VisualConfig::VisualConfig()
{
    int w, h;
    bool fs;
    SaveManager::loadConfig(w, h, fs);
    
    this->resolucionX = h; 
    this->resolucionY = w;
    
    this->frameRate = 60;
    this->volumenMusica = 100;
}

int VisualConfig::getResolucionX() const { return this->resolucionX; }
int VisualConfig::getResolucionY() const { return this->resolucionY; }
int VisualConfig::getFramerate() const { return this->frameRate; }

