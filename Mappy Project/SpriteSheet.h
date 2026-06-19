#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <iostream>
using namespace std;
int headCollided(int x, int y);
class SpriteGrabber
{
public:
	SpriteGrabber();
	~SpriteGrabber();

	bool LoadSheet(const char* filename, int frameW, int frameH, int columns);
	void DrawFrame(int frame, float x, float y, bool flip);
	void DrawRotatedFrame(int frame, float x, float y, float angle);
	void DrawScaledFrame(int frame, float x, float y, float scale);

private:
	ALLEGRO_BITMAP* sheet;
	int frameWidth;
	int frameHeight;
	int animationColumns;
};
class Sprite
{
	friend int collided(int x, int y);
	friend bool endValue( int x, int y );
public:
	Sprite();
	~Sprite();
	void InitSprites(int width, int height);
	void UpdateSprites(int moveX, int moveY, int dir); //dir 1 = right, 0 = left, 2 = Standing Still
	void DrawSprites(int xoffset, int yoffset);
	bool CollideSprite();
	float getX() {return x;}
	float getY() {return y;}
	int getWidth() {return frameWidth;}
	int getHeight() {return frameHeight;}
	bool CollisionEndBlock();
	void StandStill();
	void ResetPosition(float startX, float startY);
	void SetHitEffect(bool value);
	void DrawBig(float x, float y);

private:
	float x;
	float y;

	int maxFrame;
	int curFrame;
	int frameCount;
	int frameDelay;
	int frameWidth;
	int frameHeight;
	int animationColumns;
	int animationRows;
	int animationDirection;
	bool hitEffect;
	float hitAngle;

	SpriteGrabber grabber;
};