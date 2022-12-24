// Copyright Hanfei Yang - alleny@usc.edu


#include "TopDownButton.h"
#include "Components/TextBlock.h"

void UTopDownButton::NativePreConstruct()
{
	Super::NativePreConstruct();

	// Set the text
	ButtonTextBlock->SetText(ButtonText);
	// Set the text color
	ButtonTextBlock->SetColorAndOpacity(DefaultFontColor);
}

void UTopDownButton::NativeSetSelection(bool bIsSelected /*= true*/)
{
	ButtonTextBlock->SetColorAndOpacity(bIsSelected ? SelectedFontColor : DefaultFontColor);
}

void UTopDownButton::NativeActivated()
{
	// Just trigger the delegate
	OnActivated.Broadcast();
}