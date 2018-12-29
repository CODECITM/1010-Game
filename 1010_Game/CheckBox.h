#ifndef __CHECK_BOX__
#define __CHECK_BOX__

#include "Button.h"

struct SDL_Texture;

template <class Ret, class... Args>
class CheckBox : public Button<Ret, Args...>
{
public:
	//Constructor
	template<class Ret, class... Args> CheckBox(Ret(*action)(Args...), bool* value, fPoint center, SDL_Rect spriteList[3], SDL_Texture* tex, bool dynamic = false, UIElement* parent = NULL, p2List<UIElement*>* children = NULL)
		: Button<Ret, Args...>(action, ui_type::BUTTON_CHECK, center, spriteList[0], tex, dynamic, parent, children), active(value)
	{
		stateSprites = new SDL_Rect[3];

		for (int i = 0; i < 3; i++) {
			stateSprites[i] = spriteList[i];
		}

		if (*active) {
			*sprite = stateSprites[1];
		}
		else {
			*sprite = stateSprites[2];
		}
	};

	virtual ~CheckBox()
	{
		RELEASE(sprite);
		RELEASE_ARRAY(stateSprites);
	}

	//Button action calling
	Ret operator() (Args&... args) const
	{
		return (action)(args...);
	}

	Ret DoAction(Args&... args) const {
		return (action)(args...);
	}

	// Called each frame (framerate dependant)
	virtual bool UpdateTick(float dt)
	{
		bool ret = true;

		if (status != button_state::DISABLED) {
			CheckCurrentState();
			ButtonStateEffects();
		}

		return ret;
	}

protected:
	virtual button_state CheckCurrentState()
	{
		switch (status) {
		case button_state::IDLE:
			if (MouseOnImage() == true) {
				OnHover();
				status = button_state::HOVERING;
			}
			break;
		case button_state::HOVERING:
			if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) {
				OnPress();
				status = button_state::PRESSING;
			}
			else if (MouseOnImage() == false) {
				OnIdle();
				status = button_state::IDLE;
			}
			break;
		case button_state::PRESSING:
			if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP || MouseOnImage() == false) {
				OnIdle();
				status = button_state::IDLE;
			}
			break;
		}

		return status;
	}

	virtual button_state ButtonStateEffects()
	{
		switch (status) {
		case button_state::IDLE:
			WhileIdle();
			break;
		case button_state::HOVERING:
			WhileHover();
			break;
		case button_state::PRESSING:
			WhilePress();
			break;
		}

		if (*active) {
			*sprite = stateSprites[1];
		}
		else {
			*sprite = stateSprites[2];
		}

		return status;
	}

	virtual void OnIdle()
	{}

	virtual void OnHover()
	{}

	virtual void OnPress()
	{
		App->audio->PlayFx(App->audio->buttonPressSfx.id, 0);
		DoAction(Args...);
		*active = !*active;
	}

	virtual void WhileIdle() {}

	virtual void WhileHover() {}

	virtual void WhilePress() {}

	//Enable/Disable
	virtual void Enable()
	{
		status = button_state::IDLE;
	}

	virtual void Disable()
	{
		status = button_state::DISABLED;
		*sprite = stateSprites[0];
	}

private:
	bool* active;
	SDL_Rect* stateSprites = nullptr;	//Disabled, Idle, Hover, Pressed
};

#endif //__ACTION_BOX_H__