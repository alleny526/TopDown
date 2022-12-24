// Copyright Hanfei Yang - alleny@usc.edu


#include "InteractSubsystem.h"
#include "InteractComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TopDownCharacter.h"

void UInteractSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);


}

void UInteractSubsystem::Deinitialize()
{
	Super::Deinitialize();

	InteractComponents.Empty();
}

void UInteractSubsystem::AddComponent(UInteractComponent* Component)
{
	InteractComponents.Add(Component);
}

void UInteractSubsystem::RemoveComponent(UInteractComponent* Component)
{
	InteractComponents.Remove(Component);
}

void UInteractSubsystem::PerformInteract()
{
	if (UInteractComponent* Component = BestCandidate.Get())
	{
		if (Component->GetInteractableStatus())
		{
			Component->NativeInteract();
		}
	}
}

void UInteractSubsystem::Tick(float DeltaTime)
{
	BestCandidate = nullptr;

	if (ACharacter* Character = UGameplayStatics::GetPlayerCharacter(this, 0))
	{
		FVector PlayerPos = Character->GetActorLocation();
		float BestDistance = TNumericLimits<float>::Max();
		for (const auto& WeakPtr : InteractComponents)
		{
			if (UInteractComponent* Component = WeakPtr.Get())
			{
				if (Component->GetInteractableStatus())
				{
					float Distance = FVector::Distance(PlayerPos, Component->GetOwner()->GetActorLocation());
					Component->JudgeGlow(false);
					if (Distance < BestDistance && Distance <= 200)
					{
						FVector3d PlayerForwardNorm = Character->GetActorForwardVector();
						FVector3d ComponentPos = WeakPtr.Get()->GetOwner()->GetActorLocation();
						FVector3d PlayerToComponent = ComponentPos - PlayerPos;
						PlayerForwardNorm.Normalize();
						PlayerToComponent.Normalize();
						float DotProductRes = FVector::DotProduct(PlayerToComponent, PlayerForwardNorm);
						if (DotProductRes > 0.7f && DotProductRes < 1.3f)
						{
							BestDistance = Distance;
							BestCandidate = WeakPtr;
						}
					}
				}
			}
		}
		if (BestCandidate.Get())
		{
			BestCandidate.Get()->JudgeGlow(true);
		}
	}
}

TStatId UInteractSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UInteractSubsystem, STATGROUP_Tickables);
}

TWeakObjectPtr<class UInteractComponent> UInteractSubsystem::ReturnBestCandidate()
{
	return BestCandidate;
}

bool UInteractSubsystem::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}