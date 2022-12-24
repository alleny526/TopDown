// Copyright Hanfei Yang - alleny@usc.edu


#include "BTTask_UpdatePatrolPath.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TopDownAICharacter.h"
#include "AIController.h"

UBTTask_UpdatePatrolPath::UBTTask_UpdatePatrolPath()
{
	NodeName = "UpdatePatrolPath";
}

EBTNodeResult::Type UBTTask_UpdatePatrolPath::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();

	AAIController* AIController = OwnerComp.GetAIOwner();
	ATopDownAICharacter* AIChar = Cast<ATopDownAICharacter>(AIController->GetCharacter());


	const FName PatrolIndexName = FName(TEXT("PatrolIndex"));
	int CurrentPatrolIndex = MyBlackboard->GetValueAsInt(PatrolIndexName);

	CurrentPatrolIndex++;

	TArray<TSoftObjectPtr<AActor>> PatrolPath = AIChar->GetPatrolPath();

	if (PatrolPath.Num() == 0)
	{
		return EBTNodeResult::Failed;
	}

	if (CurrentPatrolIndex == PatrolPath.Num())
	{
		CurrentPatrolIndex = 0;
	}

	MyBlackboard->SetValueAsInt(PatrolIndexName, CurrentPatrolIndex);
	if (PatrolPath[CurrentPatrolIndex].Get())
	{
		const FName PatrolActorName = FName(TEXT("PatrolActor"));
		MyBlackboard->SetValueAsObject(PatrolActorName, PatrolPath[CurrentPatrolIndex].Get());
	}

	return EBTNodeResult::Succeeded;
}
