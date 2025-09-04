// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/HW09PlayerController.h"

#include "GameModeBase/HW09GameModeBase.h"
#include "Net/UnrealNetwork.h"
#include "UI/HW09UserWidget.h"
#include "UI/HW09InputWidget.h"
#include "HW09/HW09.h"
#include "Kismet/GameplayStatics.h"
#include "Player/HW09PlayerState.h"
void AHW09PlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (IsLocalController() == false)
	{
		return;
	}
	FInputModeUIOnly InputModeUIOnly;
	SetInputMode(InputModeUIOnly);
	if (IsValid(ChatUserWidgetClass))
	{
		ChatUserWidgetInstance = CreateWidget<UHW09InputWidget>(this, ChatUserWidgetClass);
		if (IsValid(ChatUserWidgetInstance))
		{
			ChatUserWidgetInstance->AddToViewport();
		}
	}
	if (IsValid(NotificationTextWidgetClass))
	{
		NotificationTextWidgetInstance = CreateWidget<UHW09UserWidget>(this, NotificationTextWidgetClass);
		if (IsValid(NotificationTextWidgetInstance))
		{
			NotificationTextWidgetInstance->AddToViewport();
		}
	}
}

AHW09PlayerController::AHW09PlayerController()
{
		bReplicates = true;
}

void AHW09PlayerController::ServerRPCPrintMessageString_Implementation(const FString& InChatMessageString)
{
	AGameModeBase* GM = UGameplayStatics::GetGameMode(this);
	if (IsValid(GM))
	{
		AHW09GameModeBase* H9GM = Cast<AHW09GameModeBase>(GM);
		if (IsValid(H9GM))
		{
			H9GM->PrintChatMessageString(this, InChatMessageString);
		}
	}
}

void AHW09PlayerController::ClientRPCPrintMessageString_Implementation(const FString& InChatMessageString)
{
	PrintMessageString(InChatMessageString);
}

void AHW09PlayerController::SetChatMessageString(const FString& InChatMessageString)
{
	AHW09PlayerState* H9PS = GetPlayerState<AHW09PlayerState>();
	if (IsLocalController() == true)
	{
		if (IsValid(ChatUserWidgetInstance))
		{
			ServerRPCPrintMessageString(InChatMessageString);
		}
	}
}

void AHW09PlayerController::PrintMessageString(const FString& InChatMessageString)
{
	HW09FunctionLibrary::MyPrintString(this, InChatMessageString, 10.0f);
}

void AHW09PlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, NotificationText);
}
