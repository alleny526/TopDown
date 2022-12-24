// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "TopDownPlayerController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;

UCLASS()
class ATopDownPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATopDownPlayerController();

	class UHUDWidget* GetHUDWidget() const { return HUDWidgetInstance; }
	class UTopDownMenu* GetMenuInstance() const { return MenuInstance; }

	void ShowDeadMenu();

	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UNiagaraSystem* FXCursor;

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* aPawn) override;
	// End PlayerController interface

	// The function for jump action
	void OnJumpAction();

	// The function for interact action
	void OnInteractAction();

	void OnToggleCrouchAction();
	void OnDashAction();
	void OnToggleProneAction();

	// The function for moving forward
	void OnMoveForwardAxis(float Value);

	// The function for moving right
	void OnMoveRightAxis(float Value);

	void RecoverMoveControl() { SetIgnoreMoveInput(false); }

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UHUDWidget> HUDWidgetClass;

	UPROPERTY(Transient)
	TObjectPtr<class UHUDWidget> HUDWidgetInstance;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UTopDownMenu> MenuWidgetClass;

	UPROPERTY(Transient)
	TObjectPtr<class UTopDownMenu> MenuInstance;
};


