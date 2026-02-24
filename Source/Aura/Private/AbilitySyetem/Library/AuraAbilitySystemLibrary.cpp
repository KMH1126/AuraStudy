// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySyetem/Library/AuraAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UI/WidgetController/AuraUserWidgetController.h"
#include "Player/APlayerState.h"
#include "HUD/AuraHUD.h"

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(
    const UObject* WorldContextObject)
{
    if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
    {
        if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD()))
        {
            AAPlayerState* PS = PC->GetPlayerState<AAPlayerState>();
            if (!PS) return nullptr;

            UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
            UAttributeSet* AS = PS->GetAttributeSet();

            const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
            return AuraHUD->GetOverlayController(WidgetControllerParams);
        }
    }

    return nullptr;
}

UAttributeWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenWidgetController(const UObject* WorldContextObject)
{
    if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
    {
        if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD()))
        {
            AAPlayerState* PS = PC->GetPlayerState<AAPlayerState>();
            if (!PS) return nullptr;

            UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
            UAttributeSet* AS = PS->GetAttributeSet();

            const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
            return AuraHUD->GetAttributeMenuController(WidgetControllerParams);
        }
    }
    return nullptr;
}

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
    AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
    if (!AuraGameMode)
    {
        return; 
    }

    AActor* AvatarActor = ASC->GetAvatarActor();

    UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);

    FCharacterClassDefaultInfo PrimayAttributeInfo = CharacterClassInfo->GetCharacterClassDefaultInfo(CharacterClass);
    
    FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext(); 
    ContextHandle.AddSourceObject(AvatarActor); 

    FGameplayEffectSpecHandle PrimarySpecHandle =ASC->MakeOutgoingSpec(PrimayAttributeInfo.PrimaryAttributeClass, Level, ContextHandle);
    ASC->ApplyGameplayEffectSpecToSelf(*PrimarySpecHandle.Data.Get());

   
    FGameplayEffectSpecHandle SecondSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributeClass, Level, ContextHandle);
    ASC->ApplyGameplayEffectSpecToSelf(*SecondSpecHandle.Data.Get());

    FGameplayEffectSpecHandle VitalSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributeClass, Level, ContextHandle);
    ASC->ApplyGameplayEffectSpecToSelf(*VitalSpecHandle.Data.Get());


}

void UAuraAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC)
{
    AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
    if (!AuraGameMode)
    {
        return;
    }
    UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);

    for (auto Ability : CharacterClassInfo->CommonAbilities)
    {
        FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, 1);
        ASC->GiveAbility(AbilitySpec); 
    }
   
}

UCharacterClassInfo* UAuraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
    AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
    if (!AuraGameMode)
    {
        return nullptr;
    }
    UCharacterClassInfo* CharacterClassInfo = AuraGameMode->CharacterClassInfo;
    return CharacterClassInfo; 
}

bool UAuraAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
    if (const FAuraGameplayEffectContext* AuraEffectContextHandle = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
    {
        return AuraEffectContextHandle->IsBlockedHit(); 
    } 
    return false; 
}

bool UAuraAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
    if (const FAuraGameplayEffectContext* AuraEffectContextHandle = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
    {
        return AuraEffectContextHandle->IsCriticalHit();
    }
    return false;
}

void UAuraAbilitySystemLibrary::SetIsBlockedHit(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit)
{
    if ( FAuraGameplayEffectContext* AuraEffectContextHandle = static_cast< FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
    {
         AuraEffectContextHandle->SetIsBlockedHit(bInIsBlockedHit);
    }
}

void UAuraAbilitySystemLibrary::SetIsCriticalHit(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit)
{
    if (FAuraGameplayEffectContext* AuraEffectContextHandle = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
    {
        AuraEffectContextHandle->SetIsCriticalHit(bInIsCriticalHit);
    }
}

