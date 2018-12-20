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
	red_figure = new j1Figure({ 0,0 });

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

	red_figure->Update(dt);
	
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
				ret = App->render->DrawQuad(*grid.cells[row][col]->rect, 255, 255, 255, alpha);
				break;
			case(Color::YELLOW):
				ret = App->render->DrawQuad(*grid.cells[row][col]->rect, 255, 255, 255, alpha);
				break;
			}
		}
	}

	checkFigures();

	return true;
}

void j1Scene::checkFigures() {
	if (red_figure->check) {
		for (int row = 0; row < 10; row++) {
			for (int col = 0; col < 10; col++) {
				if (grid.cells[row][col]->position.x < red_figure->cells[1][1]->position.x - red_figure->rect->w / 2 ||
					grid.cells[row][col]->position.x > red_figure->cells[1][1]->position.x + red_figure->rect->w / 2 ||
					grid.cells[row][col]->position.y < red_figure->cells[1][1]->position.y - red_figure->rect->h / 2 ||
					grid.cells[row][col]->position.y > red_figure->cells[1][1]->position.y + red_figure->rect->h / 2)
					continue;

				grid.cells[row][col]->color = Color::BLUE;
				// hiponetusa calculus

			}
		}
	}
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE))
		return false;

	//Create List of Current figures
	red_figure->PostUpdate();

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
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