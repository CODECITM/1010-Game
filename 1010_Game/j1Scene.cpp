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
#include "j1Fonts.h"
#include "j1UserInterface.h"
#include "Image.h"
#include "Text.h"
#include "Button.h"
#include "ActionBox.h"
#include "ButtonActions.h"
#include "j1Data.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scenes");

	button = new SDL_Rect[4];
	checkButton = new SDL_Rect[3];
	exit = new SDL_Rect[4];
	shutDown = new SDL_Rect[4];
	settings = new SDL_Rect[4];
	back = new SDL_Rect[4];
	webpage = new SDL_Rect[4];
}

// Destructor
j1Scene::~j1Scene()
{
	RELEASE_ARRAY(button);
	RELEASE_ARRAY(checkButton);
	RELEASE_ARRAY(exit);
	RELEASE_ARRAY(shutDown);
	RELEASE_ARRAY(settings);
	RELEASE_ARRAY(back);
	RELEASE_ARRAY(webpage);
}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	scene = (scene_type)config.attribute("start").as_int();

	//UI Data Awake
	pugi::xml_node item = config.child("ui").child("title");
	title = { item.attribute("x").as_int(), item.attribute("y").as_int(), item.attribute("w").as_int(), item.attribute("h").as_int() };

	item = config.child("ui").child("panel");
	panel = { item.attribute("x").as_int(), item.attribute("y").as_int(), item.attribute("w").as_int(), item.attribute("h").as_int() };

	item = config.child("ui").child("window");
	window = { item.attribute("x").as_int(), item.attribute("y").as_int(), item.attribute("w").as_int(), item.attribute("h").as_int() };

	RegisterButtonData(config.child("ui").child("button"), button);

	item = config.child("ui").child("checkButton");
	checkButton[0] = { item.attribute("x1").as_int(), item.attribute("y1").as_int(), item.attribute("w1").as_int(), item.attribute("h1").as_int() };
	checkButton[1] = { item.attribute("x2").as_int(), item.attribute("y2").as_int(), item.attribute("w2").as_int(), item.attribute("h2").as_int() };
	checkButton[2] = { item.attribute("x3").as_int(), item.attribute("y3").as_int(), item.attribute("w3").as_int(), item.attribute("h3").as_int() };

	RegisterButtonData(config.child("ui").child("exit"), exit);
	RegisterButtonData(config.child("ui").child("shutDown"), shutDown);
	RegisterButtonData(config.child("ui").child("settings"), settings);
	RegisterButtonData(config.child("ui").child("back"), back);
	RegisterButtonData(config.child("ui").child("webpage"), webpage);


	item = config.child("pieces");
	image_string.create(item.attribute("file").as_string());

	piece_colors.add(new SDL_Rect({ item.child("gray").attribute("x").as_int(),item.child("gray").attribute("y").as_int(),CELL_SIZE,CELL_SIZE }));
	piece_colors.add(new SDL_Rect({ item.child("red").attribute("x").as_int(),item.child("red").attribute("y").as_int(),CELL_SIZE,CELL_SIZE }));
	piece_colors.add(new SDL_Rect({ item.child("green").attribute("x").as_int(),item.child("green").attribute("y").as_int(),CELL_SIZE,CELL_SIZE }));
	piece_colors.add(new SDL_Rect({ item.child("yellow").attribute("x").as_int(),item.child("yellow").attribute("y").as_int(),CELL_SIZE,CELL_SIZE }));
	piece_colors.add(new SDL_Rect({ item.child("blue").attribute("x").as_int(),item.child("blue").attribute("y").as_int(),CELL_SIZE,CELL_SIZE }));
	piece_colors.add(new SDL_Rect({ item.child("purple").attribute("x").as_int(),item.child("purple").attribute("y").as_int(),CELL_SIZE,CELL_SIZE }));

	return ret;
}

void j1Scene::RegisterButtonData(pugi::xml_node& node, SDL_Rect* button)
{
	button[0] = { node.attribute("x1").as_int(), node.attribute("y1").as_int(), node.attribute("w1").as_int(), node.attribute("h1").as_int() };
	button[1] = { node.attribute("x2").as_int(), node.attribute("y2").as_int(), node.attribute("w2").as_int(), node.attribute("h2").as_int() };
	button[2] = { node.attribute("x3").as_int(), node.attribute("y3").as_int(), node.attribute("w3").as_int(), node.attribute("h3").as_int() };
	button[3] = { node.attribute("x4").as_int(), node.attribute("y4").as_int(), node.attribute("w4").as_int(), node.attribute("h4").as_int() };
}

// Called before the first frame
bool j1Scene::Start()
{
	bool ret = true;

	UIElement* parent;
	_TTF_Font* titleFont = App->font->titleFont;
	_TTF_Font* textFont = App->font->textFont;
	iPoint screenSize;
	App->win->GetWindowSize((uint&)screenSize.x, (uint&)screenSize.y);

	//Scene Start
	switch (scene) {
	case scene_type::MAIN_MENU:

		App->gui->CreateImage({ 576 / 2, 180 }, title, NULL, false);
		parent = App->gui->CreateActionBox(&StartGame, { 576 / 2, 350 }, button, NULL, false);
		App->gui->CreateText(DEFAULT_POINT, "Start", DEFAULT_COLOR, textFont, false, parent);
		parent = App->gui->CreateActionBox(&GoToSettings, { 576 / 2, 500 }, button, NULL, false);
		App->gui->CreateText(DEFAULT_POINT, "Settings", DEFAULT_COLOR, textFont, false, parent);
		parent = App->gui->CreateActionBox(&GoToCredits, { 576 / 2, 650 }, button, NULL, false);
		App->gui->CreateText(DEFAULT_POINT, "Credits", DEFAULT_COLOR, textFont, false, parent);
		App->gui->CreateActionBox(&CloseGame, { 20, 20 }, shutDown, NULL, false);
		App->gui->CreateActionBox(&OpenWebpage, { 55, 20 }, webpage, NULL, false);

		//App->audio->PlayMusic(App->audio->musicMainMenu.GetString());
		break;
	case scene_type::SETTINGS:

		parent = App->gui->CreateActionBox(&GoToMenu, { 576 / 2, 500 }, button, NULL, false);
		App->gui->CreateText(DEFAULT_POINT, "pop", DEFAULT_COLOR, textFont, false, parent);

		break;
	case scene_type::CREDITS:
		
		parent = App->gui->CreateActionBox(&GoToMenu, { 576 / 2, 500 }, button, NULL, false);
		App->gui->CreateText(DEFAULT_POINT, "pop", DEFAULT_COLOR, textFont, false, parent);

		break;
	case scene_type::GAME:
		texture_bricks = App->tex->Load(image_string.GetString());

		cell_size = 30;
		cell_offset = 5;

		grid.position = { 80,230 };

		//GRID
		for (int row = 0; row < 10; row++) {
			for (int col = 0; col < 10; col++) {
				float x = (col + 1) * (CELL_SIZE + OFFSET) + grid.position.x;
				float y = (row + 1) * (CELL_SIZE + OFFSET) + grid.position.y;

				grid.cells[row][col] = new Cell({ x, y },
					new SDL_Rect({ (int)x, (int)y ,CELL_SIZE, CELL_SIZE }),
					false,
					Color::GREY);
			}
		}

		createFigures();
		break;
	}

	App->audio->MusicToMixer();
	App->audio->SfxToMixer();

	return ret;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	/*for (p2List_item <j1Figure*>* item = figures.start; item != nullptr; item = item->next) {
		if(!item->data->enable)
			figures.del(item);
	}*/
	
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	bool ret = true;
	
	if (scene == scene_type::GAME) {
		for (p2List_item <j1Figure*>* item = figures.start; item != nullptr; item = item->next) {
			item->data->Update(dt);
		}

		if (deleteLines()) {
			if (!checkFigures()) {
				LOG("ENDGAME");
			}
		}
	}

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE))
		return false;

	if (App->fade->GetStep() == fade_step::FULLY_FADED) {	// When game is fully faded, start game load and disable all entities for the next frame, then enable them.
		App->gui->active = false;

		switch (App->fade->GetType()) {	//CHANGE/FIX: This should be a function
		case fade_type::MAIN_MENU:
			ChangeScene(scene_type::MAIN_MENU);
			break;
		case fade_type::SETTINGS:
			ChangeScene(scene_type::SETTINGS);
			break;
		case fade_type::CREDITS:
			ChangeScene(scene_type::CREDITS);
			break;
		case fade_type::START_GAME:
			ChangeScene(scene_type::GAME);
			break;
		case fade_type::RESTART:
			App->gui->CleanUp();
			break;
		}
	}
	else if (App->gui->active == false && App->fade->GetStep() == fade_step::NONE) {	//CHANGE/FIX: Avoids bugs, but could be improved
		App->fade->ResetType();
		App->gui->active = true;
	}

	if (scene == scene_type::GAME) {
		//DRAW EVEYTHING
		uint alpha = 255;
		for (int row = 0; row < 10; row++) {
			for (int col = 0; col < 10; col++) {
				grid.cells[row][col]->Draw();
			}
		}

		//Draw Figures
		for (p2List_item <j1Figure*>* item = figures.start; item != nullptr; item = item->next) {
			if (item->data->enable)
				item->data->PostUpdate();
		}
	}
	else {
		SDL_Rect screen = { 0, 0, 0, 0 };
		App->win->GetWindowSize((uint&)screen.w, (uint&)screen.h);
		App->render->DrawQuad(screen, 255, 255, 255);
	}

	App->gui->Draw();

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	if (scene == scene_type::GAME) {
		for (p2List_item <j1Figure*>* item = figures.start; item != nullptr; item = item->next) {
			item->data->CleanUp();
			RELEASE(item->data);
		}
		figures.clear();

		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				RELEASE(grid.cells[i][j]);
			}
		}
	}

	return true;
}

bool j1Scene::deleteLines() {
	bool ret = true;
	if (del_time.ReadMs() > 30) {
		del_time.Start();
		for (p2List_item<Line>* item = lines.start; item != nullptr; item = item->next) {
			int index = item->data.index;
			int col = item->data.col;
			int row = item->data.row;
			if (index < 10) {
				if (col != -1) {
					grid.cells[index][col]->active = false;
					grid.cells[index][col]->color = Color::GREY;
				}
				else if (row != -1) {
					grid.cells[row][index]->active = false;
					grid.cells[row][index]->color = Color::GREY;
				}
				item->data.index += 1;
				ret = false;
				App->audio->PlayFx(SFX_BRICK_DESTROYED);
			}
			else {
				check = true;
				lines.del(item);
			}
		}
	}
	return ret;
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

bool j1Scene::detectLines() {
	bool ret = false;
	Line line;
	del_time.Start();
	for (int row = 0; row < 10; row++) {
		bool del = true;
		for (int col = 0; col < 10; col++) {
			if (!grid.cells[row][col]->active)
				del = false;
		}
		if (del) {
			line.col = -1;
			line.row = row;
			lines.add(line);
			ret = true;
			//ADD POINTS @Eric
		}
	}

	for (int col = 0; col < 10; col++) {
		bool del = true;
		for (int row = 0; row < 10; row++) {
			if (!grid.cells[row][col]->active)
				del = false;
		}
		if (del) {
			line.row = -1;
			line.col = col;
			lines.add(line);
			ret = true;
			//ADD POINTS @Eric
		}
	}
	return ret;

}

void j1Scene::createFigures() {
	float x = 80;
	Color color;
	for (int i = 0; i < 3; i++) {
		int r = rand() % 100; //is Working??
		//INSERT FIGURES SPAWN
		if (r < 50) { //EASY

		}else if (r < 80) { //MEDIUM

		}
		else if (r < 100) { //HARD
		
		} 


		if (r < 40)
			color = RED;
		else if (r < 60)
			color = BLUE;
		else if (r < 70)
			color = PURPLE;
		else if (r < 80)
			color = YELLOW;
		else if (r < 100)
			color = GREEN;

		figures.add(new j1Figure({ x,100 }, color));
		x += 155;
	}
}

bool j1Scene::checkFigures() {
	bool ret = true;

	iPoint cell;
	float distance = -1.0f;
	if (figures.count() == 0) {
		figures.clear();

		createFigures();
		
		check = true;
	}

	if (check) //Check Possible Moves
		ret = checkPosibilities(),
		check = false;


	p2List_item <j1Figure*>* item = figures.start;
	while(item != nullptr && ret) {
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
						//LOG("distance:%f", c_distance);
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
					detectLines(); //Check if Game Stops unespectedly
					figures.del(item);
					check = true;
				}
				else {
					item->data->resetFigure();
					item->data->check = false;
					App->data->Returned();
				}
			}else
				item->data->resetFigure();
		}
		item = item->next;
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

bool j1Scene::Load(pugi::xml_node& data)
{

	return true;
}

bool j1Scene::Save(pugi::xml_node& data) const
{

	return true;
}

void j1Scene::ChangeScene(scene_type scene)
{
	App->gui->CleanUp();
	CleanUp();

	this->scene = scene;

	Start();
}