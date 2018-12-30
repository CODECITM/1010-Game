#ifndef __j1AUDIO_H__
#define __j1AUDIO_H__

#include "j1Module.h"
#include "SDL_mixer\include\SDL_mixer.h"
#include "p2List.h"

#define DEFAULT_MUSIC_FADE_TIME 2.0f	//IMPROVE: Put on xml

struct _Mix_Music;
struct Mix_Chunk;

enum sfx_id_list {	// @Carles: List of SFX id numbers
	SFX_BUTTON_HOVER = 1,
	SFX_BUTTON_PRESS,
	SFX_PIECE_DROPPED,
	SFX_PIECE_PICKED,
	SFX_BRICK_DESTROYED,
	SFX_MAX
};

struct sfx_file {	// @Carles: The filename of the sfx + his enum id
	p2SString filename;
	uint id;
};

class j1Audio : public j1Module
{
public:

	j1Audio();

	// Destructor
	virtual ~j1Audio();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Save and Load
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	// Play
	bool PlayMusic(const char* path, float fade_time = 0.0f);
	bool PlayFx(unsigned int fx, int repeat = 0);

	// Stop
	void StopMusic();
	void StopFx();

	// Load in memory
	unsigned int LoadFx(const char* path);
	void UnloadFx();

public:	// @Carles
	int SetMasterVolume(ushort newVolume);
	int SetMusicVolume(ushort newVolume);
	int SetSfxVolume(ushort newVolume);

	int MusicToMixer() const;
	int SfxToMixer() const;

public:	// @Carles
	p2SString musicMainMenu;

	sfx_file buttonHoverSfx;
	sfx_file buttonPressSfx;
	sfx_file PieceDroppedSfx;
	sfx_file PiecePickedSfx;
	sfx_file BrickDestroyedSfx;

	ushort masterVolume;
	ushort musicVolume;
	ushort sfxVolume;

	bool muted = false;
	bool muteMusic = true;
	bool muteSfx = false;

private:

	void LoadAllMusic(pugi::xml_node&);		// @Carles
	void LoadAllSfx(pugi::xml_node&);		// @Carles

	_Mix_Music*			music;
	p2List<Mix_Chunk*>	fx;

	p2SString musicFolder;	// @Carles
	p2SString sfxFolder;	// @Carles
};

#endif // __j1AUDIO_H__