// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySyetem/AbilityTasks/TargetDataUnderMouse.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwingAbility)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwingAbility);
	return MyObj;
}
void UTargetDataUnderMouse::Activate()
{
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();//谁在客户端？谁在服务器？
	if (bIsLocallyControlled)
	{
		SendMouseCursorData();
	}
	else
	{
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey).AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicatedCallback);
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, ActivationPredictionKey);
		if (!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
	}
}

void UTargetDataUnderMouse::SendMouseCursorData()
{
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());//我接下来要做一段‘客户端预测’操作，给它分配/启用一个 PredictionKey，让这次输入和之后的服务器确认能对上号

	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult CursorHit;
	PC->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit); //这一步只在客户端做。服务器上 PC 可能为空（dedicated server 根本没有鼠标和本地视口）。
	
	FGameplayAbilityTargetDataHandle DataHandle;																//一个“容器/箱子”，里面能装各种 TargetData。		
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();		//创建一种 TargetData 类型：单个命中点（里面就是一个 HitResult）。
	Data->HitResult = CursorHit;																				//把刚刚采样到的鼠标命中塞进去。
	DataHandle.Add(Data); 

	AbilitySystemComponent->ServerSetReplicatedTargetData(
							GetAbilitySpecHandle(), GetActivationPredictionKey(),
							DataHandle, FGameplayTag(), AbilitySystemComponent->ScopedPredictionKey);              //RPC 把 TargetData 发给服务器（真正的“传输”发生在这里）

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle); 
	}
}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag)
{
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}