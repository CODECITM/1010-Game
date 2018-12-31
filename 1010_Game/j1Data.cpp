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
	current_game[0] = 0;
	current_game[1] = 0;
	current_type = 0;

	clicks_to_play_button_save = 0;
	time_find_play_button_save = 0;

	game_type[0] = &gamesA;
	game_type[0]->add(new GameData());
	game_type[0]->At(current_game[current_type])->data->current_game_time.Stop();

	game_type[1] = &gamesB;
	game_type[1]->add(new GameData());
	game_type[1]->At(current_game[current_type])->data->current_game_time.Stop();

	b_returned = false;
	one_minute = false;
	initialized = false;
	first_entering = true;
}


j1Data::~j1Data()
{
}

bool j1Data::Start()
{
	total_played_time.Start();
	//game_type->At(current_game)->data->current_game_time.Start();
	autosave.Start();

	return true;
}

bool j1Data::Update(float dt)
{
	b_returned = false;
	one_minute = total_played_time.ReadSec() >= 60.0f;

	if (one_minute && !initialized)
	{
		game_type[current_type]->At(current_game[current_type])->data->current_game_time.Start();
		initialized = true;
	}


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
	gamesA.clear();
	gamesB.clear();

	return true;
}


bool j1Data::Save(pugi::xml_node & node) const
{
	char c[32];
	char g[32];

	sprintf_s(c, sizeof(c),"%.2f" , total_played_time.ReadSec());
	node.append_child("total_played_time").text().set(c);
	node.append_child("total_number_of_clicks").text().set(total_number_clicks);
	
	//1st Step
	pugi::xml_node first_step = node.append_child("first_step");

	first_step.append_child("clicks_to_find_play_button").text().set(clicks_to_play_button_save);
	sprintf_s(c, sizeof(c), "%.2f", time_find_play_button_save);
	first_step.append_child("time_to_find_play_button").text().set(c);

	for (int i = 0; i < 2; i++) 
	{
		sprintf_s(g, sizeof(g), "GAME_%c", (i==0)?'A':'B');
		//pugi::xml_node gameType = node.append_child(g);

		for (int k = 0; k < game_type[i]->count(); k++) {

			//sprintf_s(c, sizeof(c), "%s%d",g, k);
			pugi::xml_node game = node.append_child(g);

			sprintf_s(c, sizeof(c), "%.2f", game_type[i]->At(k)->data->current_game_time.ReadSec());
			game.append_child("game_time").text().set(c);

			//2nd Step
			pugi::xml_node second_step = game.append_child("second_step");

			sprintf_s(c, sizeof(c), "%.2f", game_type[i]->At(k)->data->average_time_to_place_save);
			second_step.append_child("average_time_to_place").text().set(c);
			sprintf_s(c, sizeof(c), "%.2f", game_type[i]->At(k)->data->average_time_to_return_save);
			second_step.append_child("average_time_to_return").text().set(c);
			sprintf_s(c, sizeof(c), "%.2f", game_type[i]->At(k)->data->average_time_to_score_save);
			second_step.append_child("average_time_to_score").text().set(c);

			second_step.append_child("total_actions").text().set(game_type[i]->At(k)->data->total_actions);
			second_step.append_child("total_scoring").text().set(game_type[i]->At(k)->data->total_scoring);
			second_step.append_child("total_picked_blocks").text().set(game_type[i]->At(k)->data->picked_blocks);
			second_step.append_child("total_returned_blocks").text().set(game_type[i]->At(k)->data->returned_blocks);
			second_step.append_child("total_placed_blocks").text().set(game_type[i]->At(k)->data->placed_blocks);

			//Extra tracking
			//pugi::xml_node score_tracking = game.append_child("score_tracking");
			//sprintf_s(c, sizeof(c), "%.2f", game_type[i]->At(k)->data->average_time_to_score_save);
			//score_tracking.append_attribute("average_times");// .set_value(c);

			pugi::xml_node n;
			pugi::xml_node a;

			// can use same index, the three lists always Add at same point
			for (int j = 0; j < game_type[i]->At(k)->data->average_list_place.count(); j++)
			{
				//sprintf_s(c, sizeof(c), "score_%d", int(game_type[i]->At(k)->data->average_list_score.At(j)->data.x));
				n = game.append_child("score");
				sprintf_s(c, sizeof(c), "%.2f", game_type[i]->At(k)->data->average_list_score.At(j)->data.y);
				n.append_attribute("time_to_score").set_value(c);

				// placing tracking
				a = n.append_child("time_to_place");
				sprintf_s(c, sizeof(c), "%.2f", game_type[i]->At(k)->data->average_list_place.At(j)->data.x);
				a.append_attribute("placed").set_value(c);
				sprintf_s(c, sizeof(c), "%.2f", game_type[i]->At(k)->data->average_list_place.At(j)->data.y);
				a.text().set(c);

				// returning tracking
				a = n.append_child("time_to_return");
				sprintf_s(c, sizeof(c), "%.2f", game_type[i]->At(k)->data->average_list_return.At(j)->data.x);
				a.append_attribute("returned").set_value(c);
				sprintf_s(c, sizeof(c), "%.2f", game_type[i]->At(k)->data->average_list_return.At(j)->data.y);
				a.text().set(c);

			}
		}
	}

	return true;
}

void j1Data::Scored()
{
	if (!one_minute) return;

	game_type[current_type]->At(current_game[current_type])->data->total_scoring++;
	float stopped_at = game_type[current_type]->At(current_game[current_type])->data->time_to_score.Stop();
	game_type[current_type]->At(current_game[current_type])->data->average_time_to_score += stopped_at;
	game_type[current_type]->At(current_game[current_type])->data->average_time_to_score_save = game_type[current_type]->At(current_game[current_type])->data->average_time_to_score / game_type[current_type]->At(current_game[current_type])->data->total_scoring;

	game_type[current_type]->At(current_game[current_type])->data->average_list_place.add(fPoint(game_type[current_type]->At(current_game[current_type])->data->placed_blocks, game_type[current_type]->At(current_game[current_type])->data->average_time_to_place_save));
	game_type[current_type]->At(current_game[current_type])->data->average_list_return.add(fPoint(game_type[current_type]->At(current_game[current_type])->data->returned_blocks, game_type[current_type]->At(current_game[current_type])->data->average_time_to_return_save));
	game_type[current_type]->At(current_game[current_type])->data->average_list_score.add(fPoint(game_type[current_type]->At(current_game[current_type])->data->total_scoring, stopped_at));

	game_type[current_type]->At(current_game[current_type])->data->time_to_score.Start();
}

void j1Data::Picked()
{
	if (!one_minute) return;
	game_type[current_type]->At(current_game[current_type])->data->total_actions++;
	game_type[current_type]->At(current_game[current_type])->data->picked_blocks++;
	game_type[current_type]->At(current_game[current_type])->data->time_picking_placing.Start();
	game_type[current_type]->At(current_game[current_type])->data->time_picking_returning.Start();
	
}

void j1Data::Returned()
{
	if (one_minute && !b_returned) {

		game_type[current_type]->At(current_game[current_type])->data->total_actions++;
		game_type[current_type]->At(current_game[current_type])->data->returned_blocks++;
		game_type[current_type]->At(current_game[current_type])->data->average_time_to_return += game_type[current_type]->At(current_game[current_type])->data->time_picking_returning.Stop();
		game_type[current_type]->At(current_game[current_type])->data->average_time_to_return_save = game_type[current_type]->At(current_game[current_type])->data->average_time_to_return / game_type[current_type]->At(current_game[current_type])->data->returned_blocks;
		b_returned = true;
	}
}

void j1Data::Placed()
{
	if (!one_minute) return;
	game_type[current_type]->At(current_game[current_type])->data->total_actions++;
	game_type[current_type]->At(current_game[current_type])->data->placed_blocks++;
	game_type[current_type]->At(current_game[current_type])->data->average_time_to_place += game_type[current_type]->At(current_game[current_type])->data->time_picking_placing.Stop();
	game_type[current_type]->At(current_game[current_type])->data->average_time_to_place_save = game_type[current_type]->At(current_game[current_type])->data->average_time_to_place / game_type[current_type]->At(current_game[current_type])->data->placed_blocks;
}

void j1Data::StartGame()
{
	if (first_entering) {
		clicks_to_play_button_save = total_number_clicks;
		time_find_play_button_save = total_played_time.ReadSec();
		game_type[current_type]->At(current_game[current_type])->data->current_game_time.Start();

		first_entering = false;
	}
	else {
		if (!one_minute) return;
	}
}

void j1Data::Restart() 
{
	if (!one_minute) return;
	game_type[current_type]->At(current_game[current_type])->data->current_game_time.Stop();

	current_game[current_type]++;
	game_type[current_type]->add(new GameData());
}


void j1Data::ChangeGameType() 
{
	if (!one_minute) return;
	current_type = (current_type == 0) ? 1 : 0;

}

void j1Data::GoToMenu() {

	if (!one_minute) return;
	game_type[current_type]->At(current_game[current_type])->data->current_game_time.Stop();

	current_game[current_type]++;
	game_type[current_type]->add(new GameData());
	game_type[current_type]->At(current_game[current_type])->data->current_game_time.Stop();
}

void j1Data::StartTimer()
{
	game_type[current_type]->At(current_game[current_type])->data->current_game_time.Start();
}