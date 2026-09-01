#include "CoinItem.h"
#include "CH3_GameState.h"

ACoinItem::ACoinItem()
{
    PointValue = 10;
    ItemType = TEXT("Coin");
}

void ACoinItem::ActivateItem(AActor* Activator)
{
    if (ACH3_GameState* GameState =
        GetWorld()->GetGameState<ACH3_GameState>())
    {
        GameState->AddScore(PointValue);
        GameState->OnCoinCollected();
    }

    OnItemPickedUp();
    DestroyItem();
}