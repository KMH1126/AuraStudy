

#include "Character/AuraCharacterBase.h"
#include "AbilitySystemComponent.h"

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
void AAuraCharacterBase::InitAbilityActorInfo()
{

}

void AAuraCharacterBase::ApplayEffectToSelf(TSubclassOf<UGameplayEffect> DefaultAttributes, float Level) const
{
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this); 
	FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(DefaultAttributes, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void AAuraCharacterBase::InitialDefaultAttributes()
{
	ApplayEffectToSelf(PrimaryAttributes, 1.0f); 
	ApplayEffectToSelf(SecondaryAttributes, 1.0f);
	ApplayEffectToSelf(VitalAttributes, 1.0f); 
}

