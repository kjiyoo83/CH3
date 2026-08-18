#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "CH3_Pawn.generated.h"

class UCapsuleComponent;
class USkeletalMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class SPARTAPROJECT_API ACH3_Pawn : public APawn
{
	GENERATED_BODY()

public:
	ACH3_Pawn();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UCapsuleComponent* CapsuleComp;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USkeletalMeshComponent* MeshComp;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USpringArmComponent* SpringArmComp;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UCameraComponent* CameraComp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputMappingContext* InputMappingContext;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* MoveAction;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* LookAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float MoveSpeed;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Look")
    float LookSensitivity;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Look")
    float MinPitch;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Look")
    float MaxPitch;

    float CurrentCameraPitch;


    UFUNCTION()
    void Move(const FInputActionValue& value);
    UFUNCTION()
    void Look(const FInputActionValue& value);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
