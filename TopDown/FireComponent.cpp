// Copyright Hanfei Yang - alleny@usc.edu


#include "FireComponent.h"
#include <Kismet/GameplayStatics.h>

UFireComponent::UFireComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UFireComponent::HandleOnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (UFireComponent* OtherFireComponent = OtherActor->FindComponentByClass<UFireComponent>())
	{
		if (bIsOnFire)
		{
			OtherFireComponent->SetOnFireStat(true);
			TriggerEffects();
		}
	}
}

void UFireComponent::SetOnFireStat(bool bStatus)
{
	bIsOnFire = bStatus;
}

bool UFireComponent::GetOnFireStat()
{
	return bIsOnFire;
}

void UFireComponent::BeginPlay()
{
	Super::BeginPlay();

	OnComponentBeginOverlap.AddDynamic(this, &UFireComponent::HandleOnBeginOverlap);

	if (bIsOnFire)
	{
		TriggerEffects();
	}
}

void UFireComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	OnComponentBeginOverlap.RemoveAll(this);
}

void UFireComponent::TriggerEffects()
{
	UGameplayStatics::SpawnSoundAttached(FireSoundBase, this);
	UGameplayStatics::SpawnEmitterAttached(FireParticleSystem, this);

	OnFire.Broadcast();
}
