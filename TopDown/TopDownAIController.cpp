// Copyright Hanfei Yang - alleny@usc.edu


#include "TopDownAIController.h"
#include "TopDownAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

void ATopDownAIController::OnPossess(APawn* InPawn)
{
	OwnerCharacter = Cast<ATopDownAICharacter>(InPawn);
	
	bool bInitializedBB = false;
	if (OwnerCharacter && OwnerCharacter->BlackboardAsset && OwnerCharacter->BTAsset)
	{
		UBlackboardComponent* BBComponent;
		if (UseBlackboard(OwnerCharacter->BlackboardAsset, BBComponent))
		{
			BBComponent->SetValueAsInt("PatrolIndex", -1);

			RunBehaviorTree(OwnerCharacter->BTAsset);
			bInitializedBB = true;
		}
	}

	if (!bInitializedBB)
	{
		UE_LOG(LogTemp, Warning, TEXT("ATopDownAIController can't initialize BT for %s"), *InPawn->GetName());
	}

	// Call OnPossess at the end cause it sets up the Blackboard
	Super::OnPossess(InPawn);
}
