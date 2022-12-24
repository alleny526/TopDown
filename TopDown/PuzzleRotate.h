// Copyright Hanfei Yang - alleny@usc.edu

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuzzleRotate.generated.h"

UCLASS()
class TOPDOWN_API APuzzleRotate : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APuzzleRotate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Our PuzzleComponent
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UPuzzleComponent> PuzzleComponent;

	// Duration of the timer
	UPROPERTY(EditAnywhere)
	float TimerDuration = 1.0f;

	// Handle to manage the timer
	FTimerHandle TimerHandle;

	int Tracker = 0;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnReceiveInput(class AActor* InputActor, bool bValue);

	// The input actors with puzzle components which we want to rotate between
	UPROPERTY(EditInstanceOnly)
	TArray<TSoftObjectPtr<class AActor>> InputActors;

	void OnRotateTimer();

};
