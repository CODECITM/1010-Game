#include "j1Animation.h"

void j1Animation::PushBack(const SDL_Rect& rect)
{
	frames[last_frame++] = rect;
}

SDL_Rect& j1Animation::AdvanceAnimation(float dt)
{
	current_frame += speed * dt;
	if (current_frame >= last_frame)
	{
		current_frame = (loop) ? 0.0f : last_frame - 1;
		loops++;
	}

	if (current_frame >= MAX_FRAMES)
	{
		Reset();
	}

	return frames[(int)current_frame];
}

int j1Animation::GetCurrentFrame()
{
	return (int)current_frame;
}

SDL_Rect& j1Animation::GetFrameRect(int frame)
{
	return frames[frame];
}

bool j1Animation::Finished() const
{
	return loops > 0;
}

void j1Animation::Reset()
{
	current_frame = 0;
	loops = 0;
}

void j1Animation::SetFrame(int frame) {
	current_frame = (float)frame;
	loops = 0;
}

bool j1Animation::IsLastFrame() {
	if (current_frame >= last_frame - 1) {
		return true;
	}
	else {
		return false;
	}
}

// @Carles: Automatically allocates an animation that consists into a single row of frames of the same size
void j1Animation::AllocAnimation(int posX, int posY, int sizeX, int sizeY, int numFrames) {

	int xCounter = 0;

	for (int i = numFrames; i > 0; i--) {
		PushBack({ posX + posY * xCounter++, posY,  sizeX, sizeY });
	}
}