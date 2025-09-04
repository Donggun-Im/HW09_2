// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HW09InputWidget.generated.h"

class UEditableTextBox;
/**
 * 
 */
UCLASS()
class HW09_API UHW09InputWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UEditableTextBox> ChatTextBox;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
protected:
	UFUNCTION()
	void OnChatInputTextCommitted(const FText& InText, ETextCommit::Type CommitMethod);
};