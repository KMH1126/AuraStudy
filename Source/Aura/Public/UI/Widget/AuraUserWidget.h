// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AuraUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController); 


	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController; 

protected:

	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet(); // 在UAuraUserWidget::SetWidgetController(UObject* InWidgetController)这个函数里面执行了 套娃了（（（

	
};
