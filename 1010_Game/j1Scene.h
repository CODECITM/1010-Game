#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;

enum Color {
	GREY = -1,
	RED,
	GREEN,
	YELLOW,
	BLUE
};

struct Cell {
	iPoint position;
	bool active;
	Color color;
	SDL_Rect* rect;

	Cell(iPoint position, SDL_Rect* rect, bool active, Color color) :
		position(position),
		active(active),
		color(color),
		rect(rect)
	{}
};

struct Grid {
	iPoint position;
	Cell* cells[10][10];
};

struct Figure {
	iPoint position;
	Cell* cells[3][3];
};


class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool Load(pugi::xml_node & data);

	bool Save(pugi::xml_node & data) const;

private:
	Figure grid;
	Figure red_figure;
	int cell_size;

};

#endif // __j1SCENE_H__