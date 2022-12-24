// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopDownPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "TopDownCharacter.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InteractSubsystem.h"
#include "HUDWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "TopDownMenu.h"

ATopDownPlayerController::ATopDownPlayerController()
{
	bShowMouseCursor = false;
	DefaultMouseCursor = EMouseCursor::Default;
}

void ATopDownPlayerController::ShowDeadMenu()
{
	if (MenuWidgetClass)
	{
		MenuInstance = NewObject<UTopDownMenu>(this, MenuWidgetClass);
		MenuInstance->AddToViewport();

		// For a widget which we want to accept input from a player,
		// we have to set the owning player controller
		MenuInstance->SetOwningPlayer(this);
		// InputMode UI means the input is only routed to the UI,
		// and we specify which widget should receive input
		UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(this, MenuInstance);
	}
}

void ATopDownPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

}

void ATopDownPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveForward", this, &ATopDownPlayerController::OnMoveForwardAxis);
	InputComponent->BindAxis("MoveRight", this, &ATopDownPlayerController::OnMoveRightAxis);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ATopDownPlayerController::OnJumpAction);
	InputComponent->BindAction("Interact", IE_Pressed, this, &ATopDownPlayerController::OnInteractAction);
	InputComponent->BindAction("ToggleCrouch", IE_Pressed, this, &ATopDownPlayerController::OnToggleCrouchAction);
	InputComponent->BindAction("Dash", IE_Pressed, this, &ATopDownPlayerController::OnDashAction);
	InputComponent->BindAction("ToggleProne", IE_Pressed, this, &ATopDownPlayerController::OnToggleProneAction);
}

void ATopDownPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	if (HUDWidgetClass)
	{
		HUDWidgetInstance = NewObject<UHUDWidget>(this, HUDWidgetClass);
		HUDWidgetInstance->AddToViewport();
	}
}

void ATopDownPlayerController::OnJumpAction()
{
	if (ACharacter* MyCharacter = GetCharacter())
	{
		MyCharacter->GetCharacterMovement()->DoJump(false);
	}
}

void ATopDownPlayerController::OnInteractAction()
{
	if (UInteractSubsystem* Subsystem = GetWorld()->GetSubsystem<UInteractSubsystem>())
	{
		Subsystem->PerformInteract();
	}
}

void ATopDownPlayerController::OnToggleCrouchAction()
{
	if (ACharacter* MyCharacter = GetCharacter())
	{
		if (!MyCharacter->bIsCrouched)
		{
			MyCharacter->Crouch();
		}
		else
		{
			MyCharacter->UnCrouch();
		}
	}
}

void ATopDownPlayerController::OnDashAction()
{
	if (ATopDownCharacter* MyPawn = GetPawn<ATopDownCharacter>())
	{
		MyPawn->StartDash();
	}
}

void ATopDownPlayerController::OnToggleProneAction()
{
	if (ATopDownCharacter* MyPawn = GetPawn<ATopDownCharacter>())
	{
		if (!MyPawn->GetPronedStatus())
		{
			MyPawn->SetPronedStatus(true);
			SetIgnoreMoveInput(true);
		}
		else
		{
			MyPawn->SetPronedStatus(false);

			FTimerHandle TimerHandle;
			GetWorldTimerManager().SetTimer(TimerHandle, this, &ATopDownPlayerController::RecoverMoveControl, 1.23f);
		}
	}
}

void ATopDownPlayerController::OnMoveForwardAxis(float Value)
{
	if (ATopDownCharacter* MyPawn = GetPawn<ATopDownCharacter>())	
	{
		if (!MyPawn->GetDashingStatus())
		{
			MyPawn->AddMovementInput(FVector::ForwardVector, Value);
		}
	}
}

void ATopDownPlayerController::OnMoveRightAxis(float Value)
{
	if (ATopDownCharacter* MyPawn = GetPawn<ATopDownCharacter>())
	{
		if (!MyPawn->GetDashingStatus())
		{
			MyPawn->AddMovementInput(FVector::RightVector, Value);
		}
	}
}
