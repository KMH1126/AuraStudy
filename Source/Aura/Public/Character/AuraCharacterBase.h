// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AttributeSet.h"
#include "GameplayEffect.h"
#include "Interaction/CombatInterface.h"
#include "Components/CapsuleComponent.h"
#include "AuraCharacterBase.generated.h"

class UGameplayEffect; 


UCLASS(Abstract)//抽象类
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();


	virtual void BeginPlay() override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void InitialDefaultAttributes() const;
	UAttributeSet* GetAttributeSet() const; 

	virtual void InitAbilityActorInfo();
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;

	virtual void Die() override; 

	UFUNCTION(NetMulticast , Reliable)
	virtual void MulticastHandleDeath();

	void Dissolve(); 
	UFUNCTION(BlueprintImplementableEvent)
	void StartTimeDissolve(UMaterialInstanceDynamic* DissovleMat);
	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponTimeDissolve(UMaterialInstanceDynamic* DissovleMat);

	UPROPERTY(EditAnywhere, Category = "GAS/Combat"); 
	TObjectPtr<USkeletalMeshComponent> Weapon; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS/Combat")
	FName WeaponTipName; 

	virtual FVector GetCombatSocketLocation() override;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent; 

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	UAnimMontage* HitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UMaterialInstance> DissovleMaterial; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UMaterialInstance> WeaponDissovleMaterial;

	//初始化属性----------------------------------------------------------------------------------------------------------------------------
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS/Attributes")
	TSubclassOf<UGameplayEffect> PrimaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS/Attributes")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS/Attributes")
	TSubclassOf<UGameplayEffect> VitalAttributes;

	void ApplayEffectToSelf(TSubclassOf<UGameplayEffect> DefaultAttributes , float Level) const;
	


	//技能部分------------------------------------------------------------------------------------------------------------------------------

	void AddCharacterAbilities();

private:
	UPROPERTY(EditAnywhere, Category = "GAS/Ability")
	TArray<TSubclassOf<UGameplayAbility>> StartUpAbilities; 
	
};
