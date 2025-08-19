#include <iostream>
#include <conio2.h>
#include <windows.h>

/**
*	Entidad Base
**/

class Entity
{
protected:
	bool bIsAlive = true;
	
public:
	bool IsAlive() const { return bIsAlive; }
};

/**

Derivar Jugador, Enemigo y Proyectil de Clase Base

**/

//Por si quiero tener mas de un tipo de player.
Class PlayerBase : Entity
{
	
};

//Voy a tener mas de un tipo de enemigo.
Class EnemyBase : Entity
{
	
};

Class Projectile : Entity
{
	
};

/**

Crear una configuracion para la pantalla

Crear una clase para el framework del juego (Puntaje, manejo de enemigos)

**/

int main(int argc, char *argv[]) {
	
	return 0;
}

