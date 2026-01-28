// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/WidgetController/AuraUserWidgetController.h"
#include "AuraHUD.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<UAuraUserWidget>  OverlayWidget;

	UOverlayWidgetController* GetOverlayController(const FWidgetControllerParams& WCParams); 

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS); 

private:
	UPROPERTY(EditAnywhere, Category = "AuraOverlay")
	TSubclassOf<UAuraUserWidget> OverlayWidgetClass; //先创建类 实例化之后用指针OverlayWidget去访问修改变量做一些操作

	UPROPERTY(EditAnywhere, Category = "AuraOverlay")
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController>  OverlayWidgetController;

};
