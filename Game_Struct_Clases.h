#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

class Mazmorras{
private:
int idChunk;
int chanceAparicion;
int dificultad;

/*
vector<Mazmorras> chunks;
vector<Enemigos> enemigos;
vector<Objetos> tesoros;
*/
public:

};

class Personajes{
protected:
int id;
int hp;
int dmg;
int lvl;
std::string nombre;
sf::Sprite sprite;
sf::Texture textura;

enum class TipoPersonaje {Jugador, Enemigos, Boss};
enum class EstadoPersonaje {Normal, Atacando, Moviendose};

public:
    Personajes(int _id, int _hp, int _dmg, int _lvl, std::string _nombre, std::string archivoSprites);
    /*
    virtual void mover(float dx, float dy);
    virtual void atacar(Personajes& objetivo);
    virtual void recibirDanio(int cantidad);
    virtual void dibujar(sf::RenderWindow& window);*/

};

class Jugador : public Personajes{
private:
    /*
    vector<Inventario> inventario;
    */
public:
    Jugador();

    int getHp();

    void update();
    //Visual
    void draw(sf::RenderTarget& target);
};

class Mapa{
private:
int idMapa;
int seed;
int pisoActual;
int totalPisos;
int chunksTotales;
int enemigosTotales;
public:

};

class Enemigos : public Personajes{
private:
int idEnemigo;
int hpBase;
int dmgBase;
int lvlMin;
int lvlMax;
public:
    Enemigos();

};

class Inventario{
private:
int idJugador;
int idObjeto;
int cantidad;
public:

};

class Objetos{
private:
int idObjeto;
std::string nombre;
int tipo;
public:

};
