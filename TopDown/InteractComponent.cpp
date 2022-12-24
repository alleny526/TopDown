// Copyright Hanfei Yang - alleny@usc.edu


#include "InteractComponent.h"
#include "InteractSubsystem.h"

// Sets default values for this component's properties
UInteractComponent::UInteractComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (UInteractSubsystem* Subsystem = GetWorld()->GetSubsystem<UInteractSubsystem>())
	{
		Subsystem->AddComponent(this);
	}
}

void UInteractComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (UInteractSubsystem* Subsystem = GetWorld()->GetSubsystem<UInteractSubsystem>())
	{
		Subsystem->RemoveComponent(this);
	}
}

void UInteractComponent::NativeInteract()
{
	OnInteract.Broadcast();
}

void UInteractComponent::JudgeGlow(bool Status)
{
	if (Status && bIsInteractable)
	{
		GetOwner()->FindComponentByClass<UStaticMeshComponent>()->SetVectorParameterValueOnMaterials(InteractGlowParamName, FVector(WhiteColor.R, WhiteColor.G, WhiteColor.B));
	}
	else
	{
		GetOwner()->FindComponentByClass<UStaticMeshComponent>()->SetVectorParameterValueOnMaterials(InteractGlowParamName, FVector(DefaultColor.R, DefaultColor.G, DefaultColor
			.B));
	}
	
}

// Called every frame
void UInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{	
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInteractComponent::OnActivateTorch()
{
	bIsInteractable = false;
	GetOwner()->FindComponentByClass<UStaticMeshComponent>()->SetVectorParameterValueOnMaterials(InteractGlowParamName, FVector(DefaultColor.R, DefaultColor.G, DefaultColor.B));
}

void UInteractComponent::OnTorchTimePassed()
{
	bIsInteractable = true;
}

