#include "j1Data.h"
#include "j1App.h"
#include "j1Input.h"
#include "p2Log.h"
#include "SDL/include/SDL_scancode.h"
#include "SDL/include/SDL_mouse.h"


j1Data::j1Data()
{
	name.create("data");

	total_number_clicks = 0;

	//To do
	clicks_to_play_button_save = 0;
	time_find_play_button_save = 0;

	total_actions  = 0;
	total_scoring  = 0;
	picked_blocks  = 0;
	returned_blocks= 0;
	placed_blocks  = 0;

	average_time_to_place  = 0.0f;
	average_time_to_return = 0.0f;
	average_time_to_score  = 0.0f;

	for (int i = 0; i < AVERAGE; i++) 
	{
		times_picking_placing[i] = -1.0f;
		times_picking_returning[i] = -1.0f;
	}

	//Done
}


j1Data::~j1Data()
{
}

bool j1Data::Start()
{
	total_played_time.Start();
	autosave.Start();

	return true;
}

bool j1Data::Update(float dt)
{

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		App->SaveGame();
	}

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) {
		total_number_clicks++;
	}

	if (autosave.ReadSec() >= AUTOSAVETIME) {
		App->SaveGame();
		autosave.Start();
	}

	return true;
}

bool j1Data::CleanUp()
{
	return true;
}

bool j1Data::Save(pugi::xml_node & node) const
{

	node.append_child("total_played_time").text().set(total_played_time.ReadSec());
	node.append_child("total_number_of_clicks").text().set(total_number_clicks);
	
	//1st Step
	pugi::xml_node first_step = node.append_child("first_step");

	first_step.append_child("clicks_to_find_play_button").text().set(clicks_to_play_button_save);
	first_step.append_child("time_to_find_play_button").text().set(time_find_play_button_save);

	//2nd Step
	pugi::xml_node second_step = node.append_child("second_step");

	second_step.append_child("average_time_to_place").text().set(average_time_to_place);
	second_step.append_child("average_time_to_return").text().set(average_time_to_return);
	second_step.append_child("average_time_to_score").text().set(average_time_to_score);

	second_step.append_child("total_actions").text().set(total_actions);
	second_step.append_child("total_scoring").text().set(total_scoring);
	second_step.append_child("total_picked_blocks").text().set(picked_blocks);
	second_step.append_child("total_returned_blocks").text().set(returned_blocks);
	second_step.append_child("total_placed_blocks").text().set(placed_blocks);


	return true;
}

void j1Data::Scored()
{
}

void j1Data::Picked()
{
	total_actions++;
	picked_blocks++;
	time_picking_placing.Start();
	time_picking_returning.Start();
}

void j1Data::Returned()
{
}

void j1Data::Placed()
{
}
