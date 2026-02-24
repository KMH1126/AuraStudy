// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AttributeWidgetController.h"
#include "AuraGameplayTags.h"

void UAttributeWidgetController::BindCallbacksToDependencies()
{
	AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	for (auto& Pair : AuraAttributeSet->TagsToAttribute)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)

			{
				FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Pair.Key);
				Info.AttributeValue = Pair.Value().GetNumericValue(AuraAttributeSet); 
				AttributeInfoSignatureDelegate.Broadcast(Info);


			}
		);
	}

}

UAttributeWidgetController::UAttributeWidgetController()
{
} 

void UAttributeWidgetController::BroadcastInitialValues()
{
	AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	check(AttributeInfo);
	for (auto& Pair : AuraAttributeSet->TagsToAttribute)
	{
		FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Pair.Key);
		Info.AttributeValue = Pair.Value().GetNumericValue(AuraAttributeSet);
		AttributeInfoSignatureDelegate.Broadcast(Info);
	}
	

}