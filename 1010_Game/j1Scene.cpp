#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1FadeScene.h"
#include "j1Scene.h"
#include "j1Window.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scenes");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{

	cell_size = 30;
	int offset = 10;

	grid.position = {50,230 };

	//GRID
	for (int row = 0; row < 10; row++) {
		for (int col = 0; col < 10; col++) {
			float x = (col + 1) * (cell_size + offset) + grid.position.x;
			float y = (row + 1) * (cell_size + offset) + grid.position.y;

			grid.cells[row][col] = new Cell({ x, y }, 
				new SDL_Rect({ (int)x, (int)y ,cell_size, cell_size }),
				false,
				Color::GREY);
		}
	}

	//FIGURE
	figures.add(new j1Figure({ 0,50 },RED));
	figures.add(new j1Figure({ 150,50 }, RED));
	figures.add(new j1Figure({ 300,50 }, RED));

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	bool ret = true;

	for (p2List_item <j1Figure*>* item = figures.start; item != nullptr; item = item->next) {
		item->data->Update(dt);
	}
	
	uint alpha = 255;
	for (int row = 0; row < 10; row++) {
		for (int col = 0; col < 10; col++) {
			switch (grid.cells[row][col]->color) {
			case(Color::BLUE):
				ret = App->render->DrawQuad(*grid.cells[row][col]->rect, 0, 0, 255, alpha);
				break;
			case(Color::GREEN):
				ret = App->render->DrawQuad(*grid.cells[row][col]->rect, 255, 255, 255, alpha);
				break;
			case(Color::GREY):
				ret = App->render->DrawQuad(*grid.cells[row][col]->rect, 255, 255, 255, alpha);
				break;
			case(Color::RED):
				ret = App->render->DrawQuad(*grid.cells[row][col]->rect, 255, 0, 0, alpha);
				break;
			case(Color::YELLOW):
				ret = App->render->DrawQuad(*grid.cells[row][col]->rect, 255, 255, 0, alpha);
				break;
			case(Color::PURPLE):
				ret = App->render->DrawQuad(*grid.cells[row][col]->rect, 255, 255, 0, alpha);
				break;
			}
		}
	}

	if (!checkFigures()) {
		LOG("ENDGAME");
	}

	return true;
}

bool j1Scene::checkPosibilities() {
	bool solution = false;
	for (p2List_item <j1Figure*>* item = figures.start; item != nullptr && !solution; item = item->next) {
		if (item->data->enable) {
			for (int row = 0; row < 10 && !solution; row++) {
				for (int col = 0; col < 10 && !solution; col++) {
					fPoint position1 = grid.cells[row][col]->position;
					fPoint position2 = item->data->cells[1][1]->position;
					fPoint movement = grid.cells[row][col]->position - item->data->cells[1][1]->position;
					item->data->moveCells(movement);
					if (isValid(iPoint({ row, col}), item->data,false)) {
						solution = true;
					}
				}
			}
			item->data->resetFigure();
		}
	}
	return solution;
}

bool j1Scene::checkFigures() {
	bool ret = true;

	iPoint cell;
	float distance = -1.0f;

	if (figures.count() == 0) {
		figures.add(new j1Figure({ 0,100 }, RED));
		figures.add(new j1Figure({ 150,100 }, RED));
		figures.add(new j1Figure({ 300,100 }, RED));
		ret = checkPosibilities();
	}

	for (p2List_item <j1Figure*>* item = figures.start; item != nullptr && ret; item = item->next) {
		if (item->data->check && item->data->enable) {
			for (int row = 0; row < 10 && ret; row++) {
				for (int col = 0; col < 10 && ret; col++) {
					if (grid.cells[row][col]->position.x < item->data->cells[1][1]->position.x - item->data->rect->w / 2 ||
						grid.cells[row][col]->position.x > item->data->cells[1][1]->position.x + item->data->rect->w / 2 ||
						grid.cells[row][col]->position.y < item->data->cells[1][1]->position.y - item->data->rect->h / 2 ||
						grid.cells[row][col]->position.y > item->data->cells[1][1]->position.y + item->data->rect->h / 2)
						continue;

					// distance calculus
					fPoint position1 = grid.cells[row][col]->position;
					fPoint position2 = item->data->cells[1][1]->position;
					float c_distance = position2.DistanceTo(position1);
					//if (c_distance < 10) {
					if (distance == -1.0f || c_distance < distance && c_distance) {
						LOG("distance:%f", c_distance);
						distance = c_distance;
						cell = iPoint({ row, col });
					}
					//}
				}
			}
			if (distance != -1.0f) {
				fPoint movement = grid.cells[cell.x][cell.y]->position - item->data->cells[1][1]->position;
				item->data->moveCells(movement);
				if (isValid(cell, item->data)) {
					figures.del(item);
					if(figures.count()!=0)
						ret = checkPosibilities();
				}
				else {
					item->data->resetFigure();
					item->data->check = false;
				}
			}
		}
	}
	return ret;
}

bool j1Scene::isValid(iPoint cell, j1Figure* figure,bool fill) {
	bool ret = true;
	p2List<iPoint> cells;
	Cell* p_cell;
	Cell* f_cell;
	for (int i = -1; i <= 1 ; i++) {
		for (int j = -1; j <= 1; j++) {
			f_cell = figure->cells[i + 1][j + 1];
			if (f_cell->active) {
				if ((cell.y + j >= 0 && cell.y + j < 10) && (cell.x + i >= 0 && cell.x + i < 10)) {
					p_cell = grid.cells[cell.x + i][cell.y + j];
					if (p_cell->position == f_cell->position
						&& !p_cell->active){
						if(fill)
							cells.add(iPoint({ cell.x + i, cell.y + j }));
					}
					else
						ret = false;
				}
				else
					ret = false;
			}
		}
	}

	if (ret) {
		for (p2List_item<iPoint>* item = cells.start; item != nullptr; item = item->next) {
			int row = item->data.x;
			int col = item->data.y;
			grid.cells[row][col]->active = true;
			grid.cells[row][col]->color = figure->color;
		}
	}

	cells.clear();

	return ret;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE))
		return false;

	//Create List of Current figures
	for (p2List_item <j1Figure*>* item = figures.start; item != nullptr; item = item->next) {
		if (item->data->enable)
			item->data->PostUpdate();
	}

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	for (p2List_item <j1Figure*>* item = figures.start; item != nullptr; item = item->next) {
			item->data->CleanUp();
			delete item->data;
	}
	figures.clear();

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			delete grid.cells[i][j];
			grid.cells[i][j] = nullptr;
		}
	}

	return true;
}

bool j1Scene::Load(pugi::xml_node& data)
{

	return true;
}

bool j1Scene::Save(pugi::xml_node& data) const
{

	return true;
}