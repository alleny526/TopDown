// Copyright Hanfei Yang - alleny@usc.edu

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "FireComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFireComponentOnFire);

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TOPDOWN_API UFireComponent : public USphereComponent
{
	GENERATED_BODY()

public:
	UFireComponent();

	UFUNCTION()
	void HandleOnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(BlueprintAssignable)
	FFireComponentOnFire OnFire;

	void SetOnFireStat(bool bStatus);
	bool GetOnFireStat();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsOnFire = false;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> FireSoundBase;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystem> FireParticleSystem;

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

	void TriggerEffects();
	
};
