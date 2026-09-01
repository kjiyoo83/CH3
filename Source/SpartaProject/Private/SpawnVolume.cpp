#include "SpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Engine/DataTable.h"

ASpawnVolume::ASpawnVolume()
{
    PrimaryActorTick.bCanEverTick = false;

    Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
    SetRootComponent(Scene);

    SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
    SpawningBox->SetupAttachment(Scene);

    // 스폰 영역이므로 실제 물리 충돌은 사용하지 않음
    SpawningBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    ItemDataTable = nullptr;
}

FVector ASpawnVolume::GetRandomPointInVolume() const
{
    const FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
    const FVector BoxOrigin = SpawningBox->GetComponentLocation();

    return BoxOrigin + FVector(
        FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
        FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
        FMath::FRandRange(-BoxExtent.Z, BoxExtent.Z)
    );
}

FItemSpawnRow* ASpawnVolume::GetRandomItem() const
{
    if (!ItemDataTable)
    {
        return nullptr;
    }

    TArray<FItemSpawnRow*> AllRows;
    static const FString ContextString(TEXT("ItemSpawnContext"));

    ItemDataTable->GetAllRows(ContextString, AllRows);

    if (AllRows.IsEmpty())
    {
        return nullptr;
    }

    float TotalChance = 0.0f;

    for (const FItemSpawnRow* Row : AllRows)
    {
        if (Row)
        {
            TotalChance += Row->SpawnChance;
        }
    }

    if (TotalChance <= 0.0f)
    {
        return nullptr;
    }

    const float RandomValue = FMath::FRandRange(0.0f, TotalChance);
    float AccumulatedChance = 0.0f;

    for (FItemSpawnRow* Row : AllRows)
    {
        if (!Row)
        {
            continue;
        }

        AccumulatedChance += Row->SpawnChance;

        if (RandomValue <= AccumulatedChance)
        {
            return Row;
        }
    }

    return nullptr;
}

AActor* ASpawnVolume::SpawnItem(TSubclassOf<AActor> ItemClass)
{
    if (!ItemClass)
    {
        return nullptr;
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride =
        ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    return GetWorld()->SpawnActor<AActor>(
        ItemClass,
        GetRandomPointInVolume(),
        FRotator::ZeroRotator,
        SpawnParams
    );
}

AActor* ASpawnVolume::SpawnRandomItem()
{
    FItemSpawnRow* SelectedRow = GetRandomItem();

    if (!SelectedRow)
    {
        return nullptr;
    }

    return SpawnItem(SelectedRow->ItemClass);
}