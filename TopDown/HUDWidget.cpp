// Copyright Hanfei Yang - alleny@usc.edu


#include "HUDWidget.h"
#include "Kismet/GameplayStatics.h"
#include "TopDownCharacter.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Update stuff
	if (auto Char = Cast<ATopDownCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)))
	{
		// Health Bar
		const float HealthPct = Char->GetHealthPercent();
		HealthBar->SetPercent(HealthPct);
		
		if (HealthPct > 0.7f)
		{
			HealthBar->SetFillColorAndOpacity(DefaultHealthColor);
		}
		else if (HealthPct > 0.3f)
		{
			HealthBar->SetFillColorAndOpacity(DangerHealthColor);
		}
		else 
		{
			HealthBar->SetFillColorAndOpacity(CriticalHealthColor);
		}

		FString HealthAsString = FString::Printf(TEXT("%d"), static_cast<int>(Char->GetCurrentHealth()));
		HealthAmount->SetText(FText::FromString(HealthAsString));

		// Threat Level
		const int ThreatLevelNum = Char->GetThreatLevel();
		
		if (ThreatLevelNum == 1)
		{
			ThreatLevel->SetColorAndOpacity(FLinearColor::Green);
		}
		else if (ThreatLevelNum == 2)
		{
			ThreatLevel->SetColorAndOpacity(FLinearColor::Yellow);
		}
		else if (ThreatLevelNum == 3)
		{
			ThreatLevel->SetColorAndOpacity(FLinearColor::Red);
		}
		else if (ThreatLevelNum == 4)
		{
			ThreatLevel->SetColorAndOpacity(FLinearColor(128.0f, 0.0f, 128.0f));
		}
	}
}
