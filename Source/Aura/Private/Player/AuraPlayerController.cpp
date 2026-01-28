// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"


AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true; 
}
void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay(); 
	check(AuraContext); 

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());  
	//check(Subsystem); 不要老是check 容易crush
	if (Subsystem)
	{
		Subsystem->AddMappingContext(AuraContext, 0); //填入映射map ； 
	}
	

	bShowMouseCursor = true; 
	DefaultMouseCursor = EMouseCursor::Default; 

	FInputModeGameAndUI InputModeData; 
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false); 
	SetInputMode(InputModeData); 
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime); 
	CursorTrace(); 
}

void AAuraPlayerController::CursorTrace()
{
    FHitResult CursorResult;
    GetHitResultUnderCursor(ECC_Visibility, false, CursorResult);
    if (!CursorResult.bBlockingHit)
    {
        return;
    }

    // 1) 先更新 LastActor
    LastActor = ThisActor;

    // 2) 正确生成 ThisActor（TScriptInterface）
    ThisActor.SetObject(nullptr);
    ThisActor.SetInterface(nullptr);

    AActor* HitActor = CursorResult.GetActor();
    if (HitActor && HitActor->Implements<UEnemyInterface>())
    {
        ThisActor.SetObject(HitActor);
        ThisActor.SetInterface(Cast<IEnemyInterface>(HitActor));
    }

    // 3) 用 GetObject() 判空；用 GetInterface() 调接口
    const bool bLastValid = (LastActor.GetObject() != nullptr);
    const bool bThisValid = (ThisActor.GetObject() != nullptr);

    if (!bLastValid)
    {
        if (bThisValid)
        {
            if (IEnemyInterface* I = ThisActor.GetInterface())
            {
                I->HightlightActor();
            }
        }
        else
        {
            // 都空：啥也不做
        }
    }
    else // lastactor 不为空
    {
        if (!bThisValid)
        {
            if (IEnemyInterface* I = LastActor.GetInterface())
            {
                I->UnHightlightActor();
            }
        }
        else
        {
            if (LastActor.GetObject() != ThisActor.GetObject())
            {
                if (IEnemyInterface* I = LastActor.GetInterface())
                {
                    I->UnHightlightActor();
                }
                if (IEnemyInterface* I = ThisActor.GetInterface())
                {
                    I->HightlightActor();
                }
            }
            else
            {
                // 指向同一个对象：啥也不做
            }
        }
    }
}



void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent); 
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move); 

}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>(); //这里获取的是键盘（WSAD）或手柄左摇杆的原始值。//InputAxisVector.Y：通常代表 前/后（W/S）。//InputAxisVector.X：通常代表 左/右（A/D）。
	const FRotator Rotation = GetControlRotation();						//这是 控制器（Controller） 的旋转，通常等同于 摄像机（Camera） 的朝向。		
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);					

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X); 
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControllPawn = GetPawn<APawn>())
	{
		ControllPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y); 
		ControllPawn->AddMovementInput(RightDirection, InputAxisVector.X);

	}

}


