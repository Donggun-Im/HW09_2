// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/HW09PlayerState.h"
#include "Net/UnrealNetwork.h"
AHW09PlayerState::AHW09PlayerState()
{
	bReplicates = true;
}

void AHW09PlayerState::BeginPlay()
{
	Super::BeginPlay();
}

FString AHW09PlayerState::GetPlayerInfoString()
{
	FString Result = FString::Printf(TEXT("%s(%d/%d)"), *PlayerName,CurrentGuessCount,MaxGuessCount);
	return Result;
}

void AHW09PlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AHW09PlayerState, CurrentGuessCount);
	DOREPLIFETIME(AHW09PlayerState, PlayerName);
	DOREPLIFETIME(AHW09PlayerState, PlayerNum);
}
