#include <SDL.h>

#include <backends/imgui_impl_sdl.h>

#include "game/inputs_manager.hpp"

using namespace Game;

#pragma region STATIC DEFINITION
bool Inputs::quit = false;
bool Inputs::allowListeningInputs = false;

std::unordered_map<SDL_KeyCode, EButton> Inputs::keys {};

std::unordered_map<std::string, ButtonAction> Inputs::buttonActions {};
std::unordered_map<std::string, AxisAction> Inputs::axisActions {};
std::list<EButtonState*> Inputs::keyPressed {};
MouseAction Inputs::mouse {};

EnumArray<EButton, EButtonState, static_cast<size_t>(EButton::KEY_COUNT)> Inputs::buttonStates {};
#pragma endregion

void Inputs::KeyPressed(EButtonState& o_state)
{
    if (o_state == EButtonState::RELEASED)
    {
        o_state = EButtonState::PRESSED;
        keyPressed.emplace_back(&o_state);
    }
}

void Inputs::CheckKeyPressed()
{
    auto it = keyPressed.begin();
    while(it != keyPressed.end())
    {
        if (**it == EButtonState::PRESSED) **it = EButtonState::DOWN;
        it = keyPressed.erase(it);
    }
}


EButtonState* Inputs::GetButtonState(Game::EButton i_button)
{
    return &buttonStates[i_button];
}

void Game::Inputs::SetButtonAction(const std::string& i_name, const std::initializer_list<EButton>& i_inputs, bool i_resetKeys)
{
    if(buttonActions.find(i_name) == buttonActions.end())
    {
        buttonActions[i_name] = ButtonAction{};
        Log::Info(std::string("New button action map created : '") + i_name + std::string("'"));
    }

    ButtonAction& action = buttonActions[i_name];

    if (i_resetKeys) action.buttons.clear();

    for (EButton key : i_inputs)
        action.buttons.emplace_back(GetButtonState(key));
}

void Inputs::SetAxisAction(const std::string& i_name, const std::initializer_list<EButton>& i_positiveInputs, const std::initializer_list<EButton>& i_negativeInputs, bool i_resetPositive, bool i_resetNegative)
{
    if(axisActions.find(i_name) == axisActions.end())
    {
        axisActions[i_name] = AxisAction{};
        Log::Info(std::string("New axis action map created : '") + i_name + std::string("'"));
    }

    AxisAction& action = axisActions[i_name];

    if (i_resetPositive) action.positiveButtons.clear();

    for (EButton key : i_positiveInputs)
        action.positiveButtons.emplace_back(GetButtonState(key));

    if (i_resetNegative) action.negativeButtons.clear();

    for (EButton key : i_negativeInputs)
        action.negativeButtons.emplace_back(GetButtonState(key));
}

Inputs::Inputs()
{
    keys[SDLK_a] = EButton::A;
    keys[SDLK_b] = EButton::B;
    keys[SDLK_c] = EButton::C;
    keys[SDLK_d] = EButton::D;
    keys[SDLK_e] = EButton::E;
    keys[SDLK_f] = EButton::F;
    keys[SDLK_g] = EButton::G;
    keys[SDLK_h] = EButton::H;
    keys[SDLK_i] = EButton::I;
    keys[SDLK_j] = EButton::J;
    keys[SDLK_k] = EButton::K;
    keys[SDLK_l] = EButton::L;
    keys[SDLK_m] = EButton::M;
    keys[SDLK_n] = EButton::N;
    keys[SDLK_o] = EButton::O;
    keys[SDLK_p] = EButton::P;
    keys[SDLK_q] = EButton::Q;
    keys[SDLK_r] = EButton::R;
    keys[SDLK_s] = EButton::S;
    keys[SDLK_t] = EButton::T;
    keys[SDLK_u] = EButton::U;
    keys[SDLK_v] = EButton::V;
    keys[SDLK_w] = EButton::W;
    keys[SDLK_x] = EButton::X;
    keys[SDLK_y] = EButton::Y;
    keys[SDLK_z] = EButton::Z;
    keys[SDLK_LSHIFT] = EButton::LEFT_SHIFT;
    keys[SDLK_LCTRL] = EButton::LEFT_CTRL;
    keys[SDLK_SPACE] = EButton::SPACE;
    keys[SDLK_DOWN] = EButton::ARROW_DOWN;
    keys[SDLK_UP] = EButton::ARROW_UP;
    keys[SDLK_RIGHT] = EButton::ARROW_RIGHT;
    keys[SDLK_LEFT] = EButton::ARROW_LEFT;
    keys[SDLK_ESCAPE] = EButton::ESCAPE;
}

void Inputs::PollEvent(const InputsEvent& editorEvent)
{
    SDL_Event event;
    CheckKeyPressed();

    /// MOUSE UDPATE
    int x, y, relX, relY;
    SDL_GetMouseState(&x, &y);
    SDL_GetRelativeMouseState(&relX, &relY);
    mouse.position = Vector2(x, y);
    mouse.deltaMotion = Vector2(relX, relY);

    /// INPTUS EVENT
    while (SDL_PollEvent(&event))
    {
        if (editorEvent) editorEvent(&event);
        EButton key;
        switch(event.type)
        {
            case SDL_QUIT:
                Inputs::quit = true;
                break;
            case SDL_KEYDOWN:
                key = keys[static_cast<SDL_KeyCode>(event.key.keysym.sym)];
                KeyPressed(buttonStates[key]);
                break;
            case SDL_KEYUP:
                key = keys[static_cast<SDL_KeyCode>(event.key.keysym.sym)];
                buttonStates[key] = EButtonState::RELEASED;
                break;
            case SDL_MOUSEMOTION:
                //Log::Info(std::to_string(mouse.deltaMotion.x) + ", " + std::to_string(mouse.deltaMotion.y));
                break;
            case SDL_MOUSEBUTTONDOWN:
                switch (event.button.button)
                {
                    case SDL_BUTTON_LEFT:
                        KeyPressed(buttonStates[EButton::MOUSE_LEFT]); break;
                    case SDL_BUTTON_MIDDLE:
                        KeyPressed(buttonStates[EButton::MOUSE_MIDDLE]); break;
                    case SDL_BUTTON_RIGHT:
                        KeyPressed(buttonStates[EButton::MOUSE_RIGHT]); break;
                    default: break;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                switch (event.button.button)
                {
                    case SDL_BUTTON_LEFT: buttonStates[EButton::MOUSE_LEFT] = EButtonState::RELEASED; break;
                    case SDL_BUTTON_MIDDLE: buttonStates[EButton::MOUSE_MIDDLE] = EButtonState::RELEASED; break;
                    case SDL_BUTTON_RIGHT:  buttonStates[EButton::MOUSE_RIGHT] = EButtonState::RELEASED; break;
                    default: break;
                }
                break;
            default:
                break;
        }
    }
}

#pragma region Action Getter
EButtonState Inputs::GetButton(const std::string& i_name)
{
    if (CheckButtonAction(i_name))
        return buttonActions[i_name].state;
    else
        return EButtonState::RELEASED;
}

bool Inputs::IsPressed(const std::string& i_name)
{
    if (CheckButtonAction(i_name))
        return buttonActions[i_name].state == EButtonState::PRESSED;
    else
        return false;
}

bool Inputs::IsPressed(EButton i_button)
{
    return ApplyListeningCheck(buttonStates[i_button] == EButtonState::PRESSED);
}

bool Inputs::IsDown(const std::string& i_name)
{
    if (CheckButtonAction(i_name))
        return buttonActions[i_name].state == EButtonState::DOWN;
    else
        return false;
}

bool Inputs::IsDown(EButton i_button)
{
    return ApplyListeningCheck(buttonStates[i_button] == EButtonState::DOWN);
}

bool Inputs::IsReleased(const std::string& i_name)
{
    if (CheckButtonAction(i_name))
        return buttonActions[i_name].state == EButtonState::RELEASED;
    else
        return false;
}

bool Inputs::IsReleased(EButton i_button)
{
    return ApplyListeningCheck(buttonStates[i_button] == EButtonState::RELEASED);
}

float Inputs::GetAxis(const std::string& i_name)
{
    if (!CheckAxisAction(i_name)) return 0.f;

    return axisActions[i_name].value;
}

Vector2 Inputs::GetMousePosition()
{
    if (ApplyListeningCheck())
        return mouse.position;
    else
        return {};
}

Vector2 Inputs::GetMouseDelta()
{
    if (ApplyListeningCheck())
        return mouse.deltaMotion;
    else
        return {};
}

#pragma endregion

bool Game::Inputs::CheckButtonAction(const std::string& i_name)
{
    // FIND
    if (buttonActions.find(i_name) == buttonActions.end())
    {
        Log::Warning(std::string("The button action '") + i_name + std::string("' does not exist."));
        return false;
    }

    ButtonAction& action = buttonActions[i_name];
    action.state = EButtonState::RELEASED;
    for(EButtonState* state : action.buttons)
    {
        if (*state == EButtonState::DOWN)
        {
            action.state = EButtonState::DOWN;
            return true;
        }
        else if (*state == EButtonState::PRESSED)
            action.state = EButtonState::PRESSED;
    }

    return ApplyListeningCheck(true);
}

bool Game::Inputs::CheckAxisAction(const std::string& i_name)
{
    // FIND
    if (axisActions.find(i_name) == axisActions.end())
    {
        Log::Warning(std::string("The axis action '") + i_name + std::string("' does not exist."));
        return false;
    }

    AxisAction& action = axisActions[i_name];
    action.value = 0.f;
    for (const EButtonState* state : action.positiveButtons)
    {
        if (*state != EButtonState::RELEASED)
        {
            action.value += 1.f;
            break;
        }
    }
    for (const EButtonState* state : action.negativeButtons)
    {
        if (*state != EButtonState::RELEASED)
        {
            action.value -= 1.f;
            break;
        }
    }
    return ApplyListeningCheck(true);
}

void Inputs::SetInputsListening(bool i_listening) noexcept
{
    allowListeningInputs = i_listening;
}

bool Inputs::ApplyListeningCheck(bool i_currentValue)
{
    return allowListeningInputs && i_currentValue;
}
