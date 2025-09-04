// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/HW09Pawn.h"

// Sets default values
AHW09Pawn::AHW09Pawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AHW09Pawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHW09Pawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}
