// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "Player/APlayerState.h"
#include "HUD/AuraHUD.h"
#include "Player/AuraPlayerController.h"
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
	
	virtual void PossessedBy(AController* NewController) override; 
	virtual void OnRep_PlayerState() override; 

private:
	void InitAbilityActorInfo(); 
};
