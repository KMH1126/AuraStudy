// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "Player/APlayerState.h"
#include "HUD/AuraHUD.h"
#include "Player/AuraPlayerController.h"
#include "AbilitySyetem/AuraAbilitySystemComponent.h"
#include "AuraCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase 
{
	GENERATED_BODY()

public:
	AAuraCharacter(); 
	
	//Enemy接口的重写函数
	virtual void PossessedBy(AController* NewController) override; 
	virtual void OnRep_PlayerState() override; 

	//Combat接口的重写函数
	UFUNCTION(BlueprintCallable)
	virtual int GetPlayerLevel() override; 

	

private:
	virtual void InitAbilityActorInfo() override; 
};
