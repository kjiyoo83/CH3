#include "HealingItem.h"
#include "CH3_Character.h"

AHealingItem::AHealingItem()
{
    HealAmount = 10.0f;
    ItemType = TEXT("Healing");
}

void AHealingItem::ActivateItem(AActor* Activator)
{
    if (ACH3_Character* Character = Cast<ACH3_Character>(Activator))
    {
        Character->AddHealth(HealAmount);

        OnItemPickedUp();
        DestroyItem();
    }
}