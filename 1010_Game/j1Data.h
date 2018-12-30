#pragma once

#include "j1Module.h"
#include "j1Timer.h"
#include "p2List.h"
#include "p2Point.h"

#define AUTOSAVETIME 2.0f

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


public:

private:

	j1Timer autosave;
	int total_number_clicks;

	// 1st Step
	int clicks_to_play_button_save;
	float time_find_play_button_save;

	//2nd and 3rd Step
	j1Timer total_played_time;
	j1Timer time_picking_placing;
	j1Timer time_picking_returning;
	j1Timer time_to_score;

	float average_time_to_place;
	float average_time_to_return;
	float average_time_to_score;

	float average_time_to_place_save;
	float average_time_to_return_save;
	float average_time_to_score_save;

	//x stands for number of placed at that moment
	//y stands for average time at that moment
	p2List<fPoint> average_list_place;
	p2List<fPoint> average_list_return;
	p2List<fPoint> average_list_score;


	int total_actions;
	int total_scoring;
	int picked_blocks;
	int returned_blocks;
	int placed_blocks;

	bool b_returned;
};

