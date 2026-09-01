#include "Item.h"
#include "Components/SphereComponent.h"

//DEFINE_LOG_CATEGORY(LogSparta);

// Sets default values
AItem::AItem()
{
    PrimaryActorTick.bCanEverTick = true;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    SetRootComponent(SceneRoot);

    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
    CollisionComp->SetupAttachment(SceneRoot);
    CollisionComp->InitSphereRadius(100.0f);
    CollisionComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnItemOverlap);
    CollisionComp->SetGenerateOverlapEvents(true);

    StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    StaticMeshComp->SetupAttachment(CollisionComp);
    StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    //리플렉션 등록을 했기 때문에 코드상에서 필요X
    //static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("'/Game/Resources/Props/SM_Chair.SM_Chair'"));
    //if (MeshAsset.Succeeded())
    //{
    //    StaticMeshComp->SetStaticMesh(MeshAsset.Object);
    //}

    //static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(TEXT(" '/Game/Resources/Materials/M_Metal_Gold.M_Metal_Gold'"));
    //if (MaterialAsset.Succeeded())
    //{
    //    StaticMeshComp->SetMaterial(0, MaterialAsset.Object); //머테리얼은 하나만 쓰는 것이 아니기 때문에 0번 인덱스처럼 인덱스를 나타내야함
    //} 
    
    RotationSpeed = 90.0f;
}

//void AItem::PostInitializeComponents()
//{
//    Super::PostInitializeComponents();
//
//    UE_LOG(LogSparta, Warning, TEXT("%s PostInitializeComponents"), *GetName())
//}

void AItem::BeginPlay()
{
    Super::BeginPlay();

    //리플렉션 등록을 했기 때문에 코드상에서 필요X
    /* SetActorLocation(FVector(300.0f, 200.0f, 100.0f));
    SetActorRotation(FRotator(0.0f, 90.0f, 0.0f));
    SetActorScale3D(FVector(2.0f));*/

    //구조체로 한 번에 관리 가능
   /* FVector NewLocation(300.0f, 200.0f, 100.0f);
    FRotator NewRotator(0.0f, 90.0f, 0.0f);
    FVector NewScale(2.0f);

    FTransform NewTransform(NewRotator, NewLocation, NewScale); 

    SetActorTransform(NewTransform);*/
}

void AItem::OnItemOverlap(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->ActorHasTag(TEXT("Player")))
    {
        ActivateItem(OtherActor);
    }
}

void AItem::ActivateItem(AActor* Activator)
{
    OnItemPickedUp();
    DestroyItem();
}

FName AItem::GetItemType() const
{
    return ItemType;
}

void AItem::DestroyItem()
{
    Destroy();
}

void AItem::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!FMath::IsNearlyZero(RotationSpeed))
    {
        AddActorLocalRotation(FRotator(0.0f, RotationSpeed * DeltaTime, 0.0f));
    }
}

//void AItem::Destroyed()
//{
//    UE_LOG(LogSparta, Warning, TEXT("%s Destroyed"), *GetName())
//
//    Super::Destroyed();
//}
//
//void AItem::EndPlay(const EEndPlayReason::Type EndPlayReason)
//{
//    UE_LOG(LogSparta, Warning, TEXT("%s EndPlay"), *GetName())
//
//    Super::EndPlay(EndPlayReason);
//}

//void AItem::ResetActorPosition()
//{
//    SetActorLocation(FVector::ZeroVector);
//}

float AItem::GetRotationSpeed() const
{
    return RotationSpeed;
}
