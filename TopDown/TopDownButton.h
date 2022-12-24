// Copyright Hanfei Yang - alleny@usc.edu

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TopDownButton.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTopDownButtonDelegate);

UCLASS()
class TOPDOWN_API UTopDownButton : public UUserWidget
{
	GENERATED_BODY()

public:	
	// Called when this button is selected or unselected
	virtual void NativeSetSelection(bool bIsSelected = true);
	// Called when this button is "activated"
	virtual void NativeActivated();
	// Text to show on the button
	UPROPERTY(EditAnywhere)
	FText ButtonText;
	// Triggered when this button is activated
	UPROPERTY(BlueprintAssignable)
	FTopDownButtonDelegate OnActivated;

protected:
	// UUserWidget interface
	//  Called once both in editor and in game, when about to be
	//  constructed
	virtual void NativePreConstruct() override;
	//~ End UUserWidget interface
	// Color of the font when not selected (by default)
	UPROPERTY(EditAnywhere)
	FLinearColor DefaultFontColor = FLinearColor::White;
	// Color of the font when selected
	UPROPERTY(EditAnywhere)
	FLinearColor SelectedFontColor = FLinearColor::Blue;

private:
	// Actual text block widget in the UMG editor
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> ButtonTextBlock;
};