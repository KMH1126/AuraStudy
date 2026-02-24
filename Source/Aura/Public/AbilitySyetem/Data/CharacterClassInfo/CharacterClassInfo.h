// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterClassInfo.generated.h"

/**
 * 
 */
class UGameplayEffect;
class UGameplayAbility;

USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "class Default")
	TSubclassOf<UGameplayEffect> PrimaryAttributeClass; 
};

UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	Elementalist,
	Warrior,
	Ranger
};


UCLASS(BlueprintType)
class AURA_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TMap<ECharacterClass, FCharacterClassDefaultInfo> CharacterClassInformation; 

	UPROPERTY(EditDefaultsOnly, Category = "Common Default")
	TSubclassOf<UGameplayEffect> SecondaryAttributeClass;

	UPROPERTY(EditDefaultsOnly, Category = "Common Default")
	TSubclassOf<UGameplayEffect> VitalAttributeClass;

	UPROPERTY(EditDefaultsOnly, Category = "Common Default")
	TArray<TSubclassOf<UGameplayAbility>> CommonAbilities; 

	UPROPERTY(EditDefaultsOnly, Category = "Common Default|Damage")
	TObjectPtr<UCurveTable> DamageCalculationCoefficients;

	UFUNCTION(BlueprintCallable, Category = "Common Default")
	FCharacterClassDefaultInfo GetCharacterClassDefaultInfo(ECharacterClass CharacterClass); 
};
