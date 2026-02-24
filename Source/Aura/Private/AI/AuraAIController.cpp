// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AuraAIController.h"

AAuraAIController::AAuraAIController()
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("behaviorTreeComponent"));
	check(BehaviorTreeComponent);  
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoard")); 
	check(Blackboard); 
}
