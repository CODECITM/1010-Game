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
	sound = new SDL_Rect[3];
	/*exit = new SDL_Rect[4];
	shutDown = new SDL_Rect[4];
	settings = new SDL_Rect[4];
	back = new SDL_Rect[4];
	webpage = new SDL_Rect[4];*/
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	scene = (scene_type)config.attribute("start").as_int();

	//UI Data Awake
	pugi::xml_node item = config.child("ui").child("title");
	title = { item.attribute("x").as_int(), item.attribute("y").as_int(), item.attribute("w").as_int(), item.attribute("h").as_int() };

	item = config.child("ui").child("webpage");
	webpage = { item.attribute("x").as_int(), item.attribute("y").as_int(), item.attribute("w").as_int(), item.attribute("h").as_int() };

	item = config.child("ui").child("panels").child("normal");
	panelNormal = { item.attribute("x").as_int(), item.attribute("y").as_int(), item.attribute("w").as_int(), item.attribute("h").as_int() };

	item = config.child("ui").child("panels").child("short");
	panelShort = { item.attribute("x").as_int(), item.attribute("y").as_int(), item.attribute("w").as_int(), item.attribute("h").as_int() };

	item = config.child("ui").child("panels").child("long");
	panelLong = { item.attribute("x").as_int(), item.attribute("y").as_int(), item.attribute("w").as_int(), item.attribute("h").as_int() };

	item = config.child("ui").child("panels").child("square");
	panelSquare = { item.attribute("x").as_int(), item.attribute("y").as_int(), item.attribute("w").as_int(), item.attribute("h").as_int() };

	item = config.child("ui").child("window");
	window = { item.attribute("x").as_int(), item.attribute("y").as_int(), item.attribute("w").as_int(), item.attribute("h").as_int() };

	item = config.child("ui").child("leftArrow");
	leftArrow = { item.attribute("x").as_int(), item.attribute("y").as_int(), item.attribute("w").as_int(), item.attribute("h").as_int() };

	item = config.child("ui").child("rightArrow");
	rightArrow = { item.attribute("x").as_int(), item.attribute("y").as_int(), item.attribute("w").as_int(), item.attribute("h").as_int() };

	RegisterButtonData(config.child("ui").child("button"), button);

	item = config.child("ui").child("sound");
	sound[0] = { item.attribute("x1").as_int(), item.attribute("y1").as_int(), item.attribute("w1").as_int(), item.attribute("h1").as_int() };
	sound[1] = { item.attribute("x2").as_int(), item.attribute("y2").as_int(), item.attribute("w2").as_int(), item.attribute("h2").as_int() };
	sound[2] = { item.attribute("x3").as_int(), item.attribute("y3").as_int(), item.attribute("w3").as_int(), item.attribute("h3").as_int() };

	item = config.child("ui").child("restart");
	restart = { item.attribute("x").as_int(), item.attribute("y").as_int(), item.attribute("w").as_int(), item.attribute("h").as_int() };

	item = config.child("pieces");
	image_string.create(item.attribute("file").as_string());

	piece_colors.add(new SDL_Rect({ item.child("gray").attribute("x").as_int(),item.child("gray").attribute("y").as_int(),CELL_SIZE,CELL_SIZE }));
	piece_colors.add(new SDL_Rect({ item.child("lightRed").attribute("x").as_int(),item.child("lightRed").attribute("y").as_int(),CELL_SIZE,CELL_SIZE }));
	piece_colors.add(new SDL_Rect({ item.child("orange").attribute("x").as_int(),item.child("orange").attribute("y").as_int(),CELL_SIZE,CELL_SIZE }));
	piece_colors.add(new SDL_Rect({ item.child("yellow").attribute("x").as_int(),item.child("yellow").attribute("y").as_int(),CELL_SIZE,CELL_SIZE }));
	piece_colors.add(new SDL_Rect({ item.child("lightGreen").attribute("x").as_int(),item.child("lightGreen").attribute("y").as_int(),CELL_SIZE,CELL_SIZE }));
	piece_colors.add(new SDL_Rect({ item.child("green").attribute("x").as_int(),item.child("green").attribute("y").as_int(),CELL_SIZE,CELL_SIZE }));
	piece_colors.add(new SDL_Rect({ item.child("greenBlue").attribute("x").as_int(),item.child("greenBlue").attribute("y").as_int(),CELL_SIZE,CELL_SIZE }));
	piece_colors.add(new SDL_Rect({ item.child("cyan").attribute("x").as_int(),item.child("cyan").attribute("y").as_int(),CELL_SIZE,CELL_SIZE }));
	piece_colors.add(new SDL_Rect({ item.child("lightBlue").attribute("x").as_int(),item.child("lightBlue").attribute("y").as_int(),CELL_SIZE,CELL_SIZE }));
	piece_colors.add(new SDL_Rect({ item.child("blue").attribute("x").as_int(),item.child("blue").attribute("y").as_int(),CELL_SIZE,CELL_SIZE }));
	piece_colors.add(new SDL_Rect({ item.child("darkBlue").attribute("x").as_int(),item.child("darkBlue").attribute("y").as_int(),CELL_SIZE,CELL_SIZE }));
	piece_colors.add(new SDL_Rect({ item.child("darkPurple").attribute("x").as_int(),item.child("darkPurple").attribute("y").as_int(),CELL_SIZE,CELL_SIZE }));
	piece_colors.add(new SDL_Rect({ item.child("purple").attribute("x").as_int(),item.child("purple").attribute("y").as_int(),CELL_SIZE,CELL_SIZE }));
	piece_colors.add(new SDL_Rect({ item.child("lightPurple").attribute("x").as_int(),item.child("lightPurple").attribute("y").as_int(),CELL_SIZE,CELL_SIZE }));
	piece_colors.add(new SDL_Rect({ item.child("pink").attribute("x").as_int(),item.child("pink").attribute("y").as_int(),CELL_SIZE,CELL_SIZE }));
	piece_colors.add(new SDL_Rect({ item.child("red").attribute("x").as_int(),item.child("red").attribute("y").as_int(),CELL_SIZE,CELL_SIZE }));
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
		App->gui->CreateText(DEFAULT_POINT, "Start", WHITE_FONT, textFont, false, parent);
		parent = App->gui->CreateActionBox(&GoToSettings, { 576 / 2, 500 }, button, NULL, false);
		App->gui->CreateText(DEFAULT_POINT, "Settings", WHITE_FONT, textFont, false, parent);
		parent = App->gui->CreateActionBox(&GoToCredits, { 576 / 2, 650 }, button, NULL, false);
		App->gui->CreateText(DEFAULT_POINT, "Credits", WHITE_FONT, textFont, false, parent);

		App->gui->CreateCheckBox(&Mute, &App->audio->muted, { 576 - 115 / 2 - 30, 900 - 115 / 2 - 30 }, sound, NULL, false);
		break;
	case scene_type::CREDITS:
		App->gui->CreateButton(&GoToMenu, { 109 / 2 + 30, /*900 -*/129 / 2 + 30 }, leftArrow, NULL, false);

		App->gui->CreateImage({ 576 / 2, 440 }, window, NULL, false);
		App->gui->CreateText({ 576 / 2, 400 }, "Made by <CODE>", BLACK_FONT, titleFont, false);
		App->gui->CreateText({ 576 / 2, 440 }, "from the UPC - CITM", BLACK_FONT, titleFont, false);
		App->gui->CreateText({ 576 / 2, 480 }, "under the MIT License", BLACK_FONT, titleFont, false);

		App->gui->CreateButton(&OpenWebpage, { 576 - 123 / 2 - 30, 900 - 123 / 2 - 30 }, webpage, NULL, false);
		break;
	case scene_type::SETTINGS:
		App->gui->CreateButton(&GoToMenu, { 109 / 2 + 30, /*900 -*/129 / 2 + 30 }, leftArrow, NULL, false);

		parent = App->gui->CreateImage({ 576 / 2, 440 }, panelShort, NULL, false);
		difficultyTxt = (Text*)App->gui->CreateText(DEFAULT_POINT, "walapop", WHITE_FONT, titleFont, false, parent);
		CheckDifficulty(difficulty);

		App->gui->CreateButton(&DownDifficulty, { 576 / 2 - 200, 440 }, leftArrow, NULL, false);
		App->gui->CreateButton(&UpDifficulty, { 576 / 2 + 200, 440 }, rightArrow, NULL, false);

		break;
	case scene_type::GAME:
		//UI
		App->gui->CreateButton(&GoToMenu, { 109 / 2 + 30, /*900 -*/129 / 2 + 30 }, leftArrow, NULL, false);
		App->gui->CreateButton(&StartGame, { 576 - 114 / 2 - 30, /*900 -*/129 / 2 + 30 }, restart, NULL, false);
		App->gui->CreateCheckBox(&Mute, &App->audio->muted, { 576 - 115 / 2 - 30, 900 - 115 / 2 - 30 }, sound, NULL, false);
		
		App->gui->CreateText({ 576 / 2, 100 }, "SCORE", WHITE_FONT, titleFont, false);
		scoreTxt = (Text*)App->gui->CreateText({ 576 / 2, 150 }, "0", WHITE_FONT, textFont, false);

		//Initialize Game
		texture_bricks = App->tex->Load(image_string.GetString());

		cell_size = 30;
		cell_offset = 5;

		grid.position = { 80, 160 };

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

		//Music
		App->audio->PlayMusic(App->audio->musicMainMenu.GetString());
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

int j1Scene::UpdateScoreboard()
{
	p2SString tmpStr("%i", score);
	scoreTxt->ChangeContent(tmpStr);
	tmpStr.Clear();
	return score;
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
				App->gui->CreateText({ 576 / 2, 570 }, "GAME OVER", {255, 0, 0, 255}, App->font->titleFont, false);
				LOG("ENDGAME");
			}
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		figures.add(new j1Figure({ 80,100 }, PURPLE));
	}

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE)) {
		scene = scene_type::NONE;
		return false;
	}

	if (App->fade->GetStep() == fade_step::FULLY_FADED) {	// When game is fully faded, start game load and disable all entities for the next frame, then enable them.
		App->gui->active = false;

		switch (App->fade->GetType()) {
		case fade_type::MAIN_MENU:
			if (scene == scene_type::GAME) {
				CleanUp();
				App->audio->StopMusic();
			}

			ChangeScene(scene_type::MAIN_MENU);
			break;
		case fade_type::SETTINGS:
			ChangeScene(scene_type::SETTINGS);
			break;
		case fade_type::CREDITS:
			ChangeScene(scene_type::CREDITS);
			break;
		case fade_type::START_GAME:
			if (scene == scene_type::GAME)
				CleanUp();

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
	score = 0;

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

	App->tex->UnLoad(texture_bricks);
	scoreTxt = nullptr;
	difficultyTxt = nullptr;
	texture_bricks = nullptr;

	if (scene == scene_type::NONE) {
		for (p2List_item <SDL_Rect*>* item = piece_colors.start; item != nullptr; item = item->next) {
			RELEASE(item->data);
		}
		piece_colors.clear();
		image_string.Clear();

		RELEASE_ARRAY(button);
		RELEASE_ARRAY(sound);
	}

	return true;
}

bool j1Scene::deleteLines() {
	bool ret = false;
	if (del_time.ReadMs() > 30) {
		ret = true;
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
			score += 10;
			UpdateScoreboard();
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
			score += 10;
			UpdateScoreboard();
		}
	}
	return ret;

}

void j1Scene::createFigures() {
	float x = 80;
	Color color;
	for (int i = 0; i < 3; i++) {
		int r = rand() % 100; //is Working?? //SDL_GetTicks() % 100 => more random
		//INSERT FIGURES SPAWN
		if (r < dif_prov[difficulty][0]) { //EASY
			r = rand() % 5;
			if (r < 1)
				color = ORANGE;
			else if (r < 2)
				color = YELLOW;
			else if (r < 3)
				color = LIGHT_GREEN;
			else if (r < 4)
				color = GREEN;
			else if (r < 5)
				color = DARK_PURPLE;
		}else if (r < dif_prov[difficulty][1]) { //MEDIUM
			r = rand() % 5;
			if (r < 1)
				color = GREEN_BLUE;
			else if (r < 2)
				color = BLUE;
			else if (r < 3)
				color = PINK;
			else if (r < 4)
				color = RED;
			else if (r < 5)
			color = LIGHT_PURPLE;
			
		}
		else if (r < dif_prov[difficulty][2]) { //RARE
			r = rand() % 5;
			if (r < 1)
				color = PURPLE;
			else if (r < 2)
				color = DARK_BLUE;
			else if (r < 3)
				color = LIGHT_BLUE;
			else if (r < 4)
				color = LIGHT_RED;
			else if (r < 5)
				color = CYAN;
		}

		figures.add(new j1Figure({ x, 600 }, color));
		x += 155;
	}
}

bool j1Scene::checkFigures() {
	bool ret = true;

	iPoint cell;
	float distance = -1.0f;
	if (figures.count() == 0 || App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
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
					App->data->Placed();
					if (detectLines()) //Check if Game Stops unespectedly
						App->data->Scored(); 
					figures.del(item);
					check = true;
				}
				else {
					item->data->resetFigure();
					item->data->check = false;
					App->data->Returned();
				}
			}
			else {
				item->data->resetFigure();
				App->data->Returned();
			}
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

	this->scene = scene;
	Start();
}