// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuzzleTrigger.generated.h"

UCLASS()
class PROJECTLIFE_API APuzzleTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APuzzleTrigger();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Trigger")
		bool bTriggerActive;

	//World에서 트리거하기를 원하는 액터를 선택해서 넣어야함.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger")
		TArray<TObjectPtr<AActor>> ToTrigger;

protected:

	//누가 이 액터를 Trigger하는지는 ToTrigger에 있는 액터가 BeginPlay()에서 추가됨.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Trigger", Meta = (AllowPrivateAccess = "true"))
		TArray<TObjectPtr<APuzzleTrigger>> FromTrigger;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void InitializeTriggerArray();
};
