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
	current_game = 0;

	//To do
	clicks_to_play_button_save = 0;
	time_find_play_button_save = 0;

	games.add(new GameData());
	games.At(current_game)->data->total_actions  = 0;
	games.At(current_game)->data->total_scoring  = 0;
	games.At(current_game)->data->picked_blocks  = 0;
	games.At(current_game)->data->returned_blocks= 0;
	games.At(current_game)->data->placed_blocks  = 0;

	games.At(current_game)->data->average_time_to_place  = 0.0f;
	games.At(current_game)->data->average_time_to_return = 0.0f;
	games.At(current_game)->data->average_time_to_score  = 0.0f;

	games.At(current_game)->data->average_time_to_place_save  = 0.0f;
	games.At(current_game)->data->average_time_to_return_save = 0.0f;
	games.At(current_game)->data->average_time_to_score_save  = 0.0f;

	b_returned = false;

	//Done
}


j1Data::~j1Data()
{
}

bool j1Data::Start()
{
	total_played_time.Start();
	games.At(current_game)->data->current_game_time.Start();
	autosave.Start();

	return true;
}

bool j1Data::Update(float dt)
{
	b_returned = false;

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
	//average_list_place.clear();
	//average_list_return.clear();
	//average_list_score.clear();
	return true;
}


bool j1Data::Save(pugi::xml_node & node) const
{
	char c[32];

	sprintf_s(c, sizeof(c),"%.2f" , total_played_time.ReadSec());
	node.append_child("total_played_time").text().set(c);
	node.append_child("total_number_of_clicks").text().set(total_number_clicks);
	
	//1st Step
	pugi::xml_node first_step = node.append_child("first_step");

	first_step.append_child("clicks_to_find_play_button").text().set(clicks_to_play_button_save);
	sprintf_s(c, sizeof(c), "%.2f", time_find_play_button_save);
	first_step.append_child("time_to_find_play_button").text().set(c);


	for (int current_game = 0; current_game < games.count(); current_game++) {

		sprintf_s(c, sizeof(c), "GAME_%d", current_game);
		pugi::xml_node game = node.append_child(c);

		sprintf_s(c, sizeof(c), "%.2f", games.At(current_game)->data->current_game_time.ReadSec());
		game.append_child("game_time").text().set(c);

		//2nd Step
		pugi::xml_node second_step = game.append_child("second_step");

		sprintf_s(c, sizeof(c), "%.2f", games.At(current_game)->data->average_time_to_place_save);
		second_step.append_child("average_time_to_place").text().set(c);
		sprintf_s(c, sizeof(c), "%.2f", games.At(current_game)->data->average_time_to_return_save);
		second_step.append_child("average_time_to_return").text().set(c);
		sprintf_s(c, sizeof(c), "%.2f", games.At(current_game)->data->average_time_to_score_save);
		second_step.append_child("average_time_to_score").text().set(c);

		second_step.append_child("total_actions").text().set(games.At(current_game)->data->total_actions);
		second_step.append_child("total_scoring").text().set(games.At(current_game)->data->total_scoring);
		second_step.append_child("total_picked_blocks").text().set(games.At(current_game)->data->picked_blocks);
		second_step.append_child("total_returned_blocks").text().set(games.At(current_game)->data->returned_blocks);
		second_step.append_child("total_placed_blocks").text().set(games.At(current_game)->data->placed_blocks);

		//Extra tracking
		pugi::xml_node score_tracking = game.append_child("score_tracking");
		sprintf_s(c, sizeof(c), "%.2f", games.At(current_game)->data->average_time_to_score_save);
		score_tracking.append_attribute("average_times_between_scores").set_value(c);

		pugi::xml_node n;
		pugi::xml_node a;

		// can use same index, the three lists always Add at same point
		for (int i = 0; i < games.At(current_game)->data->average_list_place.count(); i++)
		{
			sprintf_s(c, sizeof(c), "score_%d", int(games.At(current_game)->data->average_list_score.At(i)->data.x));
			n = score_tracking.append_child(c);
			sprintf_s(c, sizeof(c), "%.2f", games.At(current_game)->data->average_list_score.At(i)->data.y);
			n.append_attribute("time_to_score").set_value(c);

			// placing tracking
			a = n.append_child("time_to_place");
			sprintf_s(c, sizeof(c), "%.2f", games.At(current_game)->data->average_list_place.At(i)->data.x);
			a.append_attribute("placed").set_value(c);
			sprintf_s(c, sizeof(c), "%.2f", games.At(current_game)->data->average_list_place.At(i)->data.y);
			a.text().set(c);

			// returning tracking
			a = n.append_child("time_to_return");
			sprintf_s(c, sizeof(c), "%.2f", games.At(current_game)->data->average_list_return.At(i)->data.x);
			a.append_attribute("returned").set_value(c);
			sprintf_s(c, sizeof(c), "%.2f", games.At(current_game)->data->average_list_return.At(i)->data.y);
			a.text().set(c);

		}
	}


	return true;
}

void j1Data::Scored()
{
	games.At(current_game)->data->total_scoring++;
	float stopped_at = games.At(current_game)->data->time_to_score.Stop();
	games.At(current_game)->data->average_time_to_score += stopped_at;
	games.At(current_game)->data->average_time_to_score_save = games.At(current_game)->data->average_time_to_score / games.At(current_game)->data->total_scoring;

	games.At(current_game)->data->average_list_place.add(fPoint(games.At(current_game)->data->placed_blocks, games.At(current_game)->data->average_time_to_place_save));
	games.At(current_game)->data->average_list_return.add(fPoint(games.At(current_game)->data->returned_blocks, games.At(current_game)->data->average_time_to_return_save));
	games.At(current_game)->data->average_list_score.add(fPoint(games.At(current_game)->data->total_scoring, stopped_at));

	games.At(current_game)->data->time_to_score.Start();
}

void j1Data::Picked()
{
	games.At(current_game)->data->total_actions++;
	games.At(current_game)->data->picked_blocks++;
	games.At(current_game)->data->time_picking_placing.Start();
	games.At(current_game)->data->time_picking_returning.Start();
	
}

void j1Data::Returned()
{
	if (!b_returned) {

		games.At(current_game)->data->total_actions++;
		games.At(current_game)->data->returned_blocks++;
		games.At(current_game)->data->average_time_to_return += games.At(current_game)->data->time_picking_returning.Stop();
		games.At(current_game)->data->average_time_to_return_save = games.At(current_game)->data->average_time_to_return / games.At(current_game)->data->returned_blocks;
		b_returned = true;
	}
}

void j1Data::Placed()
{
	games.At(current_game)->data->total_actions++;
	games.At(current_game)->data->placed_blocks++;
	games.At(current_game)->data->average_time_to_place += games.At(current_game)->data->time_picking_placing.Stop();
	games.At(current_game)->data->average_time_to_place_save = games.At(current_game)->data->average_time_to_place / games.At(current_game)->data->placed_blocks;
}

void j1Data::StartGame()
{
	clicks_to_play_button_save = total_number_clicks;
	time_find_play_button_save = total_played_time.ReadSec();
	games.At(current_game)->data->time_to_score.Start();
}

void j1Data::Restart() 
{
	games.At(current_game)->data->current_game_time.Stop();
	current_game++;
	games.add(new GameData());
	games.At(current_game)->data->current_game_time.Start();
}
