#ifndef __j1FADESCENE_H__
#define __j1FADESCENE_H__	// @Carles

#include "j1Module.h"
#include "SDL\include\SDL_rect.h"

enum class fade_type
{
	NONE = -1,
	START_GAME,
	RESTART,
	MAIN_MENU,
	SETTINGS,
	CREDITS,

	MAX_TYPES
};

enum class fade_step
{
	NONE = -1,
	FADING,
	FULLY_FADED,
	UNFADING
};

class j1FadeScene : public j1Module	//CHANGE/FIX: FadeToBlack has parameters that do nothing, needs rework
{
public:
	j1FadeScene();

	// Destructor
	virtual ~j1FadeScene();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration (graphic)
	bool Update();

	bool FadeToBlack(float time = 2.0f, fade_type = fade_type::NONE);

public:
	fade_step GetStep() const;
	fade_type GetType() const;
	fade_type ResetType();
	float GetDelay() const;

private:
	float delay;
	fade_step step = fade_step::NONE;
	fade_type type;

	Uint32 start_time = 0;
	Uint32 total_time = 0;
	SDL_Rect screen = { 0, 0, 0, 0 };

};

#endif //__FACESCENE_H__