// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

AAuraCharacter::AAuraCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true; 
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f); 
	GetCharacterMovement()->bConstrainToPlane = true; 
	GetCharacterMovement()->bSnapToPlaneAtStart = true; 

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false; 
	bUseControllerRotationYaw = false; 


}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController); 
	//在服务器上初始化AbilityInfo
	InitAbilityActorInfo(); 
	

}
 void AAuraCharacter::OnRep_PlayerState()
{
	 Super::OnRep_PlayerState();
	 //在客户端上初始化AbilityInfo
	 InitAbilityActorInfo();
}
 void AAuraCharacter::InitAbilityActorInfo()
 {
	 
	 AAPlayerState* AuraPlayerState = GetPlayerState<AAPlayerState>();
	// check(AuraPlayerState);
	 AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this);
	 AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent(); 
	 AttributeSet = AuraPlayerState->GetAttributeSet(); 

	 if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	 {
		 if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		 {
			 AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet); 
		 }
	 }
 }
