// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HW09InputWidget.h"

#include "Components/EditableTextBox.h"
#include "Player/HW09PlayerController.h"

void UHW09InputWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (ChatTextBox->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted) == false)
	{
		ChatTextBox->OnTextCommitted.AddDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}
}

void UHW09InputWidget::NativeDestruct()
{
	Super::NativeDestruct();
	if (ChatTextBox->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted) == true)
	{
		ChatTextBox->OnTextCommitted.RemoveDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}
}

void UHW09InputWidget::OnChatInputTextCommitted(const FText& InText, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		AHW09PlayerController* PC = Cast<AHW09PlayerController>(GetOwningPlayer());
		if (IsValid(PC))
		{
			PC->SetChatMessageString(InText.ToString());
			ChatTextBox->SetText(FText::GetEmpty());
		}
	}
}
