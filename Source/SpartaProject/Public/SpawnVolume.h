#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemSpawnRow.h"
#include "SpawnVolume.generated.h"

class UBoxComponent;
class UDataTable;

UCLASS()
class SPARTAPROJECT_API ASpawnVolume : public AActor
{
    GENERATED_BODY()

public:
    ASpawnVolume();

    UFUNCTION(BlueprintCallable, Category = "Spawning")
    AActor* SpawnRandomItem();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
    USceneComponent* Scene;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
    UBoxComponent* SpawningBox;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
    UDataTable* ItemDataTable;

    UFUNCTION(BlueprintCallable, Category = "Spawning")
    AActor* SpawnItem(TSubclassOf<AActor> ItemClass);

private:
    FVector GetRandomPointInVolume() const;
    FItemSpawnRow* GetRandomItem() const;
    
};