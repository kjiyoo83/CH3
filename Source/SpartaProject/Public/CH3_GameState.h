#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "CH3_GameState.generated.h"

USTRUCT(BlueprintType)
struct FWaveData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
    float TimeLimit = 30.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
    int32 ItemCount = 10;
};

UCLASS()
class SPARTAPROJECT_API ACH3_GameState : public AGameState
{
    GENERATED_BODY()

public:
    ACH3_GameState();

    virtual void BeginPlay() override;

    UFUNCTION(BlueprintPure, Category = "Score")
    int32 GetScore() const;

    UFUNCTION(BlueprintCallable, Category = "Score")
    void AddScore(int32 Amount);

    UFUNCTION(BlueprintCallable, Category = "Level")
    void OnCoinCollected();

    UFUNCTION(BlueprintCallable, Category = "Level")
    void OnGameOver();

    void UpdateHUD();


protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
    int32 Score;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
    int32 SpawnedCoinCount;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
    int32 CollectedCoinCount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
    float LevelDuration;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
    int32 CurrentLevelIndex;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
    TArray<FName> LevelMapNames;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
    TArray<FWaveData> Waves;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave")
    int32 CurrentWaveIndex;

    FTimerHandle NextWaveTimerHandle;
    FTimerHandle LevelAnnouncementTimerHandle;

    bool bShowLevelAnnouncement;

    void HideLevelAnnouncement();

    TArray<TWeakObjectPtr<AActor>> SpawnedItems;

    FTimerHandle LevelTimerHandle;
    FTimerHandle HUDUpdateTimerHandle;

    void StartCurrentWave();
    void EndCurrentWave();
    void OnWaveTimeUp();
    void EndLevel();

private:
    bool bLevelEnding;
    bool bWaveEnding;

};