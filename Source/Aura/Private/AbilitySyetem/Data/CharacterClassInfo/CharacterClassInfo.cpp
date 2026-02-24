// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySyetem/Data/CharacterClassInfo/CharacterClassInfo.h"

FCharacterClassDefaultInfo UCharacterClassInfo::GetCharacterClassDefaultInfo(ECharacterClass CharacterClass)
{
	FCharacterClassDefaultInfo CharacterClassDefaultInfo = CharacterClassInformation.FindChecked(CharacterClass);
	return CharacterClassDefaultInfo;
}
