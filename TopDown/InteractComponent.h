// Copyright Hanfei Yang - alleny@usc.edu

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInteractDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOPDOWN_API UInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	bool bIsBestCandidate = false;
	
public:
	virtual void NativeInteract();

	UPROPERTY(BlueprintAssignable)
	FInteractDelegate OnInteract;

	// This function should be called when judging if this is the best candidate.
	// This will decide whether the owner glow white or not.
	void JudgeGlow(bool Status);

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool GetInteractableStatus() { return bIsInteractable; };

	UFUNCTION(BlueprintCallable)
	void OnActivateTorch();

	UFUNCTION(BlueprintCallable)
	void OnTorchTimePassed();

	FLinearColor WhiteColor = FLinearColor::White;
	FLinearColor DefaultColor = FLinearColor(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditDefaultsOnly)
	FName InteractGlowParamName = FName(TEXT("InteractGlow"));

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bIsInteractable = true;
};
