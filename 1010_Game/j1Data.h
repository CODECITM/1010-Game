#pragma once

#include "j1Module.h"
#include "j1Timer.h"

#define AVERAGE 5
#define AUTOSAVETIME 5.0f

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

	float times_picking_placing[AVERAGE];
	float times_picking_returning[AVERAGE];

	float average_time_to_place;
	float average_time_to_return;
	float average_time_to_score;

	int total_actions;
	int total_scoring;
	int picked_blocks;
	int returned_blocks;
	int placed_blocks;


};

