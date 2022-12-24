// Copyright Hanfei Yang - alleny@usc.edu


#include "TopDownAICharacter.h"
#include "TopDownAIController.h"

ATopDownAICharacter::ATopDownAICharacter()
{
	AIControllerClass = ATopDownAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}
