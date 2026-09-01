#pragma once

#include "CoreMinimal.h"
#include "ItemInterface.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

//DECLARE_LOG_CATEGORY_EXTERN(LogSparta, Warning, All); //로그 카테고리 정하기
class USphereComponent;

UCLASS()
class SPARTAPROJECT_API AItem : public AActor, public IItemInterface
{
	GENERATED_BODY()
	
public:	
	AItem();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item|Components")
	USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Components")
	USphereComponent* CollisionComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Components")
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Properties")
	float RotationSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemType;

	//virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	//virtual void Destroyed() override;
	//virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void ActivateItem(AActor* Activator) override;
	virtual FName GetItemType() const override;

	//UFUNCTION(BlueprintCallable, Category = "Item|Actions")
	//void ResetActorPosition();

	UFUNCTION(BlueprintPure, Category = "Item|Properties")
	float GetRotationSpeed() const;

	void DestroyItem();

	UFUNCTION(BlueprintImplementableEvent, Category = "Item|Event")
	void OnItemPickedUp();

	UFUNCTION()
	void OnItemOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
};
