#include "CH3_Pawn.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"


ACH3_Pawn::ACH3_Pawn()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComp->SetupAttachment(RootComponent);
	CapsuleComp->SetSimulatePhysics(false);

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	MeshComp->SetupAttachment(CapsuleComp);
	MeshComp->SetSimulatePhysics(false);

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(CapsuleComp);
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

	MoveSpeed = 600.f;
	LookSensitivity = 90.f;
	MinPitch = -70.f;
	MaxPitch = 40.f;

	CurrentCameraPitch = 0.f;
}

void ACH3_Pawn::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController())) // 폰 안에서는 GetLocalPlayer를 직접 호출할 수 없으므로 APlayerController를 한번 거침
	{
		if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				if (InputMappingContext)
				{
					Subsystem->AddMappingContext(InputMappingContext, 0);
				}
			}
		}
	}
	
}

void ACH3_Pawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACH3_Pawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent =
		Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(
			MoveAction,
			ETriggerEvent::Triggered,
			this,
			&ACH3_Pawn::Move
		);

		EnhancedInputComponent->BindAction(
			LookAction,
			ETriggerEvent::Triggered,
			this,
			&ACH3_Pawn::Look
		);
	}
}

void ACH3_Pawn::Move(const FInputActionValue& value)
{
	const FVector2D MoveInput = value.Get<FVector2D>();

	const float DeltaTime = GetWorld()->GetDeltaSeconds();

	const FVector LocalMove(
		MoveInput.Y * MoveSpeed * DeltaTime,
		MoveInput.X * MoveSpeed * DeltaTime,
		0.0f
	);

	AddActorLocalOffset(LocalMove, true);
}

void ACH3_Pawn::Look(const FInputActionValue& value)
{
	const FVector2D LookInput = value.Get<FVector2D>();

	const float DeltaTime = GetWorld()->GetDeltaSeconds();

	const float YawDelta = LookInput.X * LookSensitivity * DeltaTime;
	AddActorLocalRotation(FRotator(0.f, YawDelta, 0.f));
	CurrentCameraPitch += LookInput.Y * LookSensitivity * DeltaTime;
	CurrentCameraPitch = FMath::Clamp(CurrentCameraPitch, MinPitch, MaxPitch);
	SpringArmComp->SetRelativeRotation(FRotator(CurrentCameraPitch, 0.f, 0.f)
	);
}