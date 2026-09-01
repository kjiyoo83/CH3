#include "CH3_GameInstance.h"

UCH3_GameInstance::UCH3_GameInstance()
{
    TotalScore = 0;
    CurrentLevelIndex = 0;
}

void UCH3_GameInstance::AddToTotalScore(int32 Amount)
{
    TotalScore += Amount;

    UE_LOG(LogTemp, Log, TEXT("Total Score: %d"), TotalScore);
}

void UCH3_GameInstance::ResetGameData()
{
    TotalScore = 0;
    CurrentLevelIndex = 0;
}