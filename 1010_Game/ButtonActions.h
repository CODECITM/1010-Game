#pragma once
#include "j1App.h"
#include "j1FadeScene.h"

void StartGame()
{
	App->fade->FadeToBlack(App->fade->GetDelay(), fade_type::START_GAME);
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

void OpenSettings()
{
	/*if (App->scene->settingsWindow != nullptr)
		App->scene->settingsWindow->Activate();*/
}

void CloseSettings()
{
	/*if (App->scene->settingsWindow != nullptr)
		App->scene->settingsWindow->Deactivate();*/
}

void OpenWebpage()
{
	ShellExecuteA(NULL, "open", "https://github.com/CODECITM/1010_Game", NULL, NULL, SW_SHOWNORMAL);
}