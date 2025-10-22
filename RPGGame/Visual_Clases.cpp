#include "Visual_Clases.h"

VisualConfig::VisualConfig()
{
    this->resolucionX = 800;
    this->resolucionY = 800;
    this->frameRate = 61;
    this->volumenMusica = 100;
}

VisualConfig::getResolucionX() const { return this->resolucionX; }
VisualConfig::getResolucionY() const { return this->resolucionY; }
VisualConfig::getFramerate() const { return this->frameRate; }

RecursosVisuales::RecursosVisuales() : idRecurso(0), rutaSprite(""), inicioSpriteX(0),
                                        inicioSpriteY(0), frameWidth(0), frameHeight(0) {}

RecursosVisuales::RecursosVisuales(int _idRecurso, std::string _rutaSprite,int _inicioSpriteX,
                                   int _inicioSpriteY, int _frameWidth, int _frameHeight)
{
    this->idRecurso = _idRecurso;
    this->rutaSprite = _rutaSprite;
    this->inicioSpriteX = _inicioSpriteX;
    this->inicioSpriteY = _inicioSpriteY;
    this->frameWidth = _frameWidth;
    this->frameHeight = _frameHeight;

    if (!this->texture.loadFromFile(rutaSprite)) {
        std::cout << "ERROR CON LA CARGA DE TEXTURA: " << rutaSprite << std::endl;
    }
    texture.setSmooth(false);
    sprite.setTexture(this->texture);
    sprite.setTextureRect(sf::IntRect(this->inicioSpriteX, this->inicioSpriteY,
                                      this->frameWidth, this->frameHeight));
    sprite.setScale(sf::Vector2f(3.5f, 3.5f));
}

int RecursosVisuales::getIdRecurso() const { return idRecurso; }
std::string RecursosVisuales::getRutaSprite() const { return rutaSprite; }
int RecursosVisuales::getInicioSpriteX() const { return inicioSpriteX; }
int RecursosVisuales::getInicioSpriteY() const { return inicioSpriteY; }
int RecursosVisuales::getFrameWidth() const { return frameWidth; }
int RecursosVisuales::getFrameHeight() const { return frameHeight; }
sf::Texture& RecursosVisuales::getTexture() { return texture; }
sf::Sprite& RecursosVisuales::getSprite() { return sprite; }

