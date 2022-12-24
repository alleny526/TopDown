// Copyright Hanfei Yang - alleny@usc.edu


#include "PuzzleComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UPuzzleComponent::UPuzzleComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UPuzzleComponent::DebugDrawOutput()
{
	#if ENABLE_DRAW_DEBUG
		if (AActor* OutActor = OutputActor.Get())
		{
			const FColor DrawColor = bIsOutputActive ? FColor::Green : FColor::Black;
			DrawDebugDirectionalArrow(GetWorld(), GetOwner()->GetActorLocation(), OutActor->GetActorLocation(), 150.0f, DrawColor);
			if (IsRegistered())
			{
				DrawDebugString(GetWorld(), GetOwner()->GetActorLocation(), GetOwner()->GetName(), nullptr, FColor::White, 1.0f);
			}
		}
	#endif
}


// Called when the game starts
void UPuzzleComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UPuzzleComponent::SetOutputActive(bool Active)
{
	if (Active == bIsOutputActive)
	{
		return;
	}
	else
	{
		bIsOutputActive = Active;
		if (AActor* OutActor = OutputActor.Get())
		{	
			if (UPuzzleComponent* PuzzleComp = OutActor->FindComponentByClass<UPuzzleComponent>())
			{
				PuzzleComp->NativeReceiveInput(GetOwner(), Active);
			}
		}
	}
}

void UPuzzleComponent::NativeReceiveInput(AActor* InputActor, bool bIncomingValue)
{
	OnReceiveInput.Broadcast(InputActor, bIncomingValue);
}

void UPuzzleComponent::PuzzleReset()
{
	SetOutputActive(false);

	OnPuzzleReset.Broadcast();
}

void UPuzzleComponent::ChangeOutputActor(AActor* NewOutputActor)
{
	OutputActor = NewOutputActor;
	if (OutputActor != nullptr && bIsOutputActive)
	{
		if (AActor* OutActor = OutputActor.Get())
		{
			if (UPuzzleComponent* PuzzleComp = OutActor->FindComponentByClass<UPuzzleComponent>())
			{
				PuzzleComp->NativeReceiveInput(GetOwner(), bIsOutputActive);
			}
		}
	}
}
