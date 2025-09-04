// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HW09PlayerController.generated.h"

/**
 * 
 */
class UHW09UserWidget;
class UHW09InputWidget;
UCLASS()
class HW09_API AHW09PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	AHW09PlayerController();
	UFUNCTION(Server, Reliable)
	void ServerRPCPrintMessageString(const FString& InChatMessageString);

	UFUNCTION(Client, Reliable)
	void ClientRPCPrintMessageString(const FString& InChatMessageString);
	UFUNCTION()
	void SetChatMessageString(const FString& InChatMessageString);
	void PrintMessageString(const FString& InChatMessageString);

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UHW09InputWidget> ChatUserWidgetClass;
	UPROPERTY()
	TObjectPtr<UHW09InputWidget> ChatUserWidgetInstance;

	FString ChatMessageString;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UHW09UserWidget> NotificationTextWidgetClass;
	UPROPERTY()
	TObjectPtr<UHW09UserWidget> NotificationTextWidgetInstance;
public:
	UPROPERTY(Replicated, BlueprintReadOnly)
	FText NotificationText;
};
