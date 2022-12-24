// Copyright Hanfei Yang - alleny@usc.edu

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PuzzleComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPuzzleComponentOnReceiveInput, class AActor*, InputActor, bool, bValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPuzzleComponentOnPuzzleReset);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOPDOWN_API UPuzzleComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPuzzleComponent();

	void DebugDrawOutput();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	/**
	 * The function takes in a bool specifying a new value for bIsOutputActive
	 * @param Active - Boolean value to change bIsOutputActive
	 * @return None
	 */
	UFUNCTION(BlueprintCallable)
	virtual void SetOutputActive(bool Active);

	// The function receives the input boolean value from incoming actor
	virtual void NativeReceiveInput(AActor* InputActor, bool bIncomingValue);

	/**
	 * The function resets related puzzle
	 * @param None
	 * @return None
	 */
	UFUNCTION(BlueprintCallable)
	virtual void PuzzleReset();

	// This will be used to track whether or out the output is active
	UPROPERTY(Transient, BlueprintReadOnly)
	bool bIsOutputActive = false;

	// This will be used to track which actor this puzzle component should send the output to
	UPROPERTY(EditInstanceOnly)
	TSoftObjectPtr<AActor> OutputActor;

	UPROPERTY(BlueprintAssignable)
	FPuzzleComponentOnReceiveInput OnReceiveInput;

	UPROPERTY(BlueprintAssignable)
	FPuzzleComponentOnPuzzleReset OnPuzzleReset;

	UFUNCTION(BlueprintCallable)
	void ChangeOutputActor(AActor* NewOutputActor);
};
