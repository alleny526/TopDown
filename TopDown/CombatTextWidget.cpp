// Copyright Hanfei Yang - alleny@usc.edu


#include "CombatTextWidget.h"
#include "Components/TextBlock.h"

void UCombatTextWidget::ShowDamage(float Damage)
{
	FString DamageAsString = FString::Printf(TEXT("%d"), FMath::Abs(static_cast<int>(Damage)));
	// Set the text
	DamageTextBlock->SetText(FText::FromString(DamageAsString));
	// Set the text color
	DamageTextBlock->SetColorAndOpacity(Damage > 0 ? PositiveFontColor : NegativeFontColor);

	PlayAnimation(TextFly);
}
