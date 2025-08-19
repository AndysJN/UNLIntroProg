#include <iostream>
#include <conio2.h>
#include <windows.h>

/**
*	Enumerador para identificar a quien pertenece el disparo.
**/

enum class Instigator
{
	Player,
	Enemy
};

/**
*	Entidad Base
**/

class Entity
{
public:
	void Kill() { bIsAlive = false ;}
	
protected:
	//Ubicacion en pantalla
	int X{0};
	int Y{0};
	//Ubicacion en pantalla
	
	bool bIsAlive = true;
	
public:
	bool GetIsAlive() const { return bIsAlive; }
};

/**
*	Clase Base de Jugador.
**/

//Por si quiero tener mas de un tipo de player.
class PlayerBase : Entity
{
	
protected:
	int Lives{3};
	
public:
	void LoseOneLife()
	{ 
		Lives--;
		if (Lives <= 0)
		{
			bIsAlive = false;
		}
	}
	
	void Spawn (int StartX, int StartY)
	{
		X = StartX;
		Y = StartY;
	}
	
	int GetLives() const { return Lives; }
	
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
	
	//Puedo implmementar diferentes comportamientos dependiendo el enemigo?
	//Algunos se pueden poner un poco mas fuertes al recibir disparos ?
	//Podria ser que por cada disparo recibido aumente la cantidad de puntos?
	//Podrian reaccion al disparo iniciando un disparo en forma de represaria
	//al morir?
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
		PointsToGrant = 100;
		Shape = 'A';
	}
};

/***** Enemigo B *****/

class EnemyB : public EnemyBase
{
	EnemyB ()
	{
		PointsToGrant = 200;
		Shape = 'B';
	}
};

/***** Enemigo C *****/

class EnemyC : public EnemyBase
{
	EnemyC ()
	{
		PointsToGrant = 500;
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


/**
*	Clase para manejar la logica del juego 
**/

class Game
{
	
public:
	int Score{0};
	bool Running = true;
	bool Victory = false;
	
private:
	void Initializate()
	{
		
	}
	
	void ShowStartScreen()
	{
		
	}
	
	void ShowEndScreen()
	{
		
	}
	
	void ShowHUD()
	{
		
	}
};

int main() 
{
	
	return 0;
}

/** Funciones de ayuda **/

