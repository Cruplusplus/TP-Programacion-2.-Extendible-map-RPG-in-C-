#include <iostream>
using namespace std;

class Mazmorras{
private:
int idChunk;
int chanceAparicion;
int dificultad;
public:

};

class Personajes{
private:
int idPersonaje;
int hp;
int dmg;
int lvl;
int tipo; //(0 = jugador, 1 = enemigo, 2 = boss)
public:

};

class Enemigos : public Personajes{
private:
int idEnemigo;
char nombre[30];
int hpBase;
int dmgBase;
int lvlMin;
int lvlMax;
public:

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

class SavePartida{
private:
int idPartida;
int idJugador;
int idMapa;
int pisoActual;
int hpActual;
bool inventarioGuardado;
public:

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
char nombre[30];
int tipo;
public:

};

class RecursosVisuales{ //{Sprites + TilesMapa +Animaciones}
private:
int idRecurso;
char rutaArchivo[100];
int ancho;
int alto;
char tipo;
bool camina;
public:

};

class VisualConfig{
private:
int resolucionX;
int resolucionY;
int volumenMusica;
public:

};

int main()
{



return 0;
}
