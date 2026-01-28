// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"

#include "AuraAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
/**
 * 
 定义了一个属性集

里面有一个属性叫 Health

Health 是 GAS 管理的数值

服务器改 Health 时，会自动同步到客户端

客户端收到后，调用 OnRep_Health

OnRep_Health 会告诉 GAS：“血量变了”
---------------------------------------
GameplayEffect
		↓
AbilitySystemComponent
		↓
AttributeSet（你这个类）
		↓
FGameplayAttributeData Health
		↓
OnRep_Health（客户端）
 */

UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UAuraAttributeSet(); 



	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override; //告诉 Unreal 的网络系统：AttributeSet 里，哪些变量要
																										//从服务器复制到客户端，以及复制时要不要触发回调。
																										 
																										 
																										
	//当服务器上的 Health 发生变化，并同步到客户端时，自动调用 OnRep_Health()------------------
	//记录当前值/BaseValue/CurrentValue/网络同步
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes")
	FGameplayAttributeData Health;		
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Health); 


	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Vital Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Mana Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Mana);


	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Mana Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxMana);


	//通知 AbilitySystemComponent：这个属性刚刚发生了网络同步变化---------------------
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;								

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;
	
};
