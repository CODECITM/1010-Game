#ifndef __j1FIGURE_H__
#define __j1FIGURE_H__

struct SDL_Texture;

enum Color {
	COLOR_NONE = -1,
	GREY,
	LIGHT_RED,
	ORANGE,
	YELLOW,
	LIGHT_GREEN,
	GREEN,
	GREEN_BLUE,
	CYAN,
	LIGHT_BLUE,
	BLUE,
	DARK_BLUE,
	DARK_PURPLE,
	PURPLE,
	LIGHT_PURPLE,
	PINK,
	RED,
};

struct Cell {
	Cell(fPoint position, SDL_Rect* rect, bool active, Color color) :
		position(position),
		active(active),
		color(color),
		rect(rect)
	{}
	~Cell();

	void Draw();

	fPoint position;
	bool active;
	Color color;
	SDL_Rect* rect;
};

class j1Figure
{
public:
	j1Figure(fPoint position, Color color);
	~j1Figure();

	bool Start();

	bool CleanUp();

	bool Update(float dt);

	bool PostUpdate();

	void moveFigure();

	bool MouseOnFigure();

	void resetFigure();

	bool moveCells(fPoint movement);

public:
	fPoint grabOffset;
	fPoint o_position;
	fPoint position;
	Cell* cells[3][3];
	SDL_Rect* rect;
	bool setMouseGrabPos = false;
	bool check = false;
	Color color;
	bool enable = false;

private:
	int offset = 5;
	int cell_size = 30;
};

#endif

