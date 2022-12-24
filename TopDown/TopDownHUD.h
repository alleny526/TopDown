// Copyright Hanfei Yang - alleny@usc.edu

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TopDownHUD.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWN_API ATopDownHUD : public AHUD
{
	GENERATED_BODY()

	virtual void DrawHUD() override;
};