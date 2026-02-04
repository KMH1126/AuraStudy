// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystemComponent.h"
#include  "GameplayEffect.h"

#include "AuraEffectActor.generated.h"

UENUM(BlueprintType)
enum class EEffectApplicationPolicy : uint8
{
	ApplyOnOverlap,
	ApplyOnEndoverlap,
	DoNotApply
};
UENUM(BlueprintType)
enum class EEffectRemovalPolicy : uint8
{
	Remove0nEndoverlap,
	DoNotRemove
};
 
UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraEffectActor();

	
protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffect);

	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor); 

	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);



	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASC/Effects")
	bool bDestroyOnEffectRemoval = false; 

	UPROPERTY(EditAnywhere,BlueprintReadOnly ,  Category = "ASC/Effects")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASC/Effects")
	EEffectApplicationPolicy  InstantEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply; 
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASC/Effects")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASC/Effects")
	EEffectApplicationPolicy DurationEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASC/Effects")
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASC/Effects")
	EEffectRemovalPolicy InfiniteRemovealEffectPolicy = EEffectRemovalPolicy::DoNotRemove;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASC/Effects")
	EEffectApplicationPolicy InfiniteEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;


	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectHandles; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASC/Effects")
	float ActorLevel =1.f; 

};
