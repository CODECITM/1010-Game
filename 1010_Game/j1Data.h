#pragma once

#include "j1Module.h"
#include "j1Timer.h"
#include "p2List.h"
#include "p2Point.h"

#define AUTOSAVETIME 5.0f

struct GameData {

	//2nd and 3rd Step
	j1Timer current_game_time;
	j1Timer time_picking_placing;
	j1Timer time_picking_returning;
	j1Timer time_to_score;

	float average_time_to_place  = 0.0f;
	float average_time_to_return = 0.0f;
	float average_time_to_score  = 0.0f;

	float average_time_to_place_save = 0.0f;
	float average_time_to_return_save= 0.0f;
	float average_time_to_score_save = 0.0f;

	//x stands for number of placed at that moment
	//y stands for average time at that moment
	p2List<fPoint> average_list_place;
	p2List<fPoint> average_list_return;
	p2List<fPoint> average_list_score;


	int total_actions  = 0;
	int total_scoring  = 0;
	int picked_blocks  = 0;
	int returned_blocks= 0;
	int placed_blocks  = 0;
};

class j1Data :	public j1Module
{
public:

	j1Data();
	~j1Data();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	bool Save(pugi::xml_node & node) const;

	void Scored();

	void Picked();

	void Returned();

	void Placed();

	void StartGame();

	void Restart();

	void ChangeGameType();

	void GoToMenu();

	void StartTimer();


public:

private:


	p2List<GameData*> gamesA;
	p2List<GameData*> gamesB;
	p2List<GameData*>* game_type[2];
	int current_game[2] = { 0, 0};
	//p2List_item<GameData*>* current_game;


	j1Timer autosave;
	int total_number_clicks;

	j1Timer total_played_time;

	int current_type;

	// 1st Step
	int clicks_to_play_button_save;
	float time_find_play_button_save;

	bool one_minute;
	bool initialized;

	bool first_entering;
	bool b_returned;
};

