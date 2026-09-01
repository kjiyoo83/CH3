#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "HealingItem.generated.h"

UCLASS()
class SPARTAPROJECT_API AHealingItem : public AItem
{
    GENERATED_BODY()

public:
    AHealingItem();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Healing")
    float HealAmount;

    virtual void ActivateItem(AActor* Activator) override;
};