// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "HW09GameStateBase.generated.h"


/**
 * 
 */
UCLASS()
class HW09_API AHW09GameStateBase : public AGameStateBase
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;

	UPROPERTY()
	int32 WhoseTurn;
	FString GetSecretNumber()
	{
		return SecretNumber;
	};
	int32 GetWhoseTurn()
	{
		return WhoseTurn;
	};
	void SetSecretNumber(FString InSecretNumber)
	{
		SecretNumber = InSecretNumber;
	};
	void SetWhoseTurn(int32 InWhoseTurn)
	{
		WhoseTurn = InWhoseTurn;
	}
	UFUNCTION()
	int32 GetPlayerTurn() { return WhoseTurn; };
	UFUNCTION()
	void SetPlayerTurn();
	UFUNCTION()
	void ShowPlayerTurn();
	UFUNCTION()
	void MulticastRPCBroadCastingLoginMessage(FString InMessageString = FString(TEXT("XXXXX")));
	UPROPERTY()
	int32 PlayerCount =1;
	UPROPERTY()
	float TimePerTurn = 30.0f;
	UPROPERTY()
	bool bIsGameStart = false;
private:
	FString SecretNumber;
	
	FTimerHandle TurnTimerHandle;
	
};
