#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "MineItem.generated.h"

class USphereComponent;

UCLASS()
class SPARTAPROJECT_API AMineItem : public AItem
{
    GENERATED_BODY()

public:
    AMineItem();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mine")
    USphereComponent* ExplosionCollision;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mine")
    float ExplosionDelay;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mine")
    float ExplosionRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mine")
    float ExplosionDamage;

    bool bActivated;

    FTimerHandle ExplosionTimerHandle;

    virtual void ActivateItem(AActor* Activator) override;
    void Explode();
};