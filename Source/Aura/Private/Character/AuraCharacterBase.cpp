

#include "Character/AuraCharacterBase.h"

AAuraCharacterBase::AAuraCharacterBase()
{
 	
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon"); 
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket")); 
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision); 
}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

UAttributeSet* AAuraCharacterBase::GetAttributeSet() const
{
	return AttributeSet;
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
  return AbilitySystemComponent; 
}

