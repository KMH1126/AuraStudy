// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Input/AuraInputComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "UI/Widget/DamageTextComponent.h"
#include "GameFramework/Character.h"


AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true; 
    Spline = CreateDefaultSubobject<USplineComponent>("Spline");
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
    AutoRun();
}

void AAuraPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
    AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);

    AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &AAuraPlayerController::ShiftStart);
    AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &AAuraPlayerController::ShiftReleased);

    AuraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}




void AAuraPlayerController::AutoRun()
{
    if (!bAutoRunning) return;
    if (APawn* ControlledPawn = GetPawn())
    {
        const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
        const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
        ControlledPawn->AddMovementInput(Direction);

        const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
        if (DistanceToDestination <= AutoRunAcceptanceRadius)
        {
            bAutoRunning = false;
        }
    }
}

void AAuraPlayerController::CursorTrace()
{
   
    GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
    if (!CursorHit.bBlockingHit)
    {
        return;
    }
    LastActor = ThisActor;
    ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());
    if (LastActor != ThisActor)
    {
        if (LastActor)
        {
            LastActor->UnHightlightActor();
        }
        if (ThisActor)
        {
            ThisActor->HightlightActor();
        }
       
    }
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
    //GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Red, *InputTag.ToString());
    if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
    {
        bTargeting = ThisActor ? true : false; 
        bAutoRunning = false; 
    }
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
    if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
    {
        if (GetAuraASC())
        {
            GetAuraASC()->AbilityInputTagReleased(InputTag);
        }
        return;
    }

    if (GetAuraASC())
    {
        GetAuraASC()->AbilityInputTagReleased(InputTag);
    }

    if (!bTargeting && !bShiftPressed)
    {
       const  APawn* ControlledPawn = GetPawn();
        if (FollowTime <= ShortPressThreshold && ControlledPawn)
        {
            if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
            {
                Spline->ClearSplinePoints();
                for (const FVector& PointLoc : NavPath->PathPoints)
                {
                    Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
                    DrawDebugSphere(GetWorld(), PointLoc, 8.f, 8, FColor::Green, false, 5.f);
                }
                CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
                bAutoRunning = true;
            }
        }
        FollowTime = 0.f;
        bTargeting = false;
    }


}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
    if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
    {
        if (GetAuraASC())
        {
            GetAuraASC()->AbilityInputTagHeld(InputTag);
        }
        return;
    }

    if (bTargeting || bShiftPressed)
    {
        if (GetAuraASC())
        {
            GetAuraASC()->AbilityInputTagHeld(InputTag);
        }
    }
    else
    {
        FollowTime += GetWorld()->GetDeltaSeconds();

        FHitResult Hit;
        if (GetHitResultUnderCursor(ECC_Visibility, false, Hit))
        {
            CachedDestination = Hit.ImpactPoint;
        }

        if (APawn* ControlledPawn = GetPawn())
        {
            const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
            ControlledPawn->AddMovementInput(WorldDirection);
        }
    }
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetAuraASC()
{
    if ( AuraAbilitySystemComponent == nullptr)
    {
        AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
    }
    return AuraAbilitySystemComponent; 
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

void AAuraPlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter,bool bInBlocked, bool bInCritical)
{
    if (IsValid(TargetCharacter) && DamageTextComponentClass && IsLocalController())
    {
        UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
        DamageText->RegisterComponent();
        DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
        DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
        DamageText->SetDamageText(DamageAmount,  bInBlocked,  bInCritical);
    }
}