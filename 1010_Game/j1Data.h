#pragma once

#include "j1Module.h"
#include "j1Timer.h"
#include "p2List.h"
#include "p2Point.h"

#define AUTOSAVETIME 2.0f

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


public:

private:

	p2List<GameData*> games;

	j1Timer autosave;
	int total_number_clicks;

	j1Timer total_played_time;

	int current_game;

	// 1st Step
	int clicks_to_play_button_save;
	float time_find_play_button_save;

	bool b_returned;
};

