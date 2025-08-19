#include <iostream>
#include <conio2.h>
#include <windows.h>

/**
*	Entidad Base
**/

class Entity
{
public:
	void Kill() { bIsAlive = false ;}
	
protected:
	bool bIsAlive = true;
	
public:
	bool GetIsAlive() const { return bIsAlive; }
};

/**

Derivar Jugador, Enemigo y Proyectil de Clase Base

**/

//Por si quiero tener mas de un tipo de player.
class PlayerBase : Entity
{
	
};

/**
*	Clase Base de Enemigo y Derivadas.
**/

class EnemyBase : Entity
{
public:
	int HitPoints{1};
	int PointsToGrant{0};
	char Shape { 'E' };
	
	//La voy a marcar virtual por si quiero implementar algo distinto en hijos
	virtual void OnHit()
	{
		HitPoints--;
		if (HitPoints <= 0)
		{
			bIsAlive = false;
		}
	}
	
public:
		int GetPointsToGrant() {return PointsToGrant;}
};

/***** Enemigo A *****/

class EnemyA : public EnemyBase
{
	EnemyA ()
	{
		PointsToGrant = 10;
		Shape = 'A';
	}
};

/***** Enemigo B *****/

class EnemyB : public EnemyBase
{
	EnemyB ()
	{
		PointsToGrant = 20;
		Shape = 'B';
	}
};

/***** Enemigo C *****/

class EnemyC : public EnemyBase
{
	EnemyC ()
	{
		PointsToGrant = 30;
		Shape = 'C';
	}
};

/** Projectile **/

class Projectile : Entity
{
	
};

/**

Crear una configuracion para la pantalla

Crear una clase para el framework del juego (Puntaje, manejo de enemigos)

**/

class Game
{
	
public:
	int Score{0};
	bool Running = true;
	bool Victory = false;
};

int main() 
{
	
	return 0;
}

/** Funciones de ayuda **/

