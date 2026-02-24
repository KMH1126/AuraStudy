// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraUserWidgetController.h"
#include "AbilitySyetem/AuraAttributeSet.h"
#include "AbilitySyetem/AuraAbilitySystemComponent.h"
#include "AbilitySyetem/Data/AttributeInfo.h"
#include "AttributeWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FAuraAttributeInfo&, Info);
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UAttributeWidgetController : public UAuraUserWidgetController
{
	GENERATED_BODY()

public:
	UAttributeWidgetController();

	UFUNCTION(BlueprintCallable, Category = "GAS/AttributeInfo")
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
	UAuraAttributeSet* AuraAttributeSet = nullptr;


	UPROPERTY(BlueprintAssignable, Category = "GAS/AttributeInfo")
	FAttributeInfoSignature AttributeInfoSignatureDelegate;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfo> AttributeInfo;
};
