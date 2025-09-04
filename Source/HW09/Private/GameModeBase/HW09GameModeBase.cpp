// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeBase/HW09GameModeBase.h"

#include "GameState/HW09GameStateBase.h"
#include "Player/HW09PlayerController.h"
#include "Player/HW09PlayerState.h"

void AHW09GameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);
	AHW09PlayerController* H9PC = Cast<AHW09PlayerController>(NewPlayer);
	if (IsValid(H9PC))
	{
		H9PC->NotificationText = FText::FromString(TEXT("Connected to the game server"));
		AllPlayerControllers.Add(H9PC);
		GetGameState<AHW09GameStateBase>()->PlayerCount= AllPlayerControllers.Num() ;
		AHW09PlayerState* H9PS = H9PC->GetPlayerState<AHW09PlayerState>();
		if (IsValid(H9PS))
		{
			H9PS->PlayerName = TEXT("PLAYER ") + FString::FromInt(AllPlayerControllers.Num());
			H9PS->PlayerNum = AllPlayerControllers.Num();
			
			AHW09GameStateBase* H9GS = GetGameState<AHW09GameStateBase>();
			if (IsValid(H9GS))
			{
				H9GS->MulticastRPCBroadCastingLoginMessage(H9PS->PlayerName);
				if (!H9GS->bIsGameStart)
				{
					H9GS->SetPlayerTurn();
					H9GS->bIsGameStart = true;
				}
				H9GS->ShowPlayerTurn();
			}
		}
	}
}

void AHW09GameModeBase::BeginPlay()
{
	Super::BeginPlay();
	GetGameState<AHW09GameStateBase>()->SetSecretNumber(MakeRandomNumbers());
	UE_LOG(LogTemp, Warning, TEXT("Secret Number is %s"), *GetGameState<AHW09GameStateBase>()->GetSecretNumber());
}

FString AHW09GameModeBase::MakeRandomNumbers()
{
	TArray<int32> Numbers;
	for (int32 i =1 ; i<= 9 ; ++i)
	{
		Numbers.Add(i);
	}
	FString Result;
	for (int32 i =0 ; i <3 ; ++i)
	{
		int32 Index = FMath::RandRange(0, Numbers.Num()-1);
		Result.Append(FString::FromInt(Numbers[Index]));
		Numbers.RemoveAt(Index);
	}
	return Result;
}

FString AHW09GameModeBase::JudgeNumbers(FString InGuessNumber)
{
	int32 StrikeCount =0, BallCount =0;
	FString SecretNumber = GetGameState<AHW09GameStateBase>()->GetSecretNumber();
	for (int i=0; i<3; ++i)
	{
		if (InGuessNumber[i] == SecretNumber[i])
		{
			++StrikeCount;
		}
		else
		{
			FString GuessChar = FString::Printf(TEXT("%c"), InGuessNumber[i]);
			if (SecretNumber.Contains(GuessChar))
			{
				++BallCount;
			}
		}
	}
	if (StrikeCount == 0 && BallCount == 0)
	{
		return TEXT("OUT");
	}
	return FString::Printf(TEXT("%dS / %dB "), StrikeCount, BallCount);
}



bool AHW09GameModeBase::IsGuessNumber(FString InString, AHW09PlayerController* InPlayerController)
{
	bool bCanPlay = false;
	do
	{
		if (InString.Len() != 3 || GetGameState<AHW09GameStateBase>()->WhoseTurn != InPlayerController->GetPlayerState<AHW09PlayerState>()->PlayerNum)
		{
			return false;
		}
		bool bIsUnique = true;
		TSet<TCHAR> UniqueDigits;
		for (TCHAR C: InString)
		{
			if (FChar::IsDigit(C) == false || C == '0')
			{
				bIsUnique = false;
				break;
			}
			UniqueDigits.Add(C);
		}
		if (bIsUnique == false)
		{
			break;
		}
		bCanPlay = true;
	}
	while (false);
	return bCanPlay;
}

void AHW09GameModeBase::IncreaseGuessCount(AController* InController)
{
	AHW09PlayerController* H9PC = Cast<AHW09PlayerController>(InController);
	if (IsValid(H9PC))
	{
		++H9PC->GetPlayerState<AHW09PlayerState>()->CurrentGuessCount;
	}
}

void AHW09GameModeBase::PrintChatMessageString(AHW09PlayerController* InPlayerController, FString InChatMessageString)
{
	FString ChatMessageCache = InChatMessageString;
	int32 Index = ChatMessageCache.Len()-3;
	FString GuessNumString = ChatMessageCache.RightChop(Index);
	if (IsGuessNumber(GuessNumString,InPlayerController) == true && InPlayerController->GetPlayerState<AHW09PlayerState>()->CurrentGuessCount != 3)
	{
		IncreaseGuessCount(InPlayerController);
		FString JudgeResult = JudgeNumbers(GuessNumString);
		for (const auto& PC : AllPlayerControllers)
		{
			if (IsValid(PC))
			{
				FString CombinedString = FString::Printf(TEXT("%s : %s"), *InPlayerController->GetPlayerState<AHW09PlayerState>()->GetPlayerInfoString(), *ChatMessageCache);
				FString CombinedMessage = CombinedString + TEXT("->") + JudgeResult;
				PC->ClientRPCPrintMessageString(CombinedMessage);
			}
		}
		int StrikeCount = FCString::Atoi(*JudgeResult.Left(1));
		UE_LOG(LogTemp, Warning, TEXT("StrikeCount is %d"), StrikeCount);
		JudgeGame(InPlayerController, StrikeCount);
		GetGameState<AHW09GameStateBase>()->SetPlayerTurn();
		
	}
	else
	{
		for (const auto& PC : AllPlayerControllers)
		{                                                                                                                                                                                                                                                                                                                                                                                                
			if (IsValid(PC))
			{
				FString CombinedString = FString::Printf(TEXT("%s : %s"), *InPlayerController->GetPlayerState<AHW09PlayerState>()->PlayerName, *ChatMessageCache);
				PC->ClientRPCPrintMessageString(CombinedString);
			}
		}
	}
}

bool AHW09GameModeBase::IsDraw()
{
	bool bIsDrawTemp = true; 
	for (const auto& PlayerController : AllPlayerControllers)
	{
		if (IsValid(PlayerController))
		{
			AHW09PlayerState* H9PS = PlayerController->GetPlayerState<AHW09PlayerState>();
			if (IsValid(H9PS))
			{
				if (H9PS->CurrentGuessCount < H9PS->MaxGuessCount)
				{
					bIsDrawTemp = false;
					break;
				}
			}
		}
	}
	return bIsDrawTemp;
}

void AHW09GameModeBase::JudgeGame(AHW09PlayerController* InPlayerController, int32 StrikeCount)
{
	if (StrikeCount == 3)
	{
		AHW09PlayerState* H9PS = InPlayerController->GetPlayerState<AHW09PlayerState>();
		if (IsValid(H9PS))
		{
			for (const auto& PC : AllPlayerControllers)
			{
				FString WinMessage = FString::Printf(TEXT("%s %s"), *H9PS->PlayerName, TEXT("has won the game"));
				PC->ClientRPCPrintMessageString(WinMessage);
				ResetGame();
			}
		}
	}
	else
	{
		bIsDraw = IsDraw();
		
		if (bIsDraw == true)
		{
			for (const auto& PlayerController : AllPlayerControllers)
			{
				FString DrawMessage = TEXT("Draw...");
				PlayerController->ClientRPCPrintMessageString(DrawMessage);
				ResetGame();
			}
			bIsDraw = false;
		}
	}
}
void AHW09GameModeBase::ResetGame()
{
	GetGameState<AHW09GameStateBase>()->SetSecretNumber(MakeRandomNumbers());
	for (const auto& PC : AllPlayerControllers)
	{
		if (IsValid(PC))
		{
			PC->GetPlayerState<AHW09PlayerState>()->CurrentGuessCount = 0;
		}
	}
}


