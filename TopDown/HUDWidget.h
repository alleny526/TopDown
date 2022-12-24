// Copyright Hanfei Yang - alleny@usc.edu

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWN_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> HealthAmount;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> HealthBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> ThreatLevel;

	UPROPERTY(EditDefaultsOnly)
	FLinearColor DefaultHealthColor = FLinearColor::Green;

	UPROPERTY(EditDefaultsOnly)
	FLinearColor DangerHealthColor = FLinearColor::Yellow;

	UPROPERTY(EditDefaultsOnly)
	FLinearColor CriticalHealthColor = FLinearColor::Red;
};
