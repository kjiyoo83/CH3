// Fill out your copyright notice in the Description page of Project Settings.


#include "CH3_1.h"

// Sets default values
ACH3_1::ACH3_1()
{
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMech"));
	StaticMeshComp->SetupAttachment(SceneRoot);

	PrimaryActorTick.bCanEverTick = true;
	RotationSpeed = 90.0f;
	MoveSpeed = 1.0f;
	MaxRange = 300.0f;
	MoveDirection = FVector(0.0f, 1.0f, 0.0f);
	RunningTime = 0.0f;
}

// Called when the game starts or when spawned
void ACH3_1::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation();
}

// Called every frame
void ACH3_1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!FMath::IsNearlyZero(MoveSpeed))
	{
		RunningTime += DeltaTime * MoveSpeed;

		const float MoveDistance = FMath::Sin(RunningTime) * MaxRange;

		const FVector NewLocation =StartLocation + (MoveDirection.GetSafeNormal() * MoveDistance);

		SetActorLocation(NewLocation);
	}
}

