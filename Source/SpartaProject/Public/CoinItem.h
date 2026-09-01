#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "CoinItem.generated.h"

UCLASS()
class SPARTAPROJECT_API ACoinItem : public AItem
{
	GENERATED_BODY()

public:
    ACoinItem();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coin")
    int32 PointValue;

    virtual void ActivateItem(AActor* Activator) override;
	
};
