// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TopDownCharacter.generated.h"

UCLASS(Blueprintable)
class ATopDownCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATopDownCharacter();

	virtual void BeginPlay() override;

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	// This function should be called when you enter a blue trigger.
	// This will turn the player blue.
	UFUNCTION(BlueprintCallable)
	void OnEnterBlueTrigger();

	// This function should be called when you exit a blue trigger.
	// This will turn the player blue.
	UFUNCTION(BlueprintCallable)
	void OnExitBlueTrigger();

	UFUNCTION(BlueprintCallable)
	void OnEnterStealthTrigger();

	UFUNCTION(BlueprintCallable)
	void OnExitStealthTrigger();

	void StartDash();
	void EndDash();

	void IncrementThreatLevel() { ThreatLevel++; }
	void DecrementThreatLevel() { ThreatLevel--; }

	int GetThreatLevel() { return ThreatLevel; }

	bool GetDashingStatus();

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual bool ShouldTakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) const override;

	virtual bool CanCrouch() const override;

	void PauseAllAnimations();

	void AddRequestor(UObject* Requestor);
	void RemoveRequestor(UObject* Requestor);

	float GetCurrentHealth() const { return CurrentHealth; }
	float GetHealthPercent() const { return CurrentHealth / MaxHealth; }

	bool GetPronedStatus() const { return bIsProned; }
	void SetPronedStatus(bool bStatus) { bIsProned = bStatus; }

	bool IsDead() const { return bIsDead; }

	// This is the blue color that gets set when in a blue volume
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor BlueColor = FLinearColor::Blue;

	// This is the color we reset to when we are not in any blue volumes
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor DefaultColor = FLinearColor::White;

protected:
	UPROPERTY(Transient)
	TArray<TWeakObjectPtr<UObject>> InvulnerableRequestors;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UWidgetComponent> CombatTextWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsProned = false;

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(EditDefaultsOnly)
	FName TintParamName = FName(TEXT("Tint"));

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAnimMontage> DashMontage;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAnimMontage> DeadMontage;

	UPROPERTY(Transient)
	bool bIsDashing = false;

	UPROPERTY(Transient)
	bool bIsDead = false;
	
	UPROPERTY(EditAnywhere)
	float DashSpeedMultiplier = 2.0f;

	UPROPERTY(EditAnywhere)
	float DashAccelMultiplier = 5.0f;

	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100.0f;

	UPROPERTY(Transient)
	float CurrentHealth;

	bool bIsStop = true;
	bool bCalculatedCrouch = false;
	bool bCalculatedProne = false;

	int ThreatLevel = 3;
	FTimerHandle MoveTimerHandle;

	int NumBlueVolumes = 0;
	int NumStealthVolumes = 0;

	float DashMontageLength = 0.0f;
	float DeathMontageLength = 0.0f;
};

