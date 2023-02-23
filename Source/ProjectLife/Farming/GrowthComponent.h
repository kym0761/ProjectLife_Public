// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GrowthComponent.generated.h"

class UStaticMesh;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTLIFE_API UGrowthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrowthComponent();

	//�ִ� Level
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Growth")
		int32 MaxGrowthLevel;

	//�۹� level�� �ʿ��� �� ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Growth")
		int32 GrowthLevelThreshold;

	//�۹� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Growth")
		int32 NumberOfHarvest;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Growth")
		TArray<TObjectPtr<UStaticMesh>> GrowthMeshs;

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Growth", Meta = (AllowPrivateAccess = "true"))
		bool bFruited;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Growth", Meta = (AllowPrivateAccess = "true"))
		int32 GrowthLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Growth", Meta = (AllowPrivateAccess = "true"))
		int32 CurrentGrowth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Growth", Meta = (AllowPrivateAccess = "true"))
		int32 Day;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Grow();
	void Fruit();

	void SetOwnerMesh();

	bool GetFruited() const;
	int32 GetGrowthLevel() const;
	int32 GetCurrentGrowth() const;
	int32 GetDay() const;

};
