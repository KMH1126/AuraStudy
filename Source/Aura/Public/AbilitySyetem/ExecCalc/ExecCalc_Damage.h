// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectCalculation.h"
#include "GameplayEffectExecutionCalculation.h"
#include "AbilitySyetem/Library/AuraAbilitySystemLibrary.h"
#include "AbilitySyetem/Data/CharacterClassInfo/CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"
#include "AuraAbilityTypes.h"
#include "ExecCalc_Damage.generated.h"

/**
 * 
 */

UCLASS()
class AURA_API UExecCalc_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:
	UExecCalc_Damage(); 

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
