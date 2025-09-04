// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "HW09PlayerState.generated.h"

/**
 * 
 */
UCLASS()
class HW09_API AHW09PlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	AHW09PlayerState();
	virtual void BeginPlay() override;
	FString GetPlayerInfoString();
	UPROPERTY(Replicated)
	FString PlayerName;
	UPROPERTY(Replicated)
	int32 PlayerNum =0;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	int32 MaxGuessCount =3 ;
	UPROPERTY(Replicated)
	int32 CurrentGuessCount =0;
};
