// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_ConditionalLoop.generated.h"

/**
 * Conditional loop decorator node.
 * A decorator node that loops execution as long as condition is satisfied.
 */
UCLASS(HideCategories=(FlowControl))
class AIMODULE_API UBTDecorator_ConditionalLoop : public UBTDecorator_Blackboard
{
	GENERATED_UCLASS_BODY()

	virtual bool CalculateRawConditionValue(class UBehaviorTreeComponent* OwnerComp, uint8* NodeMemory) const override;
	virtual void OnBlackboardChange(const class UBlackboardComponent* Blackboard, FBlackboard::FKey ChangedKeyID) override;
	virtual void OnNodeDeactivation(struct FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type NodeResult) override;

#if WITH_EDITOR
	virtual FName GetNodeIconName() const override;
#endif // WITH_EDITOR
};
