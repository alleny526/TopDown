// Copyright Hanfei Yang - alleny@usc.edu


#include "BTService_FindPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "TopDownAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTService_FindPlayer::UBTService_FindPlayer()
{
	NodeName = "FindPlayer";
	
	PlayerKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_FindPlayer, PlayerKey), AActor::StaticClass());
}

void UBTService_FindPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	
	ATopDownCharacter* PlayerChar = Cast<ATopDownCharacter>(UGameplayStatics::GetPlayerCharacter(&OwnerComp, 0));
	int ThreatLevelNum = PlayerChar->GetThreatLevel();
	AAIController* AIController = OwnerComp.GetAIOwner();
	ATopDownAICharacter* AIChar = Cast<ATopDownAICharacter>(AIController->GetCharacter());

	int CharDistance = static_cast<int>(FVector::Distance(AIChar->GetActorLocation(), PlayerChar->GetActorLocation()));

	FVector AIForwardVector = AIChar->GetActorForwardVector();
	AIForwardVector.Normalize();
	FVector AIToPlayerVector = PlayerChar->GetActorLocation() - AIChar->GetActorLocation();
	AIToPlayerVector.Normalize();
	float CharAngle = FMath::Acos(FVector::DotProduct(AIForwardVector, AIToPlayerVector));

	TMap<int, TPair<int, float>> ThreatDistAngle;
	ThreatDistAngle.Add(1, TPair<int, float>(100, 0.52f));
	ThreatDistAngle.Add(2, TPair<int, float>(500, 0.87f));
	ThreatDistAngle.Add(3, TPair<int, float>(2000, 1.22f));
	ThreatDistAngle.Add(4, TPair<int, float>(4000, 1.57f));

	if (PlayerChar && !PlayerChar->IsDead())
	{
		if (CharDistance <= ThreatDistAngle.Find(ThreatLevelNum)->Key && CharAngle <= ThreatDistAngle.Find(ThreatLevelNum)->Value)
		{
			MyBlackboard->SetValueAsObject(GetSelectedPlayerKey(), PlayerChar);
		}
		else
		{
			MyBlackboard->SetValueAsObject(GetSelectedPlayerKey(), nullptr);
		}
	}
	else
	{
		MyBlackboard->SetValueAsObject(GetSelectedPlayerKey(), nullptr);
	}

	FHitResult Result;
	if (PlayerChar)
	{
		GetWorld()->LineTraceSingleByChannel(Result, AIChar->GetActorLocation(), PlayerChar->GetActorLocation(), ECC_Camera);
	}

	if (AActor* ResultActor = Result.GetActor())
	{
		if (Cast<ATopDownCharacter>(ResultActor) != PlayerChar)
		{
			MyBlackboard->SetValueAsObject(GetSelectedPlayerKey(), nullptr);
		}
	}
}

void UBTService_FindPlayer::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	UBlackboardData* BBAsset = GetBlackboardAsset();
	if (BBAsset)
	{
		PlayerKey.ResolveSelectedKey(*BBAsset);
	}
}
