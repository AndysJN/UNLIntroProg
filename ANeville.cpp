#include <iostream>
#include <conio2.h>
#include <windows.h>
#include <ctime>

class Projectile;

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
	
	virtual void Shoot(Projectile* InProjectile)
	{
		
	}
	
	virtual void OnHit()
	{
		
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
		
		if (Y <= Screen::BordeSup + 1 || Y > Screen::BordeInf -1)
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
	
	virtual void Shoot(Projectile* InProjectile) override
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
	
	virtual void OnHit() override
	{
		LoseOneLife();
		
		if (!GetIsAlive())
		{
			return;
		}
		
		for (int i = 0; i < 3; ++i)
		{
			Screen::Erase(GetX(), GetY());
			Sleep(100);
			Screen::Draw(GetX(), GetY(), GetColor(), GetShape());
			Sleep(80);
		}
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
protected:
	int HitPoints{1};
	int PointsToGrant{0};
	Projectile* MyProjectile;
	
public:
	
	EnemyBase ()
	{
		Shape = 'E' ;
	}
	
	virtual void OnHit() override
	{
		HitPoints--;
		if (HitPoints <= 0)
		{
			bIsAlive = false;
		}
	}
	
	virtual void Shoot(Projectile* InProjectile) override
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
		MyProjectile->Initialize(X, Y + 1, Owner::Enemy);
	}
	
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
		HitPoints = 2;
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
		HitPoints = 3;
		PointsToGrant = 500;
		Shape = 'W';
		Color = LIGHTMAGENTA;
	}
};

/**
*	Clase para manejar la logica del loop juego 
*	Y sus reglas
**/

class Game
{
	
public:
	//Normalizar la velocidad de movimiento
	
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
		
		Tempo = clock();
		
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
			
			for (int i = 0; i < MaxEnemyProjectiles; ++i)
			{
				if (EnemyProjectiles[i] && EnemyProjectiles[i]->GetIsAlive())
				{
					EnemyProjectiles[i]->Movement();
					HandleProjectileCollision(EnemyProjectiles[i]);
				}
			}
			
			EnemyTickCount++;
			if (EnemyTickCount >= EnemyTicksToBeMoved)
			{
			MoveEnemies();
			EnemyTickCount = 0;
			}
			
			EnemyProjectileFireTickCount++;
			if (EnemyProjectileFireTickCount >= EnemyProjectileTicksToBeFired)
			{
				EnemyShooting();
				EnemyProjectileFireTickCount = 0;
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
		
		for (int i = 0; i < MaxEnemyProjectiles; ++i)
		{
			delete EnemyProjectiles[i];
			EnemyProjectiles[i] = nullptr;
		}
	}
	
private:
		
	/** Referente al Jugador **/
		
	PlayerBase* Player = nullptr;
	
	/** Referente al proyectil del Jugador **/
	
	Projectile* PlayerProjectile = nullptr;
	
	/** Referente a los enemigos **/
	
	static const int Enemy_TotalRows = 3;
	static const int Enemy_TotalColumns = 8;
	
	EnemyBase* Enemies[Enemy_TotalRows][Enemy_TotalColumns] = {};
	int EnemyDirection = -1; // -1 Izquierda | +1 Drecha
	int EnemyTickCount = 0;
	int EnemyTicksToBeMoved = 15;
	
	/** Referente a los proyectiles de los enemigos **/
	
	static const int MaxEnemyProjectiles = 3;
	Projectile* EnemyProjectiles[MaxEnemyProjectiles] = {nullptr, nullptr, nullptr};
	int EnemyProjectileFireTickCount = 0;
	int EnemyProjectileTicksToBeFired = 10; //Esto tambien lo puedo incrementar dependiendo la cantidad de enemigos en pantalla.
		
	void Initializate()
	{
		Player = new PlayerBase;
		PlayerProjectile = new Projectile;
		for (int i = 0; i < MaxEnemyProjectiles; ++i)
		{
			EnemyProjectiles[i] = new Projectile;
		}
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
			textcolor(BLUE);
			gotoxy(43,10);
			std::cout << "Ganaste!!!" << std::endl;
			gotoxy(36,11);
			std::cout << "Tus Puntos finales son: " << GetScore();
			getch();
		}
		else
		{
			clrscr();
			textcolor(LIGHTGRAY);
			gotoxy(42,10);
			std::cout << "Perdiste";
			gotoxy(38,11);
			std::cout << "Sumaste " << GetScore() << " puntos.";
			getch();
		}
	}
	
	void ShowHUD()
	{
		
		textcolor(LIGHTGREEN);
		Screen::DrawBorders();
		
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
	
	void UpdateLivesHud()
	{
		textcolor(WHITE);
		gotoxy(Screen::BordeDer - 8, Screen::BordeSup);
		std::cout << "     ";
		gotoxy(Screen::BordeDer - 8, Screen::BordeSup);
		std::cout << Player->GetLives();
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
		
		if (AliveEnemyCounter < 3)
		{
			EnemyTicksToBeMoved = 4;
			EnemyProjectileTicksToBeFired = 7;
		}
		else if (AliveEnemyCounter < 5)
		{
			EnemyTicksToBeMoved = 8;
			EnemyProjectileTicksToBeFired = 8;
		}
		else if (AliveEnemyCounter < 8)
		{
			EnemyTicksToBeMoved = 10;
			EnemyProjectileTicksToBeFired = 9;
		}
		else if (AliveEnemyCounter < 10)
		{
			EnemyTicksToBeMoved = 11;
		}
		else if (AliveEnemyCounter < 16)
		{
			EnemyTicksToBeMoved = 12;
		}
		else if (AliveEnemyCounter < 20)
		{
			EnemyTicksToBeMoved = 13;
		}
		

		
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
		
		switch (InProjectile->GetOwner())
		{
			case Owner::Player:
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
							UpdateScore(Enemy->GetPointsToGrant());
							UpdateScoreHud();
							
							if (!Enemy->GetIsAlive())
							{
								Screen::Erase(Enemy->GetX(), Enemy->GetY());
							}
							else
							{
								Screen::Draw(Enemy->GetX(), Enemy->GetY(), Enemy->GetColor(), Enemy->GetShape());
							}
							
							return;
						}
					}
				}
				break;
			
			case Owner::Enemy:
				if (Player && Player->GetIsAlive())
				{
					if (Player->GetX() == ProjectileX && Player->GetY() == ProjectileY)
					{
						InProjectile->Kill();
						Screen::Erase(ProjectileX, ProjectileY);
						
						Player->OnHit();
						UpdateLivesHud();
						
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
				break;
				
		default:
			break;
		}
	}
	
	EnemyBase* GetBottomAliveInEachColumn(int InColumn)
	{
		for (int i = Enemy_TotalRows - 1; i >= 0; --i)
		{
			EnemyBase* Enemy = Enemies[i][InColumn];
			if (Enemy && Enemy->GetIsAlive())
			{
				return Enemy;
			}
		}
		return nullptr;
	}
	
	Projectile* GetEnemyProjectileIfAvailable()
	{
		for (int i = 0; i < MaxEnemyProjectiles; ++i)
		{
			if (EnemyProjectiles[i] && !EnemyProjectiles[i]->GetIsAlive())
			{
				return EnemyProjectiles[i];
			}
		}
		return nullptr;
	}
	
	void EnemyShooting()
	{
		Projectile* ProjectileSlot = GetEnemyProjectileIfAvailable();
		
		if (!ProjectileSlot)
		{
			return;
		}
		
		if ((rand() % 100) < 45)
		{
			return;
		}
		
		int StartingColumn = rand() % Enemy_TotalColumns;
		for (int i = 0; i < Enemy_TotalColumns; ++i)
		{
			int ShootingColumn = (StartingColumn + i) % Enemy_TotalColumns;
			EnemyBase* EnemyShooter = GetBottomAliveInEachColumn(ShootingColumn);
			if (EnemyShooter)
			{
				EnemyShooter->Shoot(ProjectileSlot);
				break;
			}
		}
	}
};

int main() 
{
	srand(time(NULL));
	
	Game G;
	G.Play();
	
	return 0;
}
