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

	average_time_to_place_save  = 0.0f;
	average_time_to_return_save = 0.0f;
	average_time_to_score_save  = 0.0f;

	b_returned = false;

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

	second_step.append_child("average_time_to_place").text().set(average_time_to_place_save);
	second_step.append_child("average_time_to_return").text().set(average_time_to_return_save);
	second_step.append_child("average_time_to_score").text().set(average_time_to_score_save);

	second_step.append_child("total_actions").text().set(total_actions);
	second_step.append_child("total_scoring").text().set(total_scoring);
	second_step.append_child("total_picked_blocks").text().set(picked_blocks);
	second_step.append_child("total_returned_blocks").text().set(returned_blocks);
	second_step.append_child("total_placed_blocks").text().set(placed_blocks);

	//Extra tracking
	pugi::xml_node score_tracking = node.append_child("score_tracking");
	score_tracking.append_attribute("average_times_between_scores").set_value(average_time_to_score_save);

	pugi::xml_node n;
	pugi::xml_node a;
	char c[32];

	// can use same index, the three lists always Add at same point
	for (int i = 0; i < average_list_place.count(); i++) 
	{
		sprintf_s(c,sizeof(c), "score_%d", int(average_list_score.At(i)->data.x));
		n = score_tracking.append_child(c);
		n.append_attribute("time_to_score").set_value(average_list_score.At(i)->data.y);

		// placing tracking
		a = n.append_child("time_to_place");
		a.append_attribute("placed").set_value(average_list_place.At(i)->data.x);
		a.text().set(average_list_place.At(i)->data.y);

		// returning tracking
		a = n.append_child("time_to_return");
		a.append_attribute("returned").set_value(average_list_return.At(i)->data.x);
		a.text().set(average_list_return.At(i)->data.y);

	}


	return true;
}

void j1Data::Scored()
{
	total_scoring++;
	float stopped_at = time_to_score.Stop();
	average_time_to_score += stopped_at;
	average_time_to_score_save = average_time_to_score / total_scoring;

	average_list_place.add(fPoint(placed_blocks,average_time_to_place_save));
	average_list_return.add(fPoint(returned_blocks, average_time_to_return_save));
	average_list_score.add(fPoint(total_scoring, stopped_at));
}

void j1Data::Picked()
{
	total_actions++;
	picked_blocks++;
	time_picking_placing.Start();
	time_picking_returning.Start();
	if (!time_to_score.IsRunning()) time_to_score.Start();
}

void j1Data::Returned()
{
	if (!b_returned) {

		total_actions++;
		returned_blocks++;
		average_time_to_return += time_picking_returning.Stop();
		average_time_to_return_save = average_time_to_return / returned_blocks;
		b_returned = true;
	}
}

void j1Data::Placed()
{
	total_actions++;
	placed_blocks++;
	average_time_to_place += time_picking_placing.Stop();
	average_time_to_place_save = average_time_to_place / placed_blocks;
}

void j1Data::StartGame()
{
	clicks_to_play_button_save = total_number_clicks;
	time_find_play_button_save = total_played_time.ReadSec();
}
