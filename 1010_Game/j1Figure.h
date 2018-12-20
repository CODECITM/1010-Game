#ifndef __j1FIGURE_H__
#define __j1FIGURE_H__

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
	fPoint position;
	bool active;
	Color color;
	SDL_Rect* rect;

	Cell(fPoint position, SDL_Rect* rect, bool active, Color color) :
		position(position),
		active(active),
		color(color),
		rect(rect)
	{}
};

class j1Figure : public j1Module
{
public:
	j1Figure(fPoint position);
	~j1Figure();

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	void moveFigure();

	bool MouseOnFigure();

	bool moveCells(fPoint movement);

	void checkNearPoint();

public:
	fPoint grabOffset;
	fPoint position;
	Cell* cells[3][3];
	SDL_Rect* rect;
	bool setMouseGrabPos = false;
	bool check = false;

private:
	int offset = 10;
	int cell_size = 30;
};

#endif

