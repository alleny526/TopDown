// Copyright Hanfei Yang - alleny@usc.edu

#pragma once

#include "CoreMinimal.h"
#include "TopDownCharacter.h"
#include "TopDownAICharacter.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWN_API ATopDownAICharacter : public ATopDownCharacter
{
	GENERATED_BODY()
	
public:
	ATopDownAICharacter();

	TArray<TSoftObjectPtr<AActor>> GetPatrolPath() const { return PatrolPath; }

protected:
	UPROPERTY(EditAnywhere, Category = AI)
	TObjectPtr<class UBlackboardData> BlackboardAsset;

	UPROPERTY(EditAnywhere, Category = AI)
	TObjectPtr<class UBehaviorTree> BTAsset;

	UPROPERTY(EditInstanceOnly, Category = AI)
	TArray<TSoftObjectPtr<AActor>> PatrolPath;

	friend class ATopDownAIController;
};
