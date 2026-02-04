// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraUserWidgetController.h"
#include "AbilitySyetem/AuraAttributeSet.h"
#include "GameplayEffectTypes.h"
#include "AbilitySyetem/AuraAbilitySystemComponent.h"
#include "UI/Widget/AuraUserWidget.h"
#include "OverlayWidgetController.generated.h"

USTRUCT(BlueprintType)
struct FUIWidgetRow :public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Message = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UAuraUserWidget> WidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Image = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMessageWidgetOut, FUIWidgetRow, MessageRow);



/**
 * ASC 群发 → 调用 Controller::HealthChanged → Controller 群发 OnHealthChanged → UI 收到并更新。
 */
UCLASS(BlueprintType , Blueprintable)
class AURA_API UOverlayWidgetController : public UAuraUserWidgetController
{
	GENERATED_BODY()
	
public:
	UOverlayWidgetController(); 

	virtual void BroadcastInitialValues() override; 


	virtual void BindCallbacksToDependencies() override; //ASC 里“已有一个群”，你把自己的函数加进去把 OverlayWidgetController 的 HealthChanged(Data) 加进 ASC 的群。


	//-----------------------------------------------------------------------------------------------------------------------------
	//UPROPERTY(BlueprintAssignable) OnHealthChanged：Controller 自己建的 UI 群 UI/蓝图可以加入。
	UPROPERTY(BlueprintAssignable, Category = "GAS/Attribute")
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS/Attribute")
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS/Attribute")
	FOnAttributeChangedSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS/Attribute")
	FOnAttributeChangedSignature OnMaxManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS/Message")
	FOnMessageWidgetOut OnMessageWidgetOut;

	UAuraAttributeSet* AuraAttributeSet; 

	//----------------------------------------------------------------------------------------------------------------------------
protected:

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "GAS/MessageUI")
	TObjectPtr<UDataTable> MessageWidgetStructDataTable; 

	

	template<typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag); //模板函数
};

template<typename T>
inline T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	return DataTable->FindRow<T>(Tag.GetTagName() , TEXT(""));
}
