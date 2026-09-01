#include "CH3_PlayerController.h"
#include "CH3_GameState.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Blueprint/UserWidget.h"
#include "CH3_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"

ACH3_PlayerController::ACH3_PlayerController()
    : InputMappingContext(nullptr),
    MoveAction(nullptr),
    JumpAction(nullptr),
    LookAction(nullptr),
    SprintAction(nullptr),
    HUDWidgetClass(nullptr),
    HUDWidgetInstance(nullptr),
    MainMenuWidgetClass(nullptr),
    MainMenuWidgetInstance(nullptr),
    GameResultWidgetClass(nullptr),
    GameResultWidgetInstance(nullptr)
{
}

void ACH3_PlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
        {
            if (InputMappingContext)
            {
                Subsystem->AddMappingContext(InputMappingContext, 0);
            }
        }
    }

    const FString CurrentMapName = GetWorld()->GetMapName();

    if (CurrentMapName.Contains(TEXT("MenuLevel")))
    {
        ShowMainMenu();
    }
    else
    {
        ShowGameHUD();
    }

}

UUserWidget* ACH3_PlayerController::GetHUDWidget() const
{
    return HUDWidgetInstance;
}

void ACH3_PlayerController::ShowMainMenu()
{
    if (HUDWidgetInstance)
    {
        HUDWidgetInstance->RemoveFromParent();
        HUDWidgetInstance = nullptr;
    }

    if (MainMenuWidgetInstance)
    {
        MainMenuWidgetInstance->RemoveFromParent();
        MainMenuWidgetInstance = nullptr;
    }

    if (MainMenuWidgetClass)
    {
        MainMenuWidgetInstance =
            CreateWidget<UUserWidget>(this, MainMenuWidgetClass);

        if (MainMenuWidgetInstance)
        {
            MainMenuWidgetInstance->AddToViewport();
        }
    }

    bShowMouseCursor = true;
    SetInputMode(FInputModeUIOnly());
}

void ACH3_PlayerController::ShowGameHUD()
{
    if (MainMenuWidgetInstance)
    {
        MainMenuWidgetInstance->RemoveFromParent();
        MainMenuWidgetInstance = nullptr;
    }

    if (!HUDWidgetInstance && HUDWidgetClass)
    {
        HUDWidgetInstance =
            CreateWidget<UUserWidget>(this, HUDWidgetClass);

        if (HUDWidgetInstance)
        {
            HUDWidgetInstance->AddToViewport();
        }
    }

    bShowMouseCursor = false;
    SetInputMode(FInputModeGameOnly());

    if (ACH3_GameState* GameState =
        GetWorld()->GetGameState<ACH3_GameState>())
    {
        GameState->UpdateHUD();
    }
}

void ACH3_PlayerController::StartGame()
{
    if (UCH3_GameInstance* GameInstance =
        GetGameInstance<UCH3_GameInstance>())
    {
        GameInstance->ResetGameData();
    }

    UGameplayStatics::OpenLevel(this, FName(TEXT("BasicLevel")));
}

void ACH3_PlayerController::ShowGameResult(bool bSuccess)
{
    if (HUDWidgetInstance)
    {
        HUDWidgetInstance->RemoveFromParent();
        HUDWidgetInstance = nullptr;
    }

    SetIgnoreMoveInput(true);
    SetIgnoreLookInput(true);

    if (APawn* PlayerPawn = GetPawn())
    {
        PlayerPawn->DisableInput(this);
    }

    SetPause(true);

    if (GameResultWidgetClass)
    {
        GameResultWidgetInstance =
            CreateWidget<UUserWidget>(this, GameResultWidgetClass);

        if (GameResultWidgetInstance)
        {
            GameResultWidgetInstance->AddToViewport();

            if (UTextBlock* ResultText = Cast<UTextBlock>(
                GameResultWidgetInstance->GetWidgetFromName(
                    TEXT("ResultText"))))
            {
                ResultText->SetText(FText::FromString(
                    bSuccess ? TEXT("Success!") : TEXT("Game Over")
                ));
            }
        }
    }

    bShowMouseCursor = true;
    SetInputMode(FInputModeUIOnly());
}

void ACH3_PlayerController::ReturnToMainMenu()
{
    SetPause(false);

    UGameplayStatics::OpenLevel(
        this,
        FName(TEXT("/Game/Maps/MenuLevel"))
    );
}