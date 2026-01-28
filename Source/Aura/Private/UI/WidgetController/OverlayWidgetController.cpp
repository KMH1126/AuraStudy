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
}