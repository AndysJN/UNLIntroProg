#include <iostream>
#include <conio2.h>
#include <windows.h>
#include <ctime>

/**
*	Enumerador para identificar a quien pertenece el disparo.
**/

enum class Owner
{
	Player,
	Enemy
};

/**
*	Seteo Dimensiones, limites y borde de pantalla
**/

class Screen
	
{
	
public:

static const int BordeSup = 1;
static const int BordeInf = 25;
static const int BordeIzq = 1;
static const int BordeDer = 100;

static void DrawBorders()
{
	for (int x = BordeIzq; x <= BordeDer; ++x) 
	{
		putchxy(x, BordeInf, '.');
	}
	
	for (int y = BordeSup; y <= BordeInf; ++y) 
	{
		putchxy(BordeIzq, y, '.');
		putchxy(BordeDer, y, '.');
	}
}

static void Erase(int X, int Y)
{
	putchxy(X, Y, ' ');
}

static void Draw(int X, int Y, int Color, char Shape)
{
	textcolor(Color);
	putchxy(X, Y, Shape);
}

};

/**
*	Entidad Base
**/

class Entity
{
public:
	void Kill() { bIsAlive = false ;}
	void Spawn (int StartX, int StartY, int Color)
	{
		X = StartX;
		Y = StartY;
		Screen::Draw(X,Y, Color, Shape);
	}
	
protected:
	//Ubicacion en pantalla
	int X{0};
	int Y{0};
	//Ubicacion en pantalla
	char Shape{};
	int Color = {WHITE};
	
	bool bIsAlive = true;
	
public:
	
	bool GetIsAlive() const { return bIsAlive; }
	int GetColor() const { return Color; }
	int GetX() const { return X; }
	int GetY() const { return Y; }
	char GetShape() const { return Shape; }
	
	void NewPos(int NewX, int NewY) 
	{ 
		X = NewX;
		Y = NewY;
	}
	
	virtual ~Entity() = default;
};

/** Projectile **/

class Projectile : public Entity
{
public:
	
	Projectile()
	{
		Shape = '|';
		bIsAlive = false;
		Color = RED;
	}
	
	void Initialize(int StartX, int StartY, Owner InInsigator)
	{
		X = StartX;
		Y = StartY;
		Instigator = InInsigator;
		bIsAlive = true;
		Screen::Draw(X, Y, Color, Shape);
	}
	
	void Movement()
	{
		if (!bIsAlive)
		{
			return;
		}
		
		Screen::Erase(X, Y);
		Y += (Instigator == Owner::Player) ? -1 : 1;
		
		if (Y <= Screen::BordeSup + 1 || Y >= Screen::BordeInf -1)
		{
			bIsAlive = false;
			return;
		}
		Screen::Draw(X, Y, Color, Shape);
	}
	
	Owner GetOwner() { return Instigator; }
	
private:
	Owner Instigator;
	
};


/**
*	Clase Base de Jugador.
**/

//Por si quiero tener mas de un tipo de player.
class PlayerBase : public Entity
{
	
protected:
	int Lives{3};
	Projectile* MyProjectile = nullptr;
	
public:
	PlayerBase ()
	{
			Shape = 'A';
			Color = LIGHTBLUE;
	}
	
	void MoveLeft ()
	{
		if (X > Screen::BordeIzq + 1)
		{
			Screen::Erase(X, Y);
			--X;
			Screen::Draw(X, Y, Color, Shape);
		}
	}
	
	void MoveRight()
	{
		if (X < Screen::BordeDer - 1)
		{
			Screen::Erase(X, Y);
			++X;
			Screen::Draw(X, Y, Color, Shape);
		}
	}
	
	void Shoot(Projectile* InProjectile)
	{
		if (!InProjectile)
		{
			return;
		}
		
		if (InProjectile->GetIsAlive())
		{
			return;
		}
		
		MyProjectile = InProjectile;
		MyProjectile->Initialize(X, Y - 1, Owner::Player);
	}
	
	void LoseOneLife()
	{ 
		Lives--;
		if (Lives <= 0)
		{
			bIsAlive = false;
		}
	}
	
	int GetLives() const { return Lives; }
	
};

/**
*	Clase Base de Enemigo y Derivadas.
**/

class EnemyBase : public Entity
{
public:
	int HitPoints{1};
	int PointsToGrant{0};
	
	EnemyBase ()
	{
		Shape = 'E' ;
	}
	
	
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
	
public:
	EnemyA ()
	{
		PointsToGrant = 100;
		Shape = 'H';
		Color = LIGHTGRAY;
	}
};

/***** Enemigo B *****/

class EnemyB : public EnemyBase
{
public:
	EnemyB ()
	{
		PointsToGrant = 200;
		Shape = 'M';
		Color = YELLOW;
	}
};

/***** Enemigo C *****/

class EnemyC : public EnemyBase
{
public:
	EnemyC ()
	{
		PointsToGrant = 500;
		Shape = 'W';
		Color = LIGHTMAGENTA;
	}
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
	//normalizar la velocidad de movimiento
	
	const int Velocidad = 15;
	clock_t Paso = CLOCKS_PER_SEC / Velocidad;
	clock_t Tempo = clock();
	
	// ====================================
	
	int Score{0};
	bool bIsRunning = true;
	bool bVictory = false;
	
	void HandleInput()
	{
		if (kbhit())
		{
			char Key = getch();
			if (Key == 'q')
			{
				bIsRunning = false;
			}
				if (Key == 'a')
				{
					Player->MoveLeft();
				}
				else if (Key == 'd')
				{
					Player->MoveRight();
				}
				else if (Key == ' ')
				{
					//Implementar disparo
					Player->Shoot(PlayerProjectile);
				}
		}
	}
	
	void Play()
	{
		Initializate();
		while (bIsRunning)
		{
		if (clock() - Tempo >= Paso)
		{
			HandleInput();
			if (PlayerProjectile && PlayerProjectile->GetIsAlive())
			{
				PlayerProjectile->Movement();
				HandleProjectileCollision(PlayerProjectile);
			}
			
			EnemyTickCount++;
			if (EnemyTickCount >= EnemyTicksToBeMoved)
			{
			MoveEnemies();
			EnemyTickCount = 0;
			}

			Tempo += Paso;
		}
		
		
		Sleep(1);
		}
		
		ShowEndScreen(bVictory);
	}
	
	~Game()
	{
		delete Player;
		delete PlayerProjectile;
		
		for (int i = 0; i < Enemy_TotalRows; i++)
		{
			for (int j = 0; j < Enemy_TotalColumns; j++)
			{
				if (Enemies[i][j])
				{
					delete Enemies[i][j];
					Enemies[i][j] = nullptr;
				}
			}
		}
	}
	
private:
	PlayerBase* Player = nullptr;
	Projectile* PlayerProjectile = nullptr;
	
	static const int Enemy_TotalRows = 3;
	static const int Enemy_TotalColumns = 8;
	
	EnemyBase* Enemies[Enemy_TotalRows][Enemy_TotalColumns] = {};
	int EnemyDirection = -1; // -1 Izquierda | +1 Drecha
	int EnemyTickCount = 0;
	int EnemyTicksToBeMoved = 15;
		
	void Initializate()
	{
		Player = new PlayerBase;
		PlayerProjectile = new Projectile;
		ShowStartScreen();
		ShowHUD();
		Player->Spawn(50, Screen::BordeInf - 1, LIGHTBLUE);
		SpawnEnemies();
	}
	
	void ShowStartScreen()
	{
		textcolor(BLUE);
		gotoxy(25,3);
		std::cout << "Trabajo Practico para UNL Introduccion Programacion 2025" << std::endl;
		gotoxy(40,5);
		std::cout << "Alumno: Andres Jorge Neville" << std::endl;
		gotoxy(45,7);
		std::cout << "SPACE INVADERS LITE" << std::endl;
		textcolor(GREEN);
		gotoxy(12 ,11);
		std::cout << "Reglas:" << std::endl;
		gotoxy(15 ,13);
		std::cout << "- Mover la nave: 'a' izquierda, 'd' derecha" << std::endl;
		gotoxy(15 ,14);
		std::cout << "- Presione la tecla espaciadora para disparar" << std::endl;
		gotoxy(15 ,15);
		std::cout << "- Evitar los disparos enemigos" << std::endl;
		gotoxy(15 ,16);
		std::cout << "- Sobrevive eliminando a todos los enemigos antes que aterricen" << std::endl;
		gotoxy(15 ,17);
		std::cout << "- Presiona la 'q' para salir" << std::endl;
		textcolor(RED);
		gotoxy(35 ,25);
		std::cout << "Presiona una Tecla para comenzar a Jugar" << std::endl;
		getch();
		textcolor(WHITE);
		clrscr();
	}
	
	void ShowEndScreen(bool IsVictory)
	{
		if (IsVictory)
		{
			clrscr();
			std::cout << "Ganaste";
			getch();
		}
		else
		{
			clrscr();
			std::cout << "Perdiste";
			getch();
		}
	}
	
	void ShowHUD()
	{
		// Dibujar Bordes
		
		textcolor(LIGHTGREEN);
		Screen::DrawBorders();
		
		// Dibujar Score inicial
		
		textcolor(WHITE);
		gotoxy(Screen::BordeIzq + 5, Screen::BordeSup);
		std::cout << "Score: " << Score;
		
		gotoxy(Screen::BordeDer - 15, Screen::BordeSup);
		std::cout << "Lives: " << Player->GetLives();
		
	}
	
	void UpdateScoreHud()
	{
		textcolor(WHITE);
		gotoxy(Screen::BordeIzq + 12, Screen::BordeSup);
		std::cout << "     ";
		gotoxy(Screen::BordeIzq + 12, Screen::BordeSup);
		std::cout << Score;
	}
	
	int GetScore()
	{
		return Score;
	}
	
	void UpdateScore(int DeltaScore)
	{
		Score += DeltaScore;
	}
	
	void SpawnEnemies()
	{
		int InitialPosX = 15;
		int InitialPosY = 3;
		
		int PosToDrawX = InitialPosX;
		int PosToDrawY = InitialPosY;
		
		for (int i = 0; i < Enemy_TotalRows; i++)
		{
			for (int j = 0; j < Enemy_TotalColumns; j++)
			{
				if (i == 0)
				{
					Enemies[i][j] = new EnemyC();
				}
				else if (i == 1)
				{
					Enemies[i][j] = new EnemyB();
				}
				else
				{
					Enemies[i][j] = new EnemyA();
				}
				Enemies[i][j]->Spawn(PosToDrawX, PosToDrawY, Enemies[i][j]->GetColor());
				PosToDrawX += 10;
			}
			PosToDrawX = InitialPosX;
			PosToDrawY += 2;
		}
	}
	
	void MoveEnemies()
	{
		// Tengo que buscar la manera de saber la posicion del bloque en X
		// para poder hacerlo rebotar, tambien tengo que calcular Y para saber
		// si llegaron al piso. Voy a intentar buscar eso en base a unos maximos
		// y minimos opuestos para que se ajusten en la primera busqueda
		
		int MinimunX = 1000;
		int MaximunX = -1000;
		int MaximunY = -1000;
		bool bAreEnemiesAlive = false;
		int AliveEnemyCounter = 0;
		
		for (int i = 0; i < Enemy_TotalRows; ++i)
		{
			for (int j  = 0; j < Enemy_TotalColumns; ++j)
			{
				EnemyBase* Enemy = Enemies[i][j];
				if (!Enemy || !Enemy->GetIsAlive())
				{
					continue;
				}
				
				bAreEnemiesAlive = true;
				AliveEnemyCounter++;
				
				if (Enemy->GetX() < MinimunX)
				{
					MinimunX = Enemy->GetX();
				}
				if (Enemy->GetX() > MaximunX)
				{
					MaximunX = Enemy->GetX();
				}
				if (Enemy->GetY() > MaximunY)
				{
					MaximunY = Enemy->GetY();
				}
			}
		}
		
		//Debug para despues contar enemigos, armar bloques para uqe la velocidad aumente
		//std::cout << AliveEnemyCounter;
		
		if (!bAreEnemiesAlive)
		{
			bVictory = true;
			bIsRunning = false;
			return;
		}
		
		bool bIsMaximunLeft = (EnemyDirection == -1) && (MinimunX -1 <= Screen::BordeIzq + 1);
		bool bIsMaximunRight = (EnemyDirection == 1) && (MaximunX + 1 >= Screen::BordeDer -1);
		
		if (bIsMaximunLeft || bIsMaximunRight)
		{
			for (int i = 0; i < Enemy_TotalRows; ++i)
			{
				for (int j  = 0; j < Enemy_TotalColumns; ++j)
				{
					EnemyBase* Enemy = Enemies[i][j];
					if (!Enemy || !Enemy->GetIsAlive())
					{
						continue;
					}
					
					Screen::Erase(Enemy->GetX(), Enemy->GetY());
					Enemy->NewPos(Enemy->GetX(), Enemy->GetY() + 1);
					Screen::Draw(Enemy->GetX(), Enemy->GetY(), Enemy->GetColor(), Enemy->GetShape());
				}
			}
			
			EnemyDirection = -EnemyDirection;
			
			if (MaximunY + 1 >= Player->GetY())
			{
				bIsRunning = false;
				return;
			}
			
			//Evitar que bajen y se muevan horizonzalmente en el mismo ciclo.
			return; 
		}
		
		for (int i = 0; i < Enemy_TotalRows; ++i)
		{
			for (int j  = 0; j < Enemy_TotalColumns; ++j)
			{
				EnemyBase* Enemy = Enemies[i][j];
				if (!Enemy || !Enemy->GetIsAlive())
				{
					continue;
				}
				
				Screen::Erase(Enemy->GetX(), Enemy->GetY());
				Enemy->NewPos(Enemy->GetX() + EnemyDirection, Enemy->GetY());
				Screen::Draw(Enemy->GetX(), Enemy->GetY(), Enemy->GetColor(), Enemy->GetShape());
			}
		}
	}
	
	void HandleProjectileCollision(Projectile* InProjectile)
	{
		if (!InProjectile || !InProjectile->GetIsAlive())
		{
			return;
		}
		
		int ProjectileX = InProjectile->GetX();
		int ProjectileY = InProjectile->GetY();
		
		if (InProjectile->GetOwner() == Owner::Player)
		{
			for (int i = 0; i < Enemy_TotalRows; ++i)
			{
				for (int j  = 0; j < Enemy_TotalColumns; ++j)
				{
					EnemyBase* Enemy = Enemies[i][j];
					if (!Enemy || !Enemy->GetIsAlive())
					{
						continue;
					}
					if (Enemy->GetX() == ProjectileX && Enemy->GetY() == ProjectileY)
					{
						InProjectile->Kill();
						Screen::Erase(ProjectileX, ProjectileY);
						
						Enemy->OnHit();
						
						if (!Enemy->GetIsAlive())
						{
							Screen::Erase(Enemy->GetX(), Enemy->GetY());
							UpdateScore(Enemy->PointsToGrant); //Tener en cuenta que aca estoy dando puntos solo al matar al enemigo. - por ahora esta bien.
							UpdateScoreHud();
						}
						else
						{
							Screen::Draw(Enemy->GetX(), Enemy->GetY(), Enemy->GetColor(), Enemy->GetShape());
						}
						
						return;
					}
				}
			}
		}
		else if (InProjectile->GetOwner() == Owner::Enemy)
		{
			if (Player && Player->GetIsAlive())
			{
				if (Player->GetX() == ProjectileX && Player->GetY() == ProjectileY)
				{
					InProjectile->Kill();
					Screen::Erase(ProjectileX, ProjectileY);
					
					Player->LoseOneLife();
					
					if (!Player->GetIsAlive())
					{
						bIsRunning = false;
					}
					else
					{
						Screen::Draw(Player->GetX(), Player->GetY(), Player->GetColor(), Player->GetShape());
					}
				}
			}
		}
	}

};

int main() 
{
	Game G;
	G.Play();
	
	return 0;
}

/** Funciones de ayuda **/

