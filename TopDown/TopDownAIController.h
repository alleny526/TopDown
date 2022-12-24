// Copyright Hanfei Yang - alleny@usc.edu

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TopDownAIController.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWN_API ATopDownAIController : public AAIController
{
	GENERATED_BODY()

public:
	virtual void OnPossess(APawn* InPawn) override;

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class ATopDownAICharacter> OwnerCharacter;
	
};
