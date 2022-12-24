// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopDownCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "TopDownPlayerController.h"
#include "Components/WidgetComponent.h"
#include "CombatTextWidget.h"

ATopDownCharacter::ATopDownCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	CombatTextWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("CombatTextWidget"));
	CombatTextWidget->SetupAttachment(RootComponent);
	CombatTextWidget->SetWidgetSpace(EWidgetSpace::Screen);
}

void ATopDownCharacter::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
}

void ATopDownCharacter::Tick(float DeltaSeconds)
{
	if (InvulnerableRequestors.Num() > 0)
	{
		GetMesh()->SetVectorParameterValueOnMaterials(TintParamName, FVector(BlueColor.R, BlueColor.G, BlueColor.B));
	}
	else if (InvulnerableRequestors.Num() == 0)
	{
		GetMesh()->SetVectorParameterValueOnMaterials(TintParamName, FVector(DefaultColor.R, DefaultColor.G, DefaultColor.B));
	}

	if (bIsDashing)
	{
		AddMovementInput(GetActorForwardVector(), 1.0f);
	}
	
	FVector CharacterVelocity = GetCharacterMovement()->Velocity;
	if (CharacterVelocity.X == 0.0f && CharacterVelocity.Y == 0.0f && CharacterVelocity.Z == 0.0f)
	{
		if (!bIsStop)
		{
			bIsStop = true;
			GetWorldTimerManager().SetTimer(MoveTimerHandle, this, &ATopDownCharacter::DecrementThreatLevel, 0.5f);
		}
		
	}
	else
	{
		if (bIsStop)
		{
			bIsStop = false;
			IncrementThreatLevel();
		}
	}

	if (bIsCrouched && !bCalculatedCrouch)
	{
		bCalculatedCrouch = true;
		DecrementThreatLevel();
	}
	else if (!bIsCrouched && bCalculatedCrouch)
	{
		bCalculatedCrouch = false;
		IncrementThreatLevel();
	}

	if (bIsProned && !bCalculatedProne)
	{
		bCalculatedProne = true;
		DecrementThreatLevel();
	}
	else if (!bIsProned && bCalculatedProne)
	{
		bCalculatedProne = false;
		IncrementThreatLevel();
	}

    Super::Tick(DeltaSeconds);	
}

void ATopDownCharacter::OnEnterBlueTrigger()
{
	NumBlueVolumes++;
	if (NumBlueVolumes == 1)
	{
		GetMesh()->SetVectorParameterValueOnMaterials(TintParamName, FVector(BlueColor.R, BlueColor.G, BlueColor.B));
	}
}

void ATopDownCharacter::OnExitBlueTrigger()
{
	NumBlueVolumes--;
	if (NumBlueVolumes == 0)
	{
		GetMesh()->SetVectorParameterValueOnMaterials(TintParamName, FVector(DefaultColor.R, DefaultColor.G, DefaultColor.B));
	}
}

void ATopDownCharacter::OnEnterStealthTrigger()
{
	NumStealthVolumes++;
	if (NumStealthVolumes == 1)
	{
		ThreatLevel--;
	}
}

void ATopDownCharacter::OnExitStealthTrigger()
{
	NumStealthVolumes--;
	if (NumStealthVolumes == 0)
	{
		ThreatLevel++;
	}
}

void ATopDownCharacter::StartDash()
{
	if (!bIsDashing)
	{
		if (GetCharacterMovement()->IsCrouching())
		{
			UnCrouch();
		}

		GetCharacterMovement()->MaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed * DashSpeedMultiplier;
		GetCharacterMovement()->MaxAcceleration = GetCharacterMovement()->MaxAcceleration * DashAccelMultiplier;

		FTimerHandle TimerHandle;
		DashMontageLength = GetMesh()->GetAnimInstance()->Montage_Play(DashMontage);
		bIsDashing = true;

		GetWorldTimerManager().SetTimer(TimerHandle, this, &ATopDownCharacter::EndDash, DashMontageLength);
	}
}

void ATopDownCharacter::EndDash()
{
	bIsDashing = false;

	GetCharacterMovement()->MaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed / DashSpeedMultiplier;
	GetCharacterMovement()->MaxAcceleration = GetCharacterMovement()->MaxAcceleration / DashAccelMultiplier;
}

bool ATopDownCharacter::GetDashingStatus()
{
	return bIsDashing;
}

float ATopDownCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ReturnedDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (ReturnedDamage != 0.0f)
	{
		CurrentHealth -= ReturnedDamage;
		Cast<UCombatTextWidget>(CombatTextWidget->GetWidget())->ShowDamage(Damage);
	}

	if (CurrentHealth == 0.0f && !bIsDead)
	{
		bIsDead = true;

		FTimerHandle TimerHandle;
		DeathMontageLength = GetMesh()->GetAnimInstance()->Montage_Play(DeadMontage);
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ATopDownCharacter::PauseAllAnimations, DeathMontageLength - 0.25f);

		// Get this pawn's controller as a APlayerController
		APlayerController* PC = GetController<APlayerController>();
		// Call disable input on the controller (it's silly, but you have to pass in itself)
		if (PC)
		{
			PC->DisableInput(PC);
			Cast<ATopDownPlayerController>(PC)->ShowDeadMenu();
		}
	}

	return ReturnedDamage;
}

bool ATopDownCharacter::ShouldTakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) const
{	
	if (bIsDead || InvulnerableRequestors.Num() > 0)
	{
		return false;
	}
	else
	{
		return Super::ShouldTakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	}
}

bool ATopDownCharacter::CanCrouch() const
{
	if (bIsDashing)
	{
		return false;
	}
	else
	{
		return Super::CanCrouch();
	}
}

void ATopDownCharacter::PauseAllAnimations()
{
	if (this)
	{
		FindComponentByClass<USkeletalMeshComponent>()->Deactivate();
	}	
}

void ATopDownCharacter::AddRequestor(UObject* Requestor)
{
	if (this)
	{
		InvulnerableRequestors.Add(Requestor);
	}
}

void ATopDownCharacter::RemoveRequestor(UObject* Requestor)
{
	if (this)
	{
		InvulnerableRequestors.Remove(Requestor);
	}
}
