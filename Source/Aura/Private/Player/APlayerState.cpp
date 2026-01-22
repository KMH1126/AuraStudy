// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/APlayerState.h"
#include "AbilitySyetem/AuraAbilitySystemComponent.h"
#include "AbilitySyetem/AuraAttributeSet.h"

AAPlayerState::AAPlayerState()
{

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>(TEXT("AuraASC"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed); 

	AttributeSet=CreateDefaultSubobject<UAuraAttributeSet>(TEXT("AuraAttributeSet"));

	NetUpdateFrequency = 100.f;  
}

UAbilitySystemComponent* AAPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent; 
}

UAttributeSet* AAPlayerState::GetAttributeSet() const
{
	return AttributeSet; 
}