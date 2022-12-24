// Copyright Hanfei Yang - alleny@usc.edu


#include "PuzzleRotate.h"
#include "Components/BillboardComponent.h"
#include "PuzzleComponent.h"

// Sets default values
APuzzleRotate::APuzzleRotate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<UBillboardComponent>("Root");

	PuzzleComponent = CreateDefaultSubobject<UPuzzleComponent>("Puzzle");
}

// Called when the game starts or when spawned
void APuzzleRotate::BeginPlay()
{
	Super::BeginPlay();
	
	PuzzleComponent->OnReceiveInput.AddDynamic(this, &APuzzleRotate::OnReceiveInput);

	GetWorldTimerManager().SetTimer(TimerHandle, this, &APuzzleRotate::OnRotateTimer, TimerDuration, true);

	for (auto& CurrentActor : InputActors)
	{
		CurrentActor->FindComponentByClass<UPuzzleComponent>()->ChangeOutputActor(nullptr);
	}
}

// Called every frame
void APuzzleRotate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APuzzleRotate::OnReceiveInput(AActor* InputActor, bool bValue)
{
	PuzzleComponent->SetOutputActive(bValue);
}

void APuzzleRotate::OnRotateTimer()
{
	if (InputActors.Num() > 0)
	{
		PuzzleComponent->PuzzleReset();

		for (auto& CurrentActor : InputActors)
		{
			CurrentActor->FindComponentByClass<UPuzzleComponent>()->ChangeOutputActor(nullptr);
		}

		InputActors[Tracker]->FindComponentByClass<UPuzzleComponent>()->ChangeOutputActor(this);

		Tracker++;
		Tracker = Tracker % 3;
	}
}

