// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"

UOverlayWidgetController::UOverlayWidgetController()
{
	
}

void UOverlayWidgetController::BroadcastInitialValues()
{
	AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth()); 
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth()); 

	OnManaChanged.Broadcast(AuraAttributeSet->GetMana()); 
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
}
void UOverlayWidgetController::BindCallbacksToDependencies()
{
	AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	//把我（this）加入 Health 的通知名单；以后 Health 变了，就调用我的 HealthChanged
	 //ASC 里“已有一个群”，你把自己的函数加进去把 OverlayWidgetController 的 HealthChanged(Data) 加进 ASC 的群。
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnManaChanged.Broadcast(Data.NewValue);
			}
		);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetMaxManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxManaChanged.Broadcast(Data.NewValue);
			}
		);

	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
		[this](const FGameplayTagContainer& AssetTagContainer)
		{

			for (FGameplayTag Tag : AssetTagContainer)
			{
				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(TEXT("Message"));  //拿到messagetag的父层级 再判断是不是messagetag 再广播row
				if (Tag.MatchesTag(MessageTag))
				{
					FUIWidgetRow* ROW = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetStructDataTable, Tag);
					if (ROW)
					{
						OnMessageWidgetOut.Broadcast(*ROW); 
					}
				}
			}
		}
	);
}

