#include "MineItem.h"
#include "CH3_Character.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

AMineItem::AMineItem()
{
    ExplosionDelay = 0.0f;
    ExplosionRadius = 300.0f;
    ExplosionDamage = 20.0f;
    ItemType = TEXT("Mine");
    bActivated = false;

    ExplosionCollision =
        CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollision"));

    ExplosionCollision->SetupAttachment(SceneRoot);
    ExplosionCollision->InitSphereRadius(ExplosionRadius);
    ExplosionCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}

void AMineItem::ActivateItem(AActor* Activator)
{
    if (bActivated)
    {
        return;
    }

    bActivated = true;

    if (ACH3_Character* Character = Cast<ACH3_Character>(Activator))
    {
        UGameplayStatics::ApplyDamage(
            Character,
            ExplosionDamage,
            nullptr,
            this,
            UDamageType::StaticClass()
        );
    }

    OnItemPickedUp();
    DestroyItem();
}

void AMineItem::Explode()
{
    TArray<AActor*> OverlappingActors;
    ExplosionCollision->GetOverlappingActors(OverlappingActors);

    for (AActor* Actor : OverlappingActors)
    {
        if (ACH3_Character* Character = Cast<ACH3_Character>(Actor))
        {
            UGameplayStatics::ApplyDamage(
                Character,
                ExplosionDamage,
                nullptr,
                this,
                UDamageType::StaticClass()
            );
        }
    }

    DestroyItem();
}