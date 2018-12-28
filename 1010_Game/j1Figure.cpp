#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Figure.h"
#include "Figure_Templates.h"



j1Figure::j1Figure(fPoint position, Color color) : position(position), color(color){
	bool active;
	Color c_color;
	for (int row = 0; row < 3; row++) {
		for (int col = 0; col < 3; col++) {
			float x, y;
			
			if(col != 0)
				x = col * (cell_size + offset) + position.x;
			else
				x = col + position.x;

			if(row != 0)
				y = row * (cell_size + offset) + position.y;
			else
				y = row + position.y;

			switch (color) {
			case(Color::RED):
				active = r_figure[row][col];
				break;
			case(Color::BLUE):
				active = r_figure[row][col];
				break;
			case(Color::GREEN):
				active = r_figure[row][col];
				break;
			case(Color::YELLOW):
				active = r_figure[row][col];
				break;
			case(Color::PURPLE):
				active = r_figure[row][col];
				break;
			};

			if (!active)
				c_color = GREY;
			else
				c_color = RED;

			cells[row][col] = new Cell({ x,y },
				new SDL_Rect({ (int)x, (int)y, cell_size, cell_size }),
				active,
				c_color);
		}
	}

	rect = new SDL_Rect({ cells[0][0]->rect->x,
									 cells[0][0]->rect->y,
									 (cell_size + offset) * 3 - offset,
									 (cell_size + offset) * 3 - offset });
	Start();
}


j1Figure::~j1Figure(){}

bool j1Figure::Start() {
	enable = true;
	o_position = position;
	return true;
}

bool j1Figure::CleanUp() {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			delete cells[i][j];
		}
	}
	delete rect;
	return true;
}

bool j1Figure::Update(float dt) {
	if(enable)
		moveFigure();

	rect->x = position.x;
	rect->y = position.y;

	return true;
}

bool j1Figure::PostUpdate() {

	bool ret = true;
	uint alpha = 200;

	ret = App->render->DrawQuad(*rect, 0, 255, 255, alpha);

	for (int row = 0; row < 3; row++) {
		for (int col = 0; col < 3; col++) {
			switch (cells[row][col]->color) {
			case(Color::BLUE):
				ret = App->render->DrawQuad(*cells[row][col]->rect, 255, 255, 255, alpha);
				break;
			case(Color::GREEN):
				ret = App->render->DrawQuad(*cells[row][col]->rect, 255, 255, 255, alpha);
				break;
			case(Color::GREY):
				ret = App->render->DrawQuad(*cells[row][col]->rect, 255, 255, 255, alpha);
				break;
			case(Color::RED):
				ret = App->render->DrawQuad(*cells[row][col]->rect, 255, 0, 0, alpha);
				break;
			case(Color::YELLOW):
				ret = App->render->DrawQuad(*cells[row][col]->rect, 255, 255, 255, alpha);
				break;
			}
		}
	}
	return ret;
}

void j1Figure::moveFigure()
{
	check = false;
	if (MouseOnFigure() && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) {
		iPoint mousePos;
		App->input->GetMousePosition(mousePos.x, mousePos.y);

		if (setMouseGrabPos == false) {
			grabOffset = { (float)mousePos.x - position.x, (float)mousePos.y - position.y };
			setMouseGrabPos = true;
		}
	}
	else if (setMouseGrabPos == true && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
		iPoint mousePos;
		App->input->GetMousePosition(mousePos.x, mousePos.y);

		fPoint newGrabOffset;
		newGrabOffset = { (float)mousePos.x - position.x, (float)mousePos.y - position.y };

		fPoint mouseMov = { newGrabOffset.x - grabOffset.x, newGrabOffset.y - grabOffset.y };

		moveCells(mouseMov);
	}
	else if(App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP){
		setMouseGrabPos = false;
		check = true;
	}
}

bool j1Figure::MouseOnFigure() {
	SDL_Point mousePos;
	App->input->GetMousePosition(mousePos.x, mousePos.y);

	return !(position.y + rect->h < mousePos.y || position.y > mousePos.y || position.x + rect->w < mousePos.x || position.x > mousePos.x);
}

void j1Figure::resetFigure() {
	moveCells(o_position - position);

	position = o_position;
}

bool j1Figure::moveCells(fPoint movement) {
	position.x += movement.x;
	position.y += movement.y;
	for (int row = 0; row < 3; row++) {
		for (int col = 0; col < 3; col++) {
			cells[row][col]->position += movement;
			cells[row][col]->rect->x = cells[row][col]->position.x;
			cells[row][col]->rect->y = cells[row][col]->position.y;
		}
	}
	return true;
}

Cell::~Cell()
{
	delete rect;
}
