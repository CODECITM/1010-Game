#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1FadeToBlack.h"
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
	int world_pos = 30;

	grid.position = {10,0 };

	//GRID
	for (int row = 0; row < 10; row++) {
		for (int col = 0; col < 10; col++) {
			grid.cells[row][col] = new Cell({ (row + 1),(col + 1) }, new SDL_Rect({ (row + 1) * (cell_size + offset + grid.position.x) , (col + 1) * (cell_size + offset + grid.position.y) ,cell_size, cell_size }), false, Color::GREY);
		}
	}


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

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE))
		return false;
	uint alpha = 100;
	for (int row = 0; row < 10; row++) {
		for (int col = 0; col < 10; col++) {
			switch (grid.cells[row][col]->color) {
			case(Color::BLUE):
				ret = App->render->DrawQuad(*grid.cells[row][col]->rect, 255, 255, 255, alpha);
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