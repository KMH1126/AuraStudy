

#include "Character/AuraCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "Aura/Aura.h"
#include "AbilitySyetem/AuraAbilitySystemComponent.h"

AAuraCharacterBase::AAuraCharacterBase()
{
 	
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetGenerateOverlapEvents(false);

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon"); 
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket")); 
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision); 

	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
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

void AAuraCharacterBase::InitialDefaultAttributes() const
{
	ApplayEffectToSelf(PrimaryAttributes, 1.0f); 
	ApplayEffectToSelf(SecondaryAttributes, 1.0f);
	ApplayEffectToSelf(VitalAttributes, 1.0f); 
}

void AAuraCharacterBase::AddCharacterAbilities()
{
	UAuraAbilitySystemComponent* AuraASC =CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	if (!HasAuthority())
	{
		return; 
	}

	AuraASC->AddCharacterAbilities(StartUpAbilities); 
}

FVector AAuraCharacterBase::GetCombatSocketLocation()
{

	return Weapon->GetSocketLocation(WeaponTipName);
}
UAnimMontage* AAuraCharacterBase::GetHitReactMontage_Implementation()
{
	return HitMontage;
}

void AAuraCharacterBase::Die()
{
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true)); 
	MulticastHandleDeath(); 
}

void AAuraCharacterBase::MulticastHandleDeath_Implementation()
{
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Dissolve();
}

void AAuraCharacterBase::Dissolve()
{
	if (IsValid(DissovleMaterial))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(DissovleMaterial, this);
		GetMesh()->SetMaterial(0, DynamicMatInst); 
		StartTimeDissolve(DynamicMatInst);
	}
	if (IsValid(WeaponDissovleMaterial))
	{
		UMaterialInstanceDynamic* WeaponDynamicMatInst = UMaterialInstanceDynamic::Create(WeaponDissovleMaterial, this);
		Weapon->SetMaterial(0, WeaponDynamicMatInst);
		StartWeaponTimeDissolve(WeaponDynamicMatInst);
	}
}

