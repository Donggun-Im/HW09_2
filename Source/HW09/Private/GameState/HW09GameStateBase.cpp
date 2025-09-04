// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/HW09GameStateBase.h"

#include "GameModeBase/HW09GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/HW09PlayerController.h"
#include "Player/HW09PlayerState.h"

void AHW09GameStateBase::BeginPlay()
{
	Super::BeginPlay();
	WhoseTurn =0 ;
}

void AHW09GameStateBase::SetPlayerTurn()
{
	GetWorldTimerManager().ClearTimer(TurnTimerHandle);
	
	if (WhoseTurn < PlayerCount)
	{
		++WhoseTurn;
	}
	else
	{
		WhoseTurn = 1;
	}
	AHW09GameModeBase* H9GM = Cast<AHW09GameModeBase>(UGameplayStatics::GetGameMode(this));
	AHW09PlayerState* H9PS = H9GM->AllPlayerControllers[WhoseTurn-1]->GetPlayerState<AHW09PlayerState>();
	if (H9PS->CurrentGuessCount == H9PS->MaxGuessCount)
	{
		SetPlayerTurn();
		return;
	}
	
	for (const auto& PC : H9GM->AllPlayerControllers)
	{
		if (IsValid(PC))
		{
			FString PlayerTurnString = FString::Printf(TEXT("Player %d's turn"), WhoseTurn);
			PC->NotificationText = FText::FromString(PlayerTurnString);
		}
	}
	GetWorldTimerManager().SetTimer(TurnTimerHandle, this, &AHW09GameStateBase::SetPlayerTurn , TimePerTurn, false);
}

void AHW09GameStateBase::ShowPlayerTurn()
{
	AHW09GameModeBase* H9GM= Cast<AHW09GameModeBase>(UGameplayStatics::GetGameMode(this));
	for (const auto& PC : H9GM->AllPlayerControllers)
	{
		if (IsValid(PC))
		{
			FString PlayerTurnString = FString::Printf(TEXT("Player %d's turn"), WhoseTurn);
			PC->NotificationText = FText::FromString(PlayerTurnString);
		}
	}
}

void AHW09GameStateBase::MulticastRPCBroadCastingLoginMessage(FString InMessageString)
{
	if (HasAuthority() == false)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (IsValid(PC))
		{
			AHW09PlayerController* H9PC = Cast<AHW09PlayerController>(PC);
			if (IsValid(H9PC))
			{
				FString NotificationString = InMessageString + TEXT(" Has joined the game");
				H9PC->ServerRPCPrintMessageString(NotificationString);
			}
		}
	}
}
