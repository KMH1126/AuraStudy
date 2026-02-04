// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AuraUserWidgetController.h"

void UAuraUserWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController; 
	PlayerState = WCParams.PlayerState; 
	AbilitySystemComponent = WCParams.AbilitySystemComponent; 
	AttributeSet = WCParams.AttributeSet; 
}

void UAuraUserWidgetController::BroadcastInitialValues()
{
}

void UAuraUserWidgetController::BindCallbacksToDependencies()
{
}
