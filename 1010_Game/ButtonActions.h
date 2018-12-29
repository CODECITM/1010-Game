#pragma once
#include "j1App.h"
#include "j1FadeScene.h"
#include "j1Data.h"

void StartGame()
{
	App->fade->FadeToBlack(App->fade->GetDelay(), fade_type::START_GAME);
	App->data->StartGame();
}

void GoToSettings()
{
	App->fade->FadeToBlack(App->fade->GetDelay(), fade_type::SETTINGS);
}

void GoToMenu()
{
	App->fade->FadeToBlack(App->fade->GetDelay(), fade_type::MAIN_MENU);
}

void GoToCredits()
{
	App->fade->FadeToBlack(App->fade->GetDelay(), fade_type::CREDITS);
}

void CloseGame()
{
	App->mustShutDown = true;
}

void Mute()
{
	if (App->audio->muted) {
		App->audio->SetMasterVolume(100);
	}
	else {
		App->audio->SetMasterVolume(0);
	}
}

void OpenWebpage()
{
	ShellExecuteA(NULL, "open", "https://github.com/CODECITM/1010_Game", NULL, NULL, SW_SHOWNORMAL);
}