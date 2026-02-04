// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/AuraHUD.h"

UOverlayWidgetController* AAuraHUD::GetOverlayController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass); 
		OverlayWidgetController->SetWidgetControllerParams(WCParams); 
		OverlayWidgetController->BindCallbacksToDependencies(); 
		return OverlayWidgetController;
	}

	return OverlayWidgetController;
}

void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{


	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass); //实例化OverlayWidgetClass
	OverlayWidget = Cast<UAuraUserWidget>(Widget);									 //OverlayWidget指针指向class

	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);			//创建结构体

	UOverlayWidgetController* WidgetController = GetOverlayController(WidgetControllerParams);
	OverlayWidget->SetWidgetController(WidgetController);//给OverlayWidgetController填入结构体并返回，OverlayWidget有了对应的controller
																					//GetOverlayController(WidgetControllerParams)就是OverlayWidgetController
	
	WidgetController->BroadcastInitialValues(); 

	OverlayWidget->AddToViewport(); 

}

