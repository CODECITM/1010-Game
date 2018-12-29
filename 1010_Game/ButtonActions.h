#pragma once

#include "j1App.h"
#include "j1FadeScene.h"
#include "j1Data.h"
#include "j1Scene.h"

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

void CheckDifficulty(int difficulty)
{
	switch (difficulty) {
	case 0:
		App->scene->difficultyTxt->ChangeText("Easy", { 0, 200, 0, 255 }, NULL);
		break;
	case 1:
		App->scene->difficultyTxt->ChangeText("Medium", { 255, 150, 0, 255 }, NULL);
		break;
	case 2:
		App->scene->difficultyTxt->ChangeText("Hard", { 200, 0, 0, 255 }, NULL);
		break;
	}
}

void UpDifficulty()
{
	int* diffPtr = &App->scene->difficulty;

	if (++(*diffPtr) > 2) {
		*diffPtr = 0;
	}

	CheckDifficulty(*diffPtr);
}

void DownDifficulty()
{
	int* diffPtr = &App->scene->difficulty;

	if (--(*diffPtr) < 0) {
		*diffPtr = 2;
	}

	CheckDifficulty(*diffPtr);
}