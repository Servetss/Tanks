#include "stdafx.h"
#include <SFML\Graphics.hpp>
#include <time.h>
#include <sstream>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <windows.h>

using namespace sf;

struct point
{ int x,y; };

void Pole(int field[15][15], int a, int b, int xx[200], int yy[200])
{
	std::cout << "----------------------------------------------------------------------------" << std::endl;
	for (int i = 0; i <= 15; i++)
	{
		std::cout << "| ";
		for (int q = 0; q < 15; q++)
		{
			for (int l = 0; l <= 50; l++)
			{
				if (q == xx[l] && i == yy[l])
				{
					std::cout << "-- | ";
					break;
				}
				else if (l == 50)
				{
					if (field[q][i] >= 0 && field[q][i] < 10)
						std::cout << field[q][i] << "  | ";
					else
						std::cout << field[q][i] << " | ";
				}
			}
			
		}
		std::cout << std::endl;
	}
	std::cout << "----------------------------------------------------------------------------" << std::endl;
	std::cout << a << " " << b;
}


int main()
{
	srand(time(0));
	RenderWindow app(VideoMode(600, 600), "Tanks!");
	app.setFramerateLimit(60);

	Texture t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11;
	
	t1.loadFromFile("C:/Users/Servets/Videos/source/repos/Tank/Images/background.png");
	t2.loadFromFile("C:/Users/Servets/Videos/source/repos/Tank/Images/tankUp.png");
	t3.loadFromFile("C:/Users/Servets/Videos/source/repos/Tank/Images/tankDown.png");
	t4.loadFromFile("C:/Users/Servets/Videos/source/repos/Tank/Images/tankLeft.png");
	t5.loadFromFile("C:/Users/Servets/Videos/source/repos/Tank/Images/tankRight.png");
	t6.loadFromFile("C:/Users/Servets/Videos/source/repos/Tank/Images/wall.png");
	t7.loadFromFile("C:/Users/Servets/Videos/source/repos/Tank/Images/bullet.png");
	t8.loadFromFile("C:/Users/Servets/Videos/source/repos/Tank/Images/enemyUp.png");
	t9.loadFromFile("C:/Users/Servets/Videos/source/repos/Tank/Images/enemyDown.png");
	t10.loadFromFile("C:/Users/Servets/Videos/source/repos/Tank/Images/enemyLeft.png");
	t11.loadFromFile("C:/Users/Servets/Videos/source/repos/Tank/Images/enemyRight.png");


	Sprite sBackGround(t1), sTankUp(t2), sTankDown(t3), sTankLeft(t4), sTankRight(t5), sWall(t6), sBullet(t7), sEnemyUp(t8), sEnemyDown(t9), sEnemyLeft(t10), sEnemyRight(t11);

	
	point wall[40];
	point bulletPoint[10];
	bool fire = false;
	bool start = true;
	bool Dojechal = false;
	bool BlockLeft = true, BlockRight = true, BlockUp = true, BlockDown = true; // проверка есть ли на горизоте танк или стена
	int xS, yS;
	int xF, yF;
	int x = 33, y = 33;
	int dx[40], dy[40];
	int delay = 50;
	short click = 0;
	short fireClick[5] = { 0,0,0,0,0 };
	int WallHealth[40];
	int field[15][15];
	int Proszlo = 0;
	int Finish;
	int DorogaEnemy = 0, Czekaj = 0, EnemyMove = 0, EnemyCoordinatesX = 0, EnemyCoordinatesY = 0;


	int targetS, targetF;


	for (int i = 0; i < 5; i++)
	{
		bulletPoint[i].x = -10;
		bulletPoint[i].y = -10;
	}


	for (int i = 0; i < 40; i++)
	{
		switch (i)
		{
		case 0:
			dx[0] = 7;
			dy[0] = 15;
			break;
		case 1:
			dx[1] = 9;
			dy[1] = 15;
			break;
		case 2:
			dx[2] = 7;
			dy[2] = 14;
			break;
		case 3:
			dx[3] = 8;
			dy[3] = 14;
			break;
		case 4:
			dx[4] = 9;
			dy[4] = 14;
			break;
		default:
			do
			{
				dx[i] = rand() % 15;
				dy[i] = rand() % 15;
			} while ((dx[i] == 1 && dy[i] == 1) || (dx[i] == 6 && dy[i] == 15));
		break;
		}

		while (Proszlo == 0)
		{
			for (int q = 0; q < 40; q++)
			{
					if ((dx[i] != dx[q]) && (dy[i] != dy[q]) && (i > q))
					{
						Proszlo = 0;
					}
					else
					{
						Proszlo = 1;
						break;
					}
			}
		}
		Proszlo = 0;
	}

	for (int i = 0; i < 15; i++)
		for (int q = 0; q < 15; q++)
			field[i][q] = 0;

	for (int i = 0; i < 40; i++)
	{
		field[dx[i]][dy[i]] = -1;
		wall[i].x = dx[i] * 33;
		wall[i].y = dy[i] * 33;
	}


	for (int i = 0; i < 40; i++)
		WallHealth[i] = 3;

	for (int i = 0; i <= 15; i++)
		field[i][15] = -4;


	while (app.isOpen())
	{
		Event e;

		while (app.pollEvent(e))
		{
			if (e.type == Event::Closed)
				app.close();
		}

		app.draw(sBackGround);
		
		if (Keyboard::isKeyPressed(Keyboard::Up)) { y -= 3; click = 0; }
			else if (Keyboard::isKeyPressed(Keyboard::Down)) { y += 3; click = 1; }
			else if (Keyboard::isKeyPressed(Keyboard::Left)) { x -= 3; click = 2; }
			else if (Keyboard::isKeyPressed(Keyboard::Right)) { x += 3; click = 3; }
			if (Keyboard::isKeyPressed(Keyboard::Space) && delay >= 20)
			{ 
				for (int i = 0; i < 5; i++)
				{
					if (bulletPoint[i].x < 0)
					{
						fire = true;
						fireClick[i] = click;

						switch (click)
						{
						case 0:
							bulletPoint[i].x = x + 11; bulletPoint[i].y = y - 5;
							break;
						case 1:
							bulletPoint[i].x = x + 11; bulletPoint[i].y = y + 32;
							break;
						case 2:
							bulletPoint[i].x = x - 5; bulletPoint[i].y = y + 11;
							break;
						case 3:
							bulletPoint[i].x = x + 32; bulletPoint[i].y = y + 11;
							break;
						default:
							break;
						}

						delay = 0;
						break;
					}
				}
			}

			// Размер танка 30px
			if (x < 0) x += 3;
			else if (x > 570) x -= 3;
			else if (y < 0) y += 3;
			else if (y > 570) y -= 3;

			for (int i = 0; i < 40; i++)
			{
				if ((x + 33 > wall[i].x) && (x < wall[i].x + 33) && (y + 33 > wall[i].y) && (y < wall[i].y + 33))
				{
					switch (click)
					{
					case 0:
						y += 3;
						break;
					case 1:
						y -= 3;
						break;
					case 2:
						x += 3;
						break;
					case 3:
						x -= 3;
						break;
					default:
						break;
					}
				}
			}


			for (int i = 0; i < 5; i++)
			{
				if (fire && (bulletPoint[i].x >= 0))
				{
					switch (fireClick[i])
					{
					case 0:
						bulletPoint[i].y -= 5;
						break;
					case 1:
						bulletPoint[i].y += 5;
						break;
					case 2:
						bulletPoint[i].x -= 5;
						break;
					case 3:
						bulletPoint[i].x += 5;
						break;
					default:
						break;
					}
				}
			}

			if(fire && delay <= 50) delay++;

			for (int i = 0; i < 5; i++)
			{
				if (bulletPoint[i].x >= 0 && bulletPoint[i].y >= 0 && bulletPoint[i].x <= 600 && bulletPoint[i].y <= 600)
				{

					for (int q = 0; q < 40; q++)
					{
						if ((bulletPoint[i].x + 4 > wall[q].x) && (bulletPoint[i].x < wall[q].x + 30) && (bulletPoint[i].y + 4 > wall[q].y) && (bulletPoint[i].y < wall[q].y + 30))
						{
							bulletPoint[i].x = -10;
							bulletPoint[i].y = -10;
							WallHealth[q]--;
						}
					}
					sBullet.setPosition(bulletPoint[i].x, bulletPoint[i].y);
					app.draw(sBullet);
				}
				else
				{
					bulletPoint[i].x = -10;
					bulletPoint[i].y = -10;
				}
			}



			for (int i = 0; i < 40; i++)
			{
				if (WallHealth[i] > 0)
				{
					sWall.setPosition(wall[i].x, wall[i].y);
					app.draw(sWall);
				}
				else
				{
					field[wall[i].x / 33][wall[i].y / 33] = 0;
					wall[i].x = 700;
					wall[i].y = 700;
				}
			}


		switch (click)
		{
			case 0:
				sTankUp.setPosition(x, y);
				app.draw(sTankUp);
				break;
			case 1:
				sTankDown.setPosition(x, y);
				app.draw(sTankDown);
				break;
			case 2:
				sTankLeft.setPosition(x, y);
				app.draw(sTankLeft);
				break;
			case 3:
				sTankRight.setPosition(x, y);
				app.draw(sTankRight);
				break;
			default:
				break;
		}



		int xx[200];
		int yy[200];
		int zz[200];


		if (start || Keyboard::isKeyPressed(Keyboard::A) || Dojechal)
		{
			Dojechal = false;
			if (!start)
			{
				for (int i = 0; i <= 15; i++)
					for (int q = 0; q <= 15; q++)
						if (field[i][q] != 0 && field[i][q] != -1 && field[i][q] != -4 )
							field[i][q] = 0;


				if (xS == 0)
					xS = xF;
				if (yS == 0)
					yS = yF;
			}
			else
			{
				xS = 6;
				yS = 14;


				if (xS == 0)
					xS = 2;
				if (yS == 0)
					yS = 2;
			}


			int droga = 2;
			int poz = -1;

			bool nie = true;

			for (int i = 0; i < 200; i++)
			{
				xx[i] = -1;
				yy[i] = -1;
				zz[i] = -1;
			}


			while (nie)
			{
				if (field[x / 33][y / 33] == 0)
				{
					field[x/ 33][y / 33] = -3;
					xF = x;
					yF = y;
					nie = false;
					break;
				}
			}


			system("cls");
			if ((field[xS - 1][yS] == 0 || field[xS - 1][yS] == -3) && (xS - 1 >= 0 && xS - 1 <= 495 && yS >= 0 && yS <= 495))
			{
				poz++;
				field[xS][yS] = 2;
				xx[poz] = xS; yy[poz] = yS; zz[poz] = 2;
			}
			else if ((field[xS + 1][yS] == 0 || field[xS + 1][yS] == -3) && (xS + 1 >= 0 && xS + 1 <= 495 && yS >= 0 && yS <= 495))
			{
				poz++;
				field[xS][yS] = 2;
				xx[poz] = xS; yy[poz] = yS; zz[poz] = 2;
			}
			else if ((field[xS][yS - 1] == 0 || field[xS][yS - 1] == -3) && (xS >= 0 && xS <= 495 && yS - 1 >= 0 && yS - 1 <= 495))
			{
				poz++;
				field[xS][yS] = 2;
				xx[poz] = xS; yy[poz] = yS; zz[poz] = 2;
			}
			else if ((field[xS][yS + 1] == 0 || field[xS][yS + 1] == -3) && (xS >= 0 && xS <= 495 && yS + 1 >= 0 && yS + 1 <= 495))
			{
				poz++;
				field[xS][yS] = 2;
				xx[poz] = xS; yy[poz] = yS; zz[poz] = 2;
			}

			std::cout << xx[0] << std::endl;

				while (field[xF / 33][yF / 33] == -3)
				{
					for (int i = 0; i < 200; i++)
					{
						if (xx[i] != -1 && yy[i] != -1)
							poz = i + 1;
					}

					if ((field[xx[0] - 1][yy[0]] == 0 || field[xx[0] - 1][yy[0]] == -3) && ((xx[0] - 1 >= 0 && xx[0] - 1 <= 495) && (yy[0] >= 0 && yy[0] <= 495)))
					{
						zz[poz] = zz[0] + 1;
						field[xx[0] - 1][yy[0]] = zz[poz];
						xx[poz] = xx[0] - 1; yy[poz] = yy[0];
						poz++;
					}

					if ((field[xx[0] + 1][yy[0]] == 0 || field[xx[0] + 1][yy[0]] == -3) && ((xx[0] + 1 >= 0 && xx[0] + 1 <= 495) && (yy[0] >= 0 && yy[0] <= 495)))
					{
						zz[poz] = zz[0] + 1;
						field[xx[0] + 1][yy[0]] = zz[poz];
						xx[poz] = xx[0] + 1; yy[poz] = yy[0];
						poz++;
					}

					if ((field[xx[0]][yy[0] - 1] == 0 || field[xx[0]][yy[0] - 1] == -3) && ((xx[0] >= 0 && xx[0] <= 495) && (yy[0] - 1 >= 0 && yy[0] - 1 <= 495)))
					{
						zz[poz] = zz[0] + 1;
						field[xx[0]][yy[0] - 1] = zz[poz];
						xx[poz] = xx[0]; yy[poz] = yy[0] - 1;
						poz++;
					}

					if ((field[xx[0]][yy[0] + 1] == 0 || field[xx[0]][yy[0] + 1] == -3) && ((xx[0] >= 0 && xx[0] <= 495) && (yy[0] + 1 >= 0 && yy[0] + 1 <= 495)))
					{
						zz[poz] = zz[0] + 1;
						field[xx[0]][yy[0] + 1] = zz[poz];
						xx[poz] = xx[0]; yy[poz] = yy[0] + 1;
						poz++;
					}

					xx[0] = -1; yy[0] = -1; zz[0] = -1;

					for (int i = 1; i < 200; i++)
					{
						if (xx[i] != -1 && yy[i] != -1)
						{
							xx[i - 1] = xx[i];
							yy[i - 1] = yy[i];
							zz[i - 1] = zz[i];
						}
						else break;

					}
				}
	

			int put[20];
			Finish = field[xF / 33][yF / 33];

			for (int i = 0; i < 200; i++)
			{
				xx[i] = -1;
				yy[i] = -1;
				zz[i] = -1;
			}

			xx[0] = xF / 33; yy[0] = yF / 33; zz[0] = field[xF / 33][yF / 33];

			

			for (int i = 0; i < 20; i++)
			{
				if (field[xx[i] - 1][yy[i]] == Finish - 1)
				{
					Finish--;
					xx[i + 1] = xx[i] - 1; yy[i + 1] = yy[i]; zz[i + 1] = Finish;
				}
				if (field[xx[i] + 1][yy[i]] == Finish - 1)
				{
					Finish--;
					xx[i + 1] = xx[i] + 1; yy[i + 1] = yy[i]; zz[i + 1] = Finish;
				}
				if (field[xx[i]][yy[i] - 1] == Finish - 1)
				{
					Finish--;
					xx[i + 1] = xx[i]; yy[i + 1] = yy[i] - 1; zz[i + 1] = Finish;
				}
				if (field[xx[i]][yy[i] + 1] == Finish - 1)
				{
					Finish--;
					xx[i + 1] = xx[i]; yy[i + 1] = yy[i] + 1; zz[i + 1] = Finish;
				}
			}


			
			for (int i = 0; i < 25; i++)
				std::cout << xx[i] << " ";

			std::cout << std::endl;

			for(int i = 0; i< 25; i++)
				std::cout << yy[i] << " ";

			std::cout << std::endl;

			for (int i = 0; i< 25; i++)
				std::cout << zz[i] << " ";

			std::cout << std::endl;

			Pole(field, xS, yS, xx, yy);
			std::cout << std::endl;
			std::cout << xF << " " << yF << std::endl;
			std::cout << poz << std::endl;

			start = false;
			DorogaEnemy = zz[0] - 2;
			EnemyCoordinatesX = xx[DorogaEnemy] * 33;
			EnemyCoordinatesY = yy[DorogaEnemy] * 33;

		}
		

		if (DorogaEnemy >= 0)
		{
			if (xx[DorogaEnemy] == xx[DorogaEnemy - 1])
			{
				if (yy[DorogaEnemy] > yy[DorogaEnemy - 1])//Вниз
					EnemyMove = 1;
				else if (yy[DorogaEnemy] < yy[DorogaEnemy - 1])//Вверх
					EnemyMove = 2;
			}
			else if (yy[DorogaEnemy] == yy[DorogaEnemy - 1])
			{
				if (xx[DorogaEnemy] > xx[DorogaEnemy - 1])//Вправо
					EnemyMove = 3;
				else if (xx[DorogaEnemy] < xx[DorogaEnemy - 1])//Влево
					EnemyMove = 4;
			}

			switch (EnemyMove)
			{
			case 1:
				EnemyCoordinatesY -= 3;
				break;
			case 2:
				EnemyCoordinatesY += 3;
				break;
			case 3:
				EnemyCoordinatesX -= 3;
				break;
			case 4:
				EnemyCoordinatesX += 3;
				break;
			default:
				break;
			}


			if ((EnemyCoordinatesX == xx[DorogaEnemy - 1] * 33) && (EnemyMove == 3 || EnemyMove == 4))
			{
				BlockDown = true; BlockUp = true; BlockLeft = true; BlockRight = true;
				for (int i = 0; i < 15; i++)//проверка есть ли на горизонте главный герой
				{

					if ((field[DorogaEnemy + i][DorogaEnemy] != -1 && BlockRight) || DorogaEnemy + i < 14) //Проверка на горизонте стена. Если нет, то на горизонте танк.
					{
						if (xx[DorogaEnemy] + i == x / 33 && yy[DorogaEnemy] == y / 33)
						{
							std::cout << "PUTX!!!!!!!!!!!!!"; // должен стрелять, если вражеский танк впереди, а если ушел, то двигайся дальше
						}
					}
					else { BlockRight = false; std::cout << "" << std::endl;  }

					if ((field[DorogaEnemy - i][DorogaEnemy] != -1 && BlockLeft) || DorogaEnemy - i > -1) //Проверка на горизонте стена. Если нет, то на горизонте танк.
					{
						if (xx[DorogaEnemy] - i == x / 33 && yy[DorogaEnemy] == y / 33)
						{
							std::cout << "PUTX!!!!!!!!!!!!!!";
						}
					}
					else { BlockLeft = false; std::cout << "" << std::endl;}

					if ((field[DorogaEnemy][DorogaEnemy + i] != -1 && BlockDown) || DorogaEnemy + i < 14) //Проверка на горизонте стена. Если нет, то на горизонте танк.
					{
						if (xx[DorogaEnemy] == x / 33 && yy[DorogaEnemy] + i == y / 33)
						{
							std::cout << "PUTX!!!!!!!!!!!!!!!!"; 
						}
					}
					else { BlockDown = false; std::cout << "" << std::endl; }

					if ((field[DorogaEnemy][DorogaEnemy - i] != -1 && BlockUp) || DorogaEnemy - i > -1) //Проверка на горизонте стена. Если нет, то на горизонте танк.
					{
						if (xx[DorogaEnemy] == x / 33 && yy[DorogaEnemy] - i == y / 33)
						{
							std::cout << "PUTX!!!!!!!!!!!!!!!!!!";
						}
					}
					else { BlockUp = false; std::cout << "" << std::endl;  }
				}
				DorogaEnemy--;
				std::cout << "xx: " << xx[DorogaEnemy] << std::endl;
				std::cout << "yy: " << yy[DorogaEnemy] << std::endl;
				xS = xx[DorogaEnemy];
				yS = yy[DorogaEnemy];
			}
			else if ((EnemyCoordinatesY == yy[DorogaEnemy - 1] * 33) && (EnemyMove == 1 || EnemyMove == 2))
			{
				BlockDown = true; BlockUp = true; BlockLeft = true; BlockRight = true;
				for (int i = 0; i < 15; i++)//проверка есть ли на горизонте главный герой
				{

					if ((field[DorogaEnemy + i][DorogaEnemy] != -1 && BlockRight) || DorogaEnemy + i < 14) //Проверка на горизонте стена. Если нет, то на горизонте танк.
					{
						if (xx[DorogaEnemy] + i == x / 33 && yy[DorogaEnemy] == y / 33)
						{
							std::cout << "PUTX!!!!!!!!!!!"; 
						}
					}
					else { BlockRight = false; std::cout << "" << std::endl; }

					if ((field[DorogaEnemy - i][DorogaEnemy] != -1 && BlockLeft) || DorogaEnemy - i > -1) //Проверка на горизонте стена. Если нет, то на горизонте танк.
					{
						if (xx[DorogaEnemy] - i == x / 33 && yy[DorogaEnemy] == y / 33)
						{
							std::cout << "PUTX!!!!!!!!!!!!!!!"; 
						}
					}
					else { BlockLeft = false; std::cout << "" << std::endl; }

					if ((field[DorogaEnemy][DorogaEnemy + i] != -1 && BlockDown) || DorogaEnemy + i < 14) //Проверка на горизонте стена. Если нет, то на горизонте танк.
					{
						if (xx[DorogaEnemy] == x / 33 && yy[DorogaEnemy] + i == y / 33)
						{
							std::cout << "PUTX!!!!!!!!!!!!!!!!!!!"; 
						}
					}
					else { BlockDown = false; std::cout << "" << std::endl; }

					if ((field[DorogaEnemy][DorogaEnemy - i] != -1 && BlockUp) || DorogaEnemy - i > -1) //Проверка на горизонте стена. Если нет, то на горизонте танк.
					{
						if (xx[DorogaEnemy] == x / 33 && yy[DorogaEnemy] - i == y / 33)
						{
							std::cout << "PUTX!!!!!!!!!!!!!!!!!!!!!"; 
						}
					}
					else { BlockUp = false; std::cout << "" << std::endl; }
				}
				DorogaEnemy--;
				std::cout << "xx: " << xx[DorogaEnemy] << std::endl;
				std::cout << "yy: " << yy[DorogaEnemy] << std::endl;
				xS = xx[DorogaEnemy];
				yS = yy[DorogaEnemy];
			}
			if (xx[DorogaEnemy] == xF / 33 && yy[DorogaEnemy] == yF / 33)
				Dojechal = true;

				if (DorogaEnemy == 0)
					DorogaEnemy = -1;
				

		}

		switch (EnemyMove)
		{
		case 1:
			sEnemyUp.setPosition(EnemyCoordinatesX, EnemyCoordinatesY);
			app.draw(sEnemyUp);			
			break;
		case 2:
			sEnemyDown.setPosition(EnemyCoordinatesX, EnemyCoordinatesY);
			app.draw(sEnemyDown);
			break;
		case 3:
			sEnemyLeft.setPosition(EnemyCoordinatesX, EnemyCoordinatesY);
			app.draw(sEnemyLeft);
			break;
		case 4:
			sEnemyRight.setPosition(EnemyCoordinatesX, EnemyCoordinatesY);
			app.draw(sEnemyRight);
			break;
		default:
			break;
		}

		app.display();
	}

    return 0;
}

