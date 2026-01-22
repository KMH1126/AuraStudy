// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "Aura/Aura.h"
#include "AuraEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:
	AAuraEnemy(); 

	/*敌人接口*/
	 void HightlightActor() override;
	 void UnHightlightActor() override;
	 /*敌人接口*/

protected:
	virtual void BeginPlay() override; 
	
};
