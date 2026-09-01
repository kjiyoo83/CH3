#include "CH3_GameState.h"
#include "CH3_GameInstance.h"
#include "CH3_Character.h"
#include "CH3_PlayerController.h"
#include "CoinItem.h"
#include "Kismet/GameplayStatics.h"
#include "SpawnVolume.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Blueprint/UserWidget.h"


ACH3_GameState::ACH3_GameState()
{
    Score = 0;
    SpawnedCoinCount = 0;
    CollectedCoinCount = 0;

    LevelDuration = 30.0f;
    CurrentLevelIndex = 0;

    LevelMapNames =
    {
        TEXT("BasicLevel"),
        TEXT("IntermediateLevel"),
        TEXT("AdvancedLevel")
    };

    bLevelEnding = false;
    bShowLevelAnnouncement = true;

    CurrentWaveIndex = 0;
    bWaveEnding = false;

    FWaveData Wave1;
    Wave1.TimeLimit = 60.0f;
    Wave1.ItemCount = 10;

    FWaveData Wave2;
    Wave2.TimeLimit = 45.0f;
    Wave2.ItemCount = 15;

    FWaveData Wave3;
    Wave3.TimeLimit = 30.0f;
    Wave3.ItemCount = 20;

    Waves.Add(Wave1);
    Waves.Add(Wave2);
    Waves.Add(Wave3);
}

void ACH3_GameState::BeginPlay()
{
    Super::BeginPlay();

    CurrentWaveIndex = 0;
    bShowLevelAnnouncement = true;

    StartCurrentWave();

    GetWorldTimerManager().SetTimer(
        HUDUpdateTimerHandle,
        this,
        &ACH3_GameState::UpdateHUD,
        0.1f,
        true
    );

    GetWorldTimerManager().SetTimer(
        LevelAnnouncementTimerHandle,
        this,
        &ACH3_GameState::HideLevelAnnouncement,
        3.0f,
        false
    );
}

int32 ACH3_GameState::GetScore() const
{
    return Score;
}

void ACH3_GameState::AddScore(int32 Amount)
{
    Score += Amount;

    if (UCH3_GameInstance* GameInstance =
        GetGameInstance<UCH3_GameInstance>())
    {
        GameInstance->AddToTotalScore(Amount);
    }

    UE_LOG(LogTemp, Log, TEXT("Level Score: %d"), Score);
}

void ACH3_GameState::StartCurrentWave()
{
    if (!Waves.IsValidIndex(CurrentWaveIndex))
    {
        EndLevel();
        return;
    }

    bWaveEnding = false;

    if (UCH3_GameInstance* GameInstance = GetGameInstance<UCH3_GameInstance>())
    {
        CurrentLevelIndex = GameInstance->CurrentLevelIndex;
    }

    SpawnedCoinCount = 0;
    CollectedCoinCount = 0;
    SpawnedItems.Empty();

    const FWaveData& CurrentWave = Waves[CurrentWaveIndex];

    TArray<AActor*> FoundVolumes;

    UGameplayStatics::GetAllActorsOfClass(
        GetWorld(),
        ASpawnVolume::StaticClass(),
        FoundVolumes
    );

    if (FoundVolumes.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("SpawnVolume이 없습니다."));
        return;
    }

    ASpawnVolume* SpawnVolume =
        Cast<ASpawnVolume>(FoundVolumes[0]);

    if (!SpawnVolume)
    {
        return;
    }

    for (int32 Index = 0; Index < CurrentWave.ItemCount; ++Index)
    {
        AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();

        if (!SpawnedActor)
        {
            continue;
        }

        SpawnedItems.Add(SpawnedActor);

        if (SpawnedActor->IsA(ACoinItem::StaticClass()))
        {
            SpawnedCoinCount++;
        }
    }

    GetWorldTimerManager().SetTimer(
        LevelTimerHandle,
        this,
        &ACH3_GameState::OnWaveTimeUp,
        CurrentWave.TimeLimit,
        false
    );

    UpdateHUD();

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("Level %d - Wave %d Start! Coin: %d"),
        CurrentLevelIndex + 1,
        CurrentWaveIndex + 1,
        SpawnedCoinCount
    );
}

void ACH3_GameState::OnCoinCollected()
{
    CollectedCoinCount++;

    UE_LOG(
        LogTemp,
        Log,
        TEXT("Coin: %d / %d"),
        CollectedCoinCount,
        SpawnedCoinCount
    );

    if (SpawnedCoinCount > 0 &&
        CollectedCoinCount >= SpawnedCoinCount)
    {
        EndCurrentWave();
    }
}

void ACH3_GameState::OnWaveTimeUp()
{
    OnGameOver();
}

void ACH3_GameState::EndLevel()
{
    if (bLevelEnding)
    {
        return;
    }

    bLevelEnding = true;

    GetWorldTimerManager().ClearTimer(LevelTimerHandle);
    GetWorldTimerManager().ClearTimer(NextWaveTimerHandle);

    UCH3_GameInstance* GameInstance =
        GetGameInstance<UCH3_GameInstance>();

    if (!GameInstance)
    {
        OnGameOver();
        return;
    }

    GameInstance->CurrentLevelIndex++;
    CurrentLevelIndex = GameInstance->CurrentLevelIndex;

    if (!LevelMapNames.IsValidIndex(CurrentLevelIndex))
    {
        if (ACH3_PlayerController* PlayerController =
            Cast<ACH3_PlayerController>(
                GetWorld()->GetFirstPlayerController()))
        {
            PlayerController->ShowGameResult(true);
        }

        return;
    }

    UGameplayStatics::OpenLevel(
        this,
        LevelMapNames[CurrentLevelIndex]
    );
}

void ACH3_GameState::OnGameOver()
{
    if (ACH3_PlayerController* PlayerController =
        Cast<ACH3_PlayerController>(
            GetWorld()->GetFirstPlayerController()))
    {
        PlayerController->ShowGameResult(false);
    }

    UE_LOG(LogTemp, Warning, TEXT("Game Over"));
}

void ACH3_GameState::UpdateHUD()
{
    if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
    {
        if (ACH3_PlayerController* CH3PlayerController = Cast<ACH3_PlayerController>(PlayerController))
        {
            if (UUserWidget* HUDWidget = CH3PlayerController->GetHUDWidget())
            {
                if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Time"))))
                {
                    float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
                    TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"), RemainingTime)));
                }

                if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Score"))))
                {
                    if (UGameInstance* GameInstance = GetGameInstance())
                    {
                        UCH3_GameInstance* CH3_GameInstance = Cast<UCH3_GameInstance>(GameInstance);
                        if (CH3_GameInstance)
                        {
                            ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), CH3_GameInstance->TotalScore)));
                        }
                    }
                }

                if (UTextBlock* CoinText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Coin"))))
                {
                    CoinText->SetText(FText::FromString(FString::Printf(
                        TEXT("Coin: %d / %d"),
                        CollectedCoinCount,
                        SpawnedCoinCount
                    )));
                }

                if (UProgressBar* HealthBar = Cast<UProgressBar>(HUDWidget->GetWidgetFromName(TEXT("HealthBar"))))
                {
                    if (ACH3_Character* Character = Cast<ACH3_Character>(PlayerController->GetPawn()))
                    {
                        const float HealthPercent = Character->GetHealth() / Character->GetMaxHealth();

                        HealthBar->SetPercent(HealthPercent);
                    }
                }

                if (UTextBlock* WaveText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Wave"))))
                {
                    WaveText->SetText(FText::FromString(FString::Printf(
                        TEXT("Wave: %d / %d"),
                        CurrentWaveIndex + 1,
                        Waves.Num()
                    )));
                }

                if (UTextBlock* LevelNoticeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("LevelNotice"))))
                {
                    LevelNoticeText->SetText(FText::FromString(FString::Printf(
                        TEXT("Level %d Start!"),
                        CurrentLevelIndex + 1
                    )));

                    LevelNoticeText->SetVisibility(
                        bShowLevelAnnouncement
                        ? ESlateVisibility::HitTestInvisible
                        : ESlateVisibility::Hidden
                    );
                }
            }
        }
    }
}

void ACH3_GameState::EndCurrentWave()
{
    if (bWaveEnding || bLevelEnding)
    {
        return;
    }

    bWaveEnding = true;

    GetWorldTimerManager().ClearTimer(LevelTimerHandle);

    for (TWeakObjectPtr<AActor> SpawnedItem : SpawnedItems)
    {
        if (SpawnedItem.IsValid())
        {
            SpawnedItem->Destroy();
        }
    }

    SpawnedItems.Empty();

    CurrentWaveIndex++;

    if (Waves.IsValidIndex(CurrentWaveIndex))
    {
        GetWorldTimerManager().SetTimer(
            NextWaveTimerHandle,
            this,
            &ACH3_GameState::StartCurrentWave,
            2.0f,
            false
        );

        return;
    }

    EndLevel();
}

void ACH3_GameState::HideLevelAnnouncement()
{
    bShowLevelAnnouncement = false;
}