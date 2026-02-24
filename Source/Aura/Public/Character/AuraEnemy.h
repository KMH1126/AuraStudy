// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "Aura/Aura.h"
#include "Aura/Public/AbilitySyetem/Data/CharacterClassInfo/CharacterClassInfo.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "GameFramework\CharacterMovementComponent.h"
#include "AI/AuraAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AuraEnemy.generated.h"

class UWidgetComponent;
class UCharacterMovementComponent; 
/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:
	AAuraEnemy(); 

	virtual void PossessedBy(AController* NewController)  override;

	/*敌人接口*/
	 void HightlightActor() override;
	 void UnHightlightActor() override;
	 /*敌人接口*/

	 /*Combat接口*/
	 UFUNCTION(BlueprintCallable)
	 virtual int GetPlayerLevel() override; 

	 virtual void InitialDefaultAttributes() const override;
	 virtual void Die() override; 

	 UPROPERTY(BlueprintAssignable, Category = "ASC")
	 FOnAttributeChangedSignature OnHealthChanged;

	 UPROPERTY(BlueprintAssignable, Category = "ASC")
	 FOnAttributeChangedSignature OnMaxHealthChanged;

	 UPROPERTY(EditAnywhere , BlueprintReadOnly)
	 TObjectPtr<UWidgetComponent> HealthBar; 

	 UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	 ECharacterClass CharacterClass = ECharacterClass::Warrior;


	 UPROPERTY(BlueprintReadOnly, Category = "Combat")
	 bool bHitReacting = false;

	 UPROPERTY(BlueprintReadOnly, Category = "Combat")
	 float BaseWalkSpeed = 200.f;


	 UPROPERTY(BlueprintReadOnly, Category = "Combat")
	 float LifeSpan = 5.f;


	 UPROPERTY(EditDefaultsOnly, Category = "AI")
	 TObjectPtr<AAuraAIController> AuraAIController; 

	 UPROPERTY(EditAnywhere, Category = "AI")
	 TObjectPtr<UBehaviorTree> BehaviorTree;


protected:
	virtual void BeginPlay() override; 
	
	virtual void InitAbilityActorInfo() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS/角色等级")
	int Level = 1;

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
};
