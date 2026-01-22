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
	check(Subsystem); 
	Subsystem->AddMappingContext(AuraContext, 0); //填入映射map ； 

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

	LastActor = ThisActor; 
	ThisActor = Cast<IEnemyInterface>(CursorResult.GetActor()); 

	if (LastActor ==nullptr)
	{
		if (ThisActor != nullptr)
		{
			ThisActor->HightlightActor(); 
		}
		else
		{

		}
	}
	else//lastactor 不为空
	{
		if (ThisActor ==nullptr)
		{
			LastActor->UnHightlightActor(); 
		}
		else
		{
			if (LastActor != ThisActor)
			{
				LastActor->UnHightlightActor(); 
				ThisActor->HightlightActor(); 
			}
			else
			{

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


