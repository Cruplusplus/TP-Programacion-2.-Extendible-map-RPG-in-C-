# TP-Programacion-2.-Extendible-map-RPG-in-C-
Crear un juego en C++ con recursos visuales el cual tenga una jugabilidad con mapa extensible al estilo de "chunks"

Classes to use:
*Mazmorras
idChunk int
chanceAparicion int
dificultad int

*Personajes
idPersonaje int
hp int
dmg int
lvl int
tipo int (0 = jugador, 1 = enemigo, 2 = boss)

*Enemigos : Personajes
idEnemigo int
nombre char[30]
hpBase int
dmgBase int
lvlMin int
lvlMax int

Mapa {+Piso}
idMapa int
seed int
pisoActual int
totalPisos int
chunksTotales int
enemigosTotales int

*SavePartida
idPartida int
idJugador int
idMapa int
pisoActual int
hpActual int
inventarioGuardado bool

*Inventario
idJugador int
idObjeto int
cantidad int

*Objetos
idObjeto int
nombre char[30]
tipo int

RecursosVisuales{Sprites + TilesMapa +Animaciones}
idRecurso int
rutaArchivo char[100]
ancho int
alto int
tipo char
camina bool

*VisualConfig
resolucionX int
resolucionY int
volumenMusica int
