#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CH3_PlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class UUserWidget;

UCLASS()
class SPARTAPROJECT_API ACH3_PlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    ACH3_PlayerController();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputMappingContext* InputMappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* MoveAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* JumpAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* LookAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* SprintAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    TSubclassOf<UUserWidget> HUDWidgetClass;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
    UUserWidget* HUDWidgetInstance;

    UFUNCTION(BlueprintPure, Category = "HUD")
    UUserWidget* GetHUDWidget() const;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
    TSubclassOf<UUserWidget> MainMenuWidgetClass;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Menu")
    UUserWidget* MainMenuWidgetInstance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Result")
    TSubclassOf<UUserWidget> GameResultWidgetClass;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Result")
    UUserWidget* GameResultWidgetInstance;

    UFUNCTION(BlueprintCallable, Category = "Result")
    void ShowGameResult(bool bSuccess);

    UFUNCTION(BlueprintCallable, Category = "Menu")
    void ReturnToMainMenu();


    UFUNCTION(BlueprintCallable, Category = "HUD")
    void ShowGameHUD();

    UFUNCTION(BlueprintCallable, Category = "Menu")
    void ShowMainMenu();

    UFUNCTION(BlueprintCallable, Category = "Menu")
    void StartGame();

    virtual void BeginPlay() override;
};