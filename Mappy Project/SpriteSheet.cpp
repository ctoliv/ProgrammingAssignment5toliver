#include "SpriteSheet.h"
SpriteGrabber::SpriteGrabber()
{
	sheet = NULL;
	frameWidth = 0;
	frameHeight = 0;
	animationColumns = 1;
}

SpriteGrabber::~SpriteGrabber()
{
	if (sheet)
		al_destroy_bitmap(sheet);
}

bool SpriteGrabber::LoadSheet(const char* filename, int frameW, int frameH, int columns)
{
	sheet = al_load_bitmap(filename);

	if (!sheet)
	{
		cout << "Could not load sprite sheet: " << filename << endl;
		return false;
	}

	al_convert_mask_to_alpha(sheet, al_map_rgb(255, 255, 255));

	frameWidth = frameW;
	frameHeight = frameH;
	animationColumns = columns;

	return true;
}

void SpriteGrabber::DrawFrame(int frame, float x, float y, bool flip)
{
	int fx = (frame % animationColumns) * frameWidth;
	int fy = (frame / animationColumns) * frameHeight;

	int flags = 0;

	if (flip)
		flags = ALLEGRO_FLIP_HORIZONTAL;

	al_draw_bitmap_region(sheet, fx, fy, frameWidth, frameHeight, x, y, flags);
}
void SpriteGrabber::DrawRotatedFrame(int frame, float x, float y, float angle)
{
	int fx = (frame % animationColumns) * frameWidth;
	int fy = (frame / animationColumns) * frameHeight;

	ALLEGRO_BITMAP* frameImage = al_create_sub_bitmap(
		sheet,
		fx,
		fy,
		frameWidth,
		frameHeight
	);

	if (frameImage)
	{
		al_draw_rotated_bitmap(
			frameImage,
			frameWidth / 2,
			frameHeight / 2,
			x + frameWidth / 2,
			y + frameHeight / 2,
			angle,
			0
		);

		al_destroy_bitmap(frameImage);
	}
}

void SpriteGrabber::DrawScaledFrame(int frame, float x, float y, float scale)
{
	int fx = (frame % animationColumns) * frameWidth;
	int fy = (frame / animationColumns) * frameHeight;

	ALLEGRO_BITMAP* frameImage = al_create_sub_bitmap(
		sheet,
		fx,
		fy,
		frameWidth,
		frameHeight
	);

	if (frameImage)
	{
		al_draw_scaled_bitmap(
			frameImage,
			0,
			0,
			frameWidth,
			frameHeight,
			x,
			y,
			frameWidth * scale,
			frameHeight * scale,
			0
		);

		al_destroy_bitmap(frameImage);
	}
}

Sprite::Sprite()
{
	
}
Sprite::~Sprite()
{
	
}
void Sprite::InitSprites(int width, int height)
{
	x = 80;
	y = 80;


	maxFrame = 3;
	curFrame = 0;
	frameCount = 0;
	frameDelay = 6;
	frameWidth = 32;
	frameHeight = 32;
	animationColumns = 4;
	animationDirection = 1;
	hitEffect = false;
	hitAngle = 0;

	grabber.LoadSheet("hero1.bmp", frameWidth, frameHeight, animationColumns);
}

void Sprite::UpdateSprites(int moveX, int moveY, int dir)
{
    int oldx = x;
    int oldy = y;

    animationDirection = dir;

    x += moveX;
    y += moveY;

    // Check all four corners of the sprite.
    if (collided(x, y) ||
        collided(x + frameWidth - 1, y) ||
        collided(x, y + frameHeight - 1) ||
        collided(x + frameWidth - 1, y + frameHeight - 1))
    {
        x = oldx;
        y = oldy;
    }

    if (++frameCount > frameDelay)
    {
        frameCount = 0;

        if (++curFrame > maxFrame)
            curFrame = 0;
    }
}
void Sprite::StandStill()
{
	curFrame = 0;
}

void Sprite::SetHitEffect(bool value)
{
	hitEffect = value;
	if (value)
	{
		hitAngle = 0;
	}
}

bool Sprite::CollisionEndBlock()
{
	if (endValue(x + frameWidth / 2, y + frameHeight / 2))
		return true;
	else
		return false;
}

void Sprite::DrawSprites(int xoffset, int yoffset)
{
	float drawX = x - xoffset;
	float drawY = y - yoffset;

	if (hitEffect)
	{
		// Spin the ant while it is in the hit effect.
		grabber.DrawRotatedFrame(curFrame, drawX, drawY, hitAngle);

		hitAngle += 0.25;

		return;
	}

	if (animationDirection == 0) // up
	{
		grabber.DrawRotatedFrame(curFrame, drawX, drawY, -ALLEGRO_PI / 2);
	}
	else if (animationDirection == 1) // down
	{
		grabber.DrawRotatedFrame(curFrame, drawX, drawY, ALLEGRO_PI / 2);
	}
	else if (animationDirection == 2) // left
	{
		grabber.DrawFrame(curFrame, drawX, drawY, true);
	}
	else // right
	{
		grabber.DrawFrame(curFrame, drawX, drawY, false);
	}
}


void Sprite::ResetPosition(float startX, float startY)
{
    x = startX;
    y = startY;
    curFrame = 0;
}
void Sprite::DrawBig(float x, float y)
{
	// Draw a larger ant image for the ending screen.
	grabber.DrawScaledFrame(0, x, y, 3.0);
}

EnemyAnt::EnemyAnt()
{
	x = 0;
	y = 0;
	startX = 0;
	distance = 120;
	speed = 2;
	direction = 1;
	active = false;

	maxFrame = 3;
	curFrame = 0;
	frameCount = 0;
	frameDelay = 6;
	frameWidth = 32;
	frameHeight = 32;
	animationColumns = 4;
}

EnemyAnt::~EnemyAnt()
{

}

void EnemyAnt::InitEnemy(const char* filename, float startXValue, float startYValue, float moveDistance)
{
	x = startXValue;
	y = startYValue;
	startX = startXValue;
	distance = moveDistance;
	speed = 2;
	direction = 1;
	active = true;

	maxFrame = 3;
	curFrame = 0;
	frameCount = 0;
	frameDelay = 6;
	frameWidth = 32;
	frameHeight = 32;
	animationColumns = 4;

	grabber.LoadSheet(filename, frameWidth, frameHeight, animationColumns);
}

void EnemyAnt::Update()
{

	if (!active)
	{
		return;
	}

	float oldX = x;
	float oldY = y;

	x += speed * direction;

	if (x > startX + distance)
	{
		direction = -1;
	}
	else if (x < startX)
	{
		direction = 1;
	}

	// If the enemy hits a wall, move back and reverse direction.
	if (collided(x, y) ||
		collided(x + frameWidth - 1, y) ||
		collided(x, y + frameHeight - 1) ||
		collided(x + frameWidth - 1, y + frameHeight - 1))
	{
		x = oldX;
		y = oldY;
		direction *= -1;
	}

	if (++frameCount > frameDelay)
	{
		frameCount = 0;

		if (++curFrame > maxFrame)
		{
			curFrame = 0;
		}
	}
}

void EnemyAnt::Draw(int xoffset, int yoffset)
{
	if (!active)
	{
		return;
	}

	float drawX = x - xoffset;
	float drawY = y - yoffset;

	if (direction == -1)
	{
		grabber.DrawFrame(curFrame, drawX, drawY, true);
	}
	else
	{
		grabber.DrawFrame(curFrame, drawX, drawY, false);
	}
}

void EnemyAnt::Reset(float startXValue, float startYValue, float moveDistance)
{
	x = startXValue;
	y = startYValue;
	startX = startXValue;
	distance = moveDistance;
	direction = 1;
	curFrame = 0;
}

bool EnemyAnt::CollidesWith(Sprite& player)
{
	if (!active)
	{
		return false;
	}

	if (player.getX() < x + frameWidth &&
		player.getX() + player.getWidth() > x &&
		player.getY() < y + frameHeight &&
		player.getY() + player.getHeight() > y)
	{
		return true;
	}

	return false;
}

void EnemyAnt::Deactivate()
{
	active = false;
}

bool EnemyAnt::IsActive()
{
	return active;
}