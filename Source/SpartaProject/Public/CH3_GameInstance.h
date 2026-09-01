#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CH3_GameInstance.generated.h"

UCLASS()
class SPARTAPROJECT_API UCH3_GameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    UCH3_GameInstance();

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Game Data")
    int32 TotalScore;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Game Data")
    int32 CurrentLevelIndex;

    UFUNCTION(BlueprintCallable, Category = "Game Data")
    void AddToTotalScore(int32 Amount);

    UFUNCTION(BlueprintCallable, Category = "Game Data")
    void ResetGameData();


};