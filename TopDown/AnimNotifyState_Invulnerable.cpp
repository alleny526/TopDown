// Copyright Hanfei Yang - alleny@usc.edu


#include "AnimNotifyState_Invulnerable.h"
#include "TopDownCharacter.h"
#include <Kismet/GameplayStatics.h>

void UAnimNotifyState_Invulnerable::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	Cast<ATopDownCharacter>(MeshComp->GetOwner())->AddRequestor(this);
}

void UAnimNotifyState_Invulnerable::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);	

	Cast<ATopDownCharacter>(MeshComp->GetOwner())->RemoveRequestor(this);
}
