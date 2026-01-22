// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AttributeSet.h"
#include "AuraCharacterBase.generated.h"

UCLASS(Abstract)//≥ÈœÛ¿‡
class AURA_API AAuraCharacterBase : public ACharacter,public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();


	virtual void BeginPlay() override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const; 

	UPROPERTY(EditAnywhere, Category = "Combat"); 
	TObjectPtr<USkeletalMeshComponent> Weapon; 

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent; 

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet; 


};
