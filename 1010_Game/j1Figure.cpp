#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Figure.h"
#include "Figure_Templates.h"
#include "j1Audio.h"
#include "j1Scene.h"
#include "j1Data.h"


j1Figure::j1Figure(fPoint position, Color color) : position(position), color(color){
	bool active;
	Color c_color;
	for (int row = 0; row < 3; row++) {
		for (int col = 0; col < 3; col++) {
			float x, y;
			
			if(col != 0)
				x = col * (CELL_SIZE + OFFSET) + position.x;
			else
				x = col + position.x;

			if(row != 0)
				y = row * (CELL_SIZE + OFFSET) + position.y;
			else
				y = row + position.y;

			switch (color) {
			case(Color::RED):
				active = r_figure[row][col];
				break;
			case(Color::BLUE):
				active = b_figure[row][col];
				break;
			case(Color::GREEN):
				active = g_figure[row][col];
				break;
			case(Color::YELLOW):
				active = y_figure[row][col];
				break;
			case(Color::PURPLE):
				active = p_figure[row][col];
				break;
			};

			if (!active)
				c_color = COLOR_NONE;
			else
				c_color = color;

			cells[row][col] = new Cell({ x,y },
				new SDL_Rect({ (int)x, (int)y, CELL_SIZE, CELL_SIZE }),
				active,
				c_color);
			check = false;
		}
	}

	rect = new SDL_Rect({ cells[0][0]->rect->x,
									 cells[0][0]->rect->y,
									 (CELL_SIZE + OFFSET) * 3 - OFFSET,
									 (CELL_SIZE + OFFSET) * 3 - OFFSET });
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
			RELEASE(cells[i][j]);
		}
	}
	RELEASE(rect);
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
	uint alpha = 255;

	//ret = App->render->DrawQuad(*rect, 0, 255, 255, alpha);

	for (int row = 0; row < 3; row++) {
		for (int col = 0; col < 3; col++) {
			cells[row][col]->Draw();
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
			App->audio->PlayFx(SFX_PIECE_PICKED);
			App->data->Picked();
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
	else if(setMouseGrabPos == true && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP){
		if(setMouseGrabPos)
			App->audio->PlayFx(SFX_PIECE_DROPPED);
		setMouseGrabPos = false;
		check = true;//CHECK
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
	RELEASE(rect);
}

void Cell::Draw()
{
	if (color != COLOR_NONE) {
		SDL_Rect* rect = App->scene->piece_colors.At(color)->data;
		App->render->Blit(App->scene->texture_bricks, position.x, position.y, rect);
	}
}
