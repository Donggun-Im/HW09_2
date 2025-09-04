// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HW09GameModeBase.generated.h"
class AHW09PlayerController;
/**
 * @class AHW09GameModeBase
 * @brief Custom game mode base class for the HW09 project.
 *
 * Extends the functionality of the AGameModeBase class to provide
 * custom behavior for the game mode in the HW09 project.
 */
UCLASS()
class HW09_API AHW09GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	virtual void OnPostLogin(AController* NewPlayer) override;
	virtual void BeginPlay() override;
	void PrintChatMessageString(AHW09PlayerController* InPlayerController, FString InChatMessageString);
	UPROPERTY()
	TArray<TObjectPtr<AHW09PlayerController>> AllPlayerControllers;
	UFUNCTION()
	bool IsDraw();
	bool bIsDraw;
protected:
	FString MakeRandomNumbers();
	FString JudgeNumbers(FString InGuessNumber);
	void JudgeGame(AHW09PlayerController* InPlayerController,int32 StrikeCount);
	bool IsGuessNumber(FString InString, AHW09PlayerController* InPlayerController);
	void IncreaseGuessCount(AController* InController);
	void ResetGame();
	
};
