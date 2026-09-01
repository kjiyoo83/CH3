#include "CH3_GameMode.h"
#include "CH3_Character.h"
#include "CH3_PlayerController.h"
#include "CH3_GameState.h"


ACH3_GameMode::ACH3_GameMode()
{
	DefaultPawnClass = ACH3_Character::StaticClass();
	PlayerControllerClass = ACH3_PlayerController::StaticClass();
	GameStateClass = ACH3_GameState::StaticClass();
}
