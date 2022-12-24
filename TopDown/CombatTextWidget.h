// Copyright Hanfei Yang - alleny@usc.edu

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CombatTextWidget.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWN_API UCombatTextWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void ShowDamage(float Damage);

protected:
	// Color of the font when not selected (by default)
	UPROPERTY(EditAnywhere)
	FLinearColor PositiveFontColor = FLinearColor::Red;
	// Color of the font when selected
	UPROPERTY(EditAnywhere)
	FLinearColor NegativeFontColor = FLinearColor::Green;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> DamageTextBlock;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<class UWidgetAnimation> TextFly;
};
