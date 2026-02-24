// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraEnemy.h"
#include "AbilitySyetem/AuraAbilitySystemComponent.h"
#include "AbilitySyetem/AuraAttributeSet.h"
#include "UI/Widget/AuraUserWidget.h"
#include "AbilitySyetem/Library/AuraAbilitySystemLibrary.h"
#include "Components/WidgetComponent.h"
#include "AuraGameplayTags.h"

AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>(TEXT("AuraASC")); 
	AbilitySystemComponent->SetIsReplicated(true); 
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);


	AttributeSet=CreateDefaultSubobject<UAuraAttributeSet>(TEXT("AuraAttributeSet")); 

	HealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar")); 
	HealthBar->SetupAttachment(GetRootComponent());

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

}
void AAuraEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!HasAuthority())
	{
		return;
	}

	AuraAIController = Cast<AAuraAIController>(NewController);
	AuraAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->GetBlackboardAsset());
	AuraAIController->RunBehaviorTree(BehaviorTree);

	AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
	AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"), CharacterClass != ECharacterClass::Warrior);
}

void AAuraEnemy::HightlightActor()
{
	GetMesh()->SetRenderCustomDepth(true); 
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED); 
	Weapon->SetRenderCustomDepth(true); 
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED); 
}
void AAuraEnemy::UnHightlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);

}



void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay(); 
	//check(AbilitySystemComponent); 

	BaseWalkSpeed = GetCharacterMovement()->MaxWalkSpeed; 
	InitAbilityActorInfo();
	if (HasAuthority())
	{
		UAuraAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent);
	}
	
	if (UAuraUserWidget* AuraUserWidget = Cast<UAuraUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		AuraUserWidget->SetWidgetController(this);
	}

	if (const UAuraAttributeSet* AuraAS = Cast<UAuraAttributeSet>(AttributeSet))
	{
		if (AbilitySystemComponent)
		{
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetHealthAttribute()).AddLambda(
				[this](const FOnAttributeChangeData& Data)
				{
					OnHealthChanged.Broadcast(Data.NewValue);
				}

			);
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetMaxHealthAttribute()).AddLambda(
				[this](const FOnAttributeChangeData& Data)
				{
					OnMaxHealthChanged.Broadcast(Data.NewValue);
				}

			);
			
			AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Effect_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject
			(	this,
				&AAuraEnemy::HitReactTagChanged   //通过GE_HitReact在给actor加Effect_HitReactTag 在这里注册事件 并且绑定到HitReactTagChanged（）函数上
			);
		}
		OnHealthChanged.Broadcast(AuraAS->GetHealth());
		OnMaxHealthChanged.Broadcast(AuraAS->GetMaxHealth());
	}
}
void AAuraEnemy::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
	AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), bHitReacting);
}


void AAuraEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this); //ASC 通过 InitAbilityActorInfo(OwnerActor, AvatarActor) 建立“身份绑定”

	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();


	if (HasAuthority())
	{
		InitialDefaultAttributes();
	}
}


int AAuraEnemy::GetPlayerLevel()
{
	return Level; 
}

void AAuraEnemy::InitialDefaultAttributes() const
{
	UAuraAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent); 
}
void AAuraEnemy::Die()
{
	SetLifeSpan(LifeSpan); 
	Super::Die(); 
}

