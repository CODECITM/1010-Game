#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL_rect.h"
#define MAX_FRAMES 25

class j1Animation
{
public:
	bool loop = true;
	float speed = 1.0f;
	SDL_Rect frames[MAX_FRAMES];

private:
	float current_frame;
	int last_frame = 0;
	int loops = 0;

public:

	void PushBack(const SDL_Rect& rect);

	SDL_Rect& AdvanceAnimation(float dt = 1);

	int GetCurrentFrame();
	SDL_Rect& GetFrameRect(int frame);

	bool Finished() const;
	void Reset();

	void SetFrame(int frame);
	bool IsLastFrame();

	// @Carles: Automatically allocates an animation that consists into a single row of frames of the same size
	void AllocAnimation(int posX, int posY, int sizeX, int sizeY, int numFrames);
};

#endif