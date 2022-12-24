// Copyright Hanfei Yang - alleny@usc.edu


#include "TopDownHUD.h"
#include "DrawDebugHelpers.h"
#include "PuzzleComponent.h"
#include "InteractComponent.h"
#include "InteractSubsystem.h"
#include "TopDownCharacter.h"
#include <Kismet/GameplayStatics.h>

static TAutoConsoleVariable<int32> CVarDebugPuzzles(
	TEXT("DebugPuzzles"),									 // Console command
	0,														 // Default value
	TEXT("Turns puzzle debug view on/off (off by default)"), // Help text
	ECVF_Default);											 // Flags

void ATopDownHUD::DrawHUD()
{
	Super::DrawHUD();

	DrawText(TEXT("Hanfei Yang - alleny@usc.edu"), FLinearColor::Blue, 5.0f, 0.0f, GEngine->GetSubtitleFont(), 0.5f);

	//if (ATopDownCharacter* MyCharacter = Cast<ATopDownCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)))
	//{
	//	int Health = FMath::RoundToInt(MyCharacter->CurrentHealth);
	//	DrawText(FString::Printf(TEXT("Health: %d"), Health), FLinearColor::Yellow, 5.0f, 50.0f, GEngine->GetSubtitleFont(), 0.5f);
	//}

	UInteractSubsystem* Subsystem = GetWorld()->GetSubsystem<UInteractSubsystem>();
	if (UInteractComponent* BestCandidate = Subsystem->ReturnBestCandidate().Get())
	{
		FString InteractMsg = TEXT("[E] - Interact");
		float OutWidth, OutHeight;
		GetTextSize(InteractMsg, OutWidth, OutHeight, nullptr, 1.5f);
		FVector BestCandidateScreenLocation = Project(BestCandidate->GetOwner()->GetActorLocation());
		DrawText(InteractMsg, FLinearColor::Red, BestCandidateScreenLocation.X - OutWidth / 2, BestCandidateScreenLocation.Y - OutHeight / 2, nullptr, 1.5f);
	}
	
	#if ENABLE_DRAW_DEBUG
		if (CVarDebugPuzzles.GetValueOnGameThread() != 0)
		{
			for (TObjectIterator<UPuzzleComponent> Itr; Itr; ++Itr)
			{
				Itr->DebugDrawOutput();
			}
		}
	#endif
}
