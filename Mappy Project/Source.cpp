#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include "SpriteSheet.h"
#include "mappy_A5.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

int collided(int x, int y);  //Tile Collision
int countFoodTiles();
bool endValue( int x, int y ); //End Block with the User Value = 8
bool foodValue(int x, int y);
bool hazardValue(int x, int y);
int headCollided(int x, int y);
bool enemyPathClear(int x, int y, int moveDistance);
bool findRandomEnemySpot(float& enemyX, float& enemyY, int moveDistance);
void setupEnemiesForLevel(EnemyAnt enemies[], int& enemyCount, int currentLevel);
int main(void)
{
	const int WIDTH = 900;
	const int HEIGHT = 480;
	const double LEVEL_TIME = 60.0;
	bool keys[] = {false, false, false, false};
	enum KEYS{UP, DOWN, LEFT, RIGHT};
	//variables
	bool introScreen = true;
	bool gameStarted = false;
	bool gameOver = false;
	int lives = 3;
	int hitEffectTimer = 0;

	int foodCollected = 0;
	int totalFoodCollected = 0;
	int foodNeeded = 0;
	int ammo = 1;
	int enemiesDefeated = 0;
	int noAmmoMessageTimer = 0;
	int needFoodMessageTimer = 0;
	bool touchingFood = false;
	bool collectedFood[4][200][50] = { false };

	bool projectileActive = false;
	float projectileX = 0;
	float projectileY = 0;
	float projectileSpeed = 8;
	float playerDirection = 8;
	int projectileDirection = 3;

	bool done = false;
	bool render = false;
	bool levelComplete = false;
	bool gameWon = false;
	bool timeOver = false;
	int currentLevel = 1;
	const int TOTAL_LEVELS = 3;

	double totalStartTime = 0;
	double totalFinalTime = 0;
	double startTime = 0;
	double finalTime = 0;
	double levelStartTime = 0;
	double timeRemaining = LEVEL_TIME;
	//Player Variable
	Sprite player;

	// Enemy ant variables
	const int MAX_ENEMIES = 20;
	EnemyAnt enemies[MAX_ENEMIES];
	int enemyCount = 0;


	//allegro variable
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer;
	ALLEGRO_FONT* font = NULL;
	ALLEGRO_SAMPLE* music = NULL;
	ALLEGRO_SAMPLE* collectSound = NULL;
	ALLEGRO_SAMPLE* damageSound = NULL;
	ALLEGRO_SAMPLE* winSound = NULL;
	ALLEGRO_SAMPLE* killSound = NULL;
	ALLEGRO_SAMPLE* shootSound = NULL;

	//program init
	if(!al_init())										//initialize Allegro
		return -1;

	srand(time(NULL));

	display = al_create_display(WIDTH, HEIGHT);			//create our display object

	if(!display)										//test display object
		return -1;

	//addon init
	al_install_keyboard();
	al_init_image_addon();
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_install_audio();
	al_init_acodec_addon();
	al_reserve_samples(10);

	player.InitSprites(WIDTH,HEIGHT);

	

	int xOff = 0;
	int yOff = 0;
	if(MapLoad("map1.fmp", 1))
		return -5;
	foodNeeded = countFoodTiles();
	setupEnemiesForLevel(enemies, enemyCount, currentLevel);

	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / 60);

	font = al_load_ttf_font("gamefont.ttf", 20, 0);

	if (!font)
	{
		cout << "Could not load gamefont.ttf. Using built-in font." << endl;
		font = al_create_builtin_font();
	}

	music = al_load_sample("music.wav");
	collectSound = al_load_sample("collect.wav");
	damageSound = al_load_sample("damage.wav");
	winSound = al_load_sample("win.wav");
	killSound = al_load_sample("kill.wav");
	shootSound = al_load_sample("shoot.wav");

	if (music)
	{
		al_play_sample(music, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
	}

	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	al_start_timer(timer);
	startTime = al_get_time();
	levelStartTime = al_get_time();
	totalStartTime = al_get_time();
	//draw the background tiles
	MapDrawBG(xOff,yOff, 0, 0, WIDTH-1, HEIGHT-1);

	//draw foreground tiles
	MapDrawFG(xOff,yOff, 0, 0, WIDTH-1, HEIGHT-1, 0);
	player.DrawSprites(0,0);
	al_flip_display();
	al_clear_to_color(al_map_rgb(0,0,0));
	while(!done)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);
		// Draw and control the intro screen before normal gameplay starts.
		if (introScreen)
		{
			if (ev.type == ALLEGRO_EVENT_TIMER)
			{
				render = true;
			}

			if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
			{
				if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
				{
					introScreen = false;
					gameStarted = true;
					levelStartTime = al_get_time();
					totalStartTime = al_get_time();
				}
				else if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
				{
					done = true;
				}
			}

			if (render && al_is_event_queue_empty(event_queue))
			{
				render = false;

				al_clear_to_color(al_map_rgb(0, 0, 0));

				al_draw_text(
					font,
					al_map_rgb(255, 255, 255),
					WIDTH / 2,
					150,
					ALLEGRO_ALIGN_CENTER,
					"ANT ESCAPE"
				);

				al_draw_text(
					font,
					al_map_rgb(255, 255, 255),
					WIDTH / 2,
					200,
					ALLEGRO_ALIGN_CENTER,
					"Collect all food while avoiding red hazard areas before reaching the exit."
				);

				al_draw_text(
					font,
					al_map_rgb(255, 255, 255),
					WIDTH / 2,
					230,
					ALLEGRO_ALIGN_CENTER,
					"Food gives ammo. Press SPACE to shoot enemy ants."
				);

				al_draw_text(
					font,
					al_map_rgb(255, 255, 255),
					WIDTH / 2,
					260,
					ALLEGRO_ALIGN_CENTER,
					"Shoot or Avoid enemy ants or you lose a life."
				);

				al_draw_text(
					font,
					al_map_rgb(255, 255, 255),
					WIDTH / 2,
					320,
					ALLEGRO_ALIGN_CENTER,
					"Press ENTER to start."
				);

				al_flip_display();
			}

			continue;
		}
		// normal gameplay timer code starts here
		if(ev.type == ALLEGRO_EVENT_TIMER)
		{
			MapUpdateAnims();

			double elapsed = al_get_time() - levelStartTime;
			timeRemaining = LEVEL_TIME - elapsed;

			if (timeRemaining <= 0)
			{
				timeRemaining = 0;
				timeOver = true;
				done = true;
			}

			render = true;
			if (keys[UP])
			{
				player.UpdateSprites(0, -4, 0);
				playerDirection = 0;
			}
			else if (keys[DOWN])
			{
				player.UpdateSprites(0, 4, 1);
				playerDirection = 1;
			}
			else if (keys[LEFT])
			{
				player.UpdateSprites(-4, 0, 2);
				playerDirection = 2;
			}
			else if (keys[RIGHT])
			{
				player.UpdateSprites(4, 0, 3);
				playerDirection = 3;
			}
			else
			{
				player.StandStill();
			}
			for (int i = 0; i < enemyCount; i++)
			{
				enemies[i].Update();
			}

			for (int i = 0; i < enemyCount; i++)
			{
				if (enemies[i].CollidesWith(player))
				{
					if (hitEffectTimer == 0)
					{
						lives--;

						cout << "Enemy ant hit!" << endl;

						if (damageSound)
						{
							al_play_sample(damageSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
						}

						player.SetHitEffect(true);
						hitEffectTimer = 60;

						player.ResetPosition(80, 80);

						if (lives <= 0)
						{
							gameOver = true;
							done = true;
						}
					}
				}
			}

			if (projectileActive)
			{
				if (projectileDirection == 0) // up
				{
					projectileY -= projectileSpeed;
				}
				else if (projectileDirection == 1) // down
				{
					projectileY += projectileSpeed;
				}
				else if (projectileDirection == 2) // left
				{
					projectileX -= projectileSpeed;
				}
				else if (projectileDirection == 3) // right
				{
					projectileX += projectileSpeed;
				}

				for (int i = 0; i < enemyCount; i++)
				{
					if (enemies[i].IsActive())
					{
						if (projectileX > enemies[i].getX() &&
							projectileX < enemies[i].getX() + 32 &&
							projectileY > enemies[i].getY() &&
							projectileY < enemies[i].getY() + 32)
						{
							enemies[i].Deactivate();
							projectileActive = false;
							enemiesDefeated++;

							cout << "Enemy ant defeated!" << endl;

							if (killSound)
							{
								al_play_sample(killSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
							}
						}
					}
				}


				if (projectileX < 0 || projectileX > mapwidth * mapblockwidth ||
					projectileY < 0 || projectileY > mapheight * mapblockheight)
				{
					projectileActive = false;
				}
				// If the projectile hits a wall, remove it.
				if (projectileActive && collided(projectileX, projectileY))
				{
					projectileActive = false;
				}
			}

			// Check if the player touched a food tile.
			int foodTileX = (player.getX() + player.getWidth() / 2) / mapblockwidth;
			int foodTileY = (player.getY() + player.getHeight() / 2) / mapblockheight;

			if (foodValue(player.getX() + player.getWidth() / 2, player.getY() + player.getHeight() / 2))
			{
				// Only collect this specific food tile once.
				if (!collectedFood[currentLevel][foodTileX][foodTileY])
				{
					foodCollected++;
					totalFoodCollected++;
					ammo++;

					collectedFood[currentLevel][foodTileX][foodTileY] = true;

					// Change the food tile into a normal floor tile after it is collected.
					MapSetBlock(foodTileX, foodTileY, 419);

					cout << "Food collected!" << endl;

					if (collectSound)
					{
						al_play_sample(collectSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
					}
				}
			}
			

			// Check if the player touched a hazard tile.
			if (hazardValue(player.getX() + player.getWidth() / 2, player.getY() + player.getHeight() / 2))
			{
				if (hitEffectTimer == 0)
				{
					lives--;

					cout << "Hazard hit!" << endl;

					if (damageSound)
					{
						al_play_sample(damageSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
					}

					player.SetHitEffect(true);
					hitEffectTimer = 60; // about 1 second at 60 FPS

					player.ResetPosition(80, 80);

					if (lives <= 0)
					{
						gameOver = true;
						done = true;
					}
				}
			}
			// Count down the hit effect.
			if (hitEffectTimer > 0)
			{
				hitEffectTimer--;

				if (hitEffectTimer == 0)
				{
					player.SetHitEffect(false);
				}
			}

			// Count down the collect-all-food message.
			if (needFoodMessageTimer > 0)
			{
				needFoodMessageTimer--;
			}

			if (noAmmoMessageTimer > 0)
			{
				noAmmoMessageTimer--;
			}

			if (player.CollisionEndBlock())
			{
				if (foodCollected < foodNeeded)
				{
					cout << "Collect all food before leaving!" << endl;
					needFoodMessageTimer = 120;
				}
				else
				{
					cout << "Level complete!" << endl;

					currentLevel++;

					if (currentLevel > TOTAL_LEVELS)
					{
						gameWon = true;
						levelComplete = true;
						done = true;
						totalFinalTime = al_get_time() - totalStartTime;

						if (winSound)
						{
							al_play_sample(winSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
						}
					}
					else
					{
						MapFreeMem();

						char mapName[20];
						sprintf_s(mapName, "map%d.fmp", currentLevel);

						if (MapLoad(mapName, 1))
						{
							cout << "Could not load " << mapName << endl;
							return -6;
						}

						player.ResetPosition(80, 80);

						foodCollected = 0;
						foodNeeded = countFoodTiles();

						setupEnemiesForLevel(enemies, enemyCount, currentLevel);

						xOff = 0;
						yOff = 0;
						levelStartTime = al_get_time();
						timeRemaining = LEVEL_TIME;
					}
				}
			}
			render = true;
			
		}
		else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			done = true;
		}
		else if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch(ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = true;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = true;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = true;
				break;
			case ALLEGRO_KEY_SPACE:
				if (!projectileActive && ammo > 0)
				{
					projectileActive = true;
					ammo--;

					projectileX = player.getX() + player.getWidth() / 2;
					projectileY = player.getY() + player.getHeight() / 2;

					projectileDirection = playerDirection;

					if (shootSound)
					{
						al_play_sample(shootSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
					}
				}
				else if (ammo <= 0)
				{
					noAmmoMessageTimer = 90;
				}
				break;

			}
		}
		else if(ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch(ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = false;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = false;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = false;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = false;
				break;
			}
		}
		if (!done && render && al_is_event_queue_empty(event_queue))
		{
			render = false;

			//update the map scroll position
			xOff = player.getX()+player.getWidth() - WIDTH/2 ;
			yOff = player.getY()+player.getHeight()   - HEIGHT/2 ;

			//avoid moving beyond the map edge
			if (xOff < 0) xOff = 0;

			if (xOff > (mapwidth * mapblockwidth - WIDTH))
				xOff = mapwidth * mapblockwidth - WIDTH;
			if (yOff < 0) 
				yOff = 0;
			if (yOff > (mapheight * mapblockheight - HEIGHT)) 
				yOff = mapheight * mapblockheight - HEIGHT;

			//draw the background tiles
			MapDrawBG(xOff,yOff, 0, 0, WIDTH, HEIGHT);

			//draw foreground tiles
			MapDrawFG(xOff,yOff, 0, 0, WIDTH, HEIGHT, 0);
			for (int i = 0; i < enemyCount; i++)
			{
				enemies[i].Draw(xOff, yOff);
			}

			if (projectileActive)
			{
				al_draw_filled_circle(
					projectileX - xOff,
					projectileY - yOff,
					5,
					al_map_rgb(255, 255, 0)
				);
			}

			player.DrawSprites(xOff, yOff);
			al_draw_filled_rectangle(0, 0, WIDTH, 32, al_map_rgb(0, 0, 0));

			al_draw_textf(
				font,
				al_map_rgb(255, 255, 255),
				10,
				10,
				0,
				"Level: %d / 3   Time: %.1f   Lives: %d   Food: %d / %d   Ammo: %d   Defeated: %d",
				currentLevel,
				timeRemaining,
				lives,
				totalFoodCollected,
				foodNeeded,
				ammo,
				enemiesDefeated
			);

			if (needFoodMessageTimer > 0)
			{
				al_draw_text(
					font,
					al_map_rgb(255, 255, 0),
					WIDTH / 2,
					40,
					ALLEGRO_ALIGN_CENTER,
					"Collect all food before leaving!"
				);
			}

			if (noAmmoMessageTimer > 0)
			{
				al_draw_text(
					font,
					al_map_rgb(255, 255, 0),
					WIDTH / 2,
					65,
					ALLEGRO_ALIGN_CENTER,
					"No ammo! Collect food to gain ammo."
				);
			}

			al_flip_display();
			al_clear_to_color(al_map_rgb(0,0,0));
		}
	}
	if (gameWon)
	{
		al_clear_to_color(al_map_rgb(40, 120, 40));

		al_draw_text(
			font,
			al_map_rgb(255, 255, 255),
			WIDTH / 2,
			100,
			ALLEGRO_ALIGN_CENTER,
			"You completed all 3 maze levels!"
		);

		al_draw_textf(
			font,
			al_map_rgb(255, 255, 255),
			WIDTH / 2,
			150,
			ALLEGRO_ALIGN_CENTER,
			"Food Collected: %d",
			totalFoodCollected
		);

		al_draw_textf(
			font,
			al_map_rgb(255, 255, 255),
			WIDTH / 2,
			185,
			ALLEGRO_ALIGN_CENTER,
			"Lives Remaining: %d",
			lives
		);

		al_draw_textf(
			font,
			al_map_rgb(255, 255, 255),
			WIDTH / 2,
			255,
			ALLEGRO_ALIGN_CENTER,
			"Enemy Ants Defeated: %d",
			enemiesDefeated
		);

		al_draw_textf(
			font,
			al_map_rgb(255, 255, 255),
			WIDTH / 2,
			220,
			ALLEGRO_ALIGN_CENTER,
			"Final Time: %.1f seconds",
			totalFinalTime
		);

		player.DrawBig(WIDTH / 2 - 48, 300);

		al_flip_display();
		al_rest(5.0);
	}

	else if (gameOver)
	{
		al_clear_to_color(al_map_rgb(120, 20, 20));

		al_draw_text(
			font,
			al_map_rgb(255, 255, 255),
			WIDTH / 2,
			120,
			ALLEGRO_ALIGN_CENTER,
			"Game Over!"
		);

		al_draw_text(
			font,
			al_map_rgb(255, 255, 255),
			WIDTH / 2,
			170,
			ALLEGRO_ALIGN_CENTER,
			"You ran out of lives."
		);

		al_draw_textf(
			font,
			al_map_rgb(255, 255, 255),
			WIDTH / 2,
			220,
			ALLEGRO_ALIGN_CENTER,
			"Food Collected: %d",
			totalFoodCollected
		);

		al_draw_textf(
			font,
			al_map_rgb(255, 255, 255),
			WIDTH / 2,
			255,
			ALLEGRO_ALIGN_CENTER,
			"Level Reached: %d",
			currentLevel
		);

		al_draw_textf(
			font,
			al_map_rgb(255, 255, 255),
			WIDTH / 2,
			290,
			ALLEGRO_ALIGN_CENTER,
			"Enemy Ants Defeated: %d",
			enemiesDefeated
		);

		player.DrawBig(WIDTH / 2 - 48, 335);

		al_flip_display();
		al_rest(5.0);
	}

	else if (timeOver)
	{
		al_clear_to_color(al_map_rgb(0, 0, 0));

		al_draw_text(
			font,
			al_map_rgb(255, 255, 255),
			WIDTH / 2,
			HEIGHT / 2,
			ALLEGRO_ALIGN_CENTER,
			"Game Over! Time ran out."
		);

		al_flip_display();
		al_rest(5.0);
	}
	MapFreeMem();

	if (music)
		al_destroy_sample(music);

	if (collectSound)
		al_destroy_sample(collectSound);

	if (damageSound)
		al_destroy_sample(damageSound);

	if (winSound)
		al_destroy_sample(winSound);

	if (killSound)
		al_destroy_sample(killSound);

	if (shootSound)
		al_destroy_sample(shootSound);

	al_destroy_font(font);
	al_destroy_event_queue(event_queue);
	al_destroy_display(display);						//destroy our display object

	return 0;
}

int headCollided(int x, int y)
{
	BLKSTR* blockdata;
	blockdata = MapGetBlock(x / mapblockwidth, y / mapblockheight);

	// For head collision, check the bottom of the tile.
	// This lets the player jump through platforms that only have top collision.
	return blockdata->bl || blockdata->br;
}


int collided(int x, int y)
{
	BLKSTR *blockdata;
	blockdata = MapGetBlock(x/mapblockwidth, y/mapblockheight);
	return blockdata->tl;
}

bool endValue( int x, int y )
{

	BLKSTR* data;
	data = MapGetBlock( x/mapblockwidth, y/mapblockheight );

	if( data->user1 == 8 )
	{
		return true;
	}else
		return false;
}
bool foodValue(int x, int y)
{
	BLKSTR* data;
	data = MapGetBlock(x / mapblockwidth, y / mapblockheight);

	if (data->user1 == 5)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool hazardValue(int x, int y)
{
	BLKSTR* data;
	data = MapGetBlock(x / mapblockwidth, y / mapblockheight);

	if (data->user1 == 6)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool enemyPathClear(int x, int y, int moveDistance)
{
	for (int step = 0; step <= moveDistance; step += 16)
	{
		int checkX = x + step;

		if (collided(checkX, y) ||
			collided(checkX + 31, y) ||
			collided(checkX, y + 31) ||
			collided(checkX + 31, y + 31))
		{
			return false;
		}
	}

	return true;
}

bool findRandomEnemySpot(float& enemyX, float& enemyY, int moveDistance)
{
	for (int attempt = 0; attempt < 300; attempt++)
	{
		int tileX = rand() % mapwidth;
		int tileY = rand() % mapheight;

		int x = tileX * mapblockwidth;
		int y = tileY * mapblockheight;

		if (y < 40)
			continue;

		if (x < 180 && y < 180)
			continue;

		if (x + moveDistance + 32 >= mapwidth * mapblockwidth)
			continue;

		if (enemyPathClear(x, y, moveDistance))
		{
			enemyX = x;
			enemyY = y;
			return true;
		}
	}

	enemyX = 300;
	enemyY = 160;
	return false;
}

void setupEnemiesForLevel(EnemyAnt enemies[], int& enemyCount, int currentLevel)
{
	if (currentLevel == 1)
	{
		enemyCount = 7;
	}
	else if (currentLevel == 2)
	{
		enemyCount = 12;
	}
	else
	{
		enemyCount = 16;
	}

	for (int i = 0; i < enemyCount; i++)
	{
		float enemyX;
		float enemyY;

		int moveDistance = 64 + rand() % 65; // 64 to 128 pixels

		findRandomEnemySpot(enemyX, enemyY, moveDistance);

		enemies[i].InitEnemy("enemy.bmp", enemyX, enemyY, moveDistance);
	}
}
int countFoodTiles()
{
	int count = 0;

	for (int y = 0; y < mapheight; y++)
	{
		for (int x = 0; x < mapwidth; x++)
		{
			BLKSTR* data = MapGetBlock(x, y);

			if (data->user1 == 5)
			{
				count++;
			}
		}
	}

	return count;
}