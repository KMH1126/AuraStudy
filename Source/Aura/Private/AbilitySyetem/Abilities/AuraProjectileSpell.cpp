// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySyetem/Abilities/AuraProjectileSpell.h"


void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	
	
}

void UAuraProjectileSpell::SpawnProjectile(const FVector& TargetLocation)
{
	
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
		FRotator Rotation = (TargetLocation - SocketLocation).Rotation();
		FTransform SpawnTransform;
		

		SpawnTransform.SetLocation(SocketLocation);//起始点
		SpawnTransform.SetRotation(Rotation.Quaternion());
		

		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		//TODO: Give the Projectile a Gameplay Effect Spec for causing Damage.
		const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo()); //拿到发技能的actor的asc 也就是aura自己
		const FGameplayEffectSpecHandle DamageSpecHandle = SourceASC->MakeOutgoingSpec(DamageGameplayEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext()); 

		FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
		//从damageTypes拿到tag和对应的CurveTable 在此处读到蓝图里设置的tag和伤害值
		for (auto& Pair : DamageTypes)
		{
			const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, Pair.Key, ScaledDamage);
		}
		
		
		Projectile->DamageEffectSpecHandle = DamageSpecHandle;

		Projectile->FinishSpawning(SpawnTransform);
	}
}
