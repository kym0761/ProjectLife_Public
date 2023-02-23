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

	//World���� Ʈ�����ϱ⸦ ���ϴ� ���͸� �����ؼ� �־����.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger")
		TArray<TObjectPtr<AActor>> ToTrigger;

protected:

	//���� �� ���͸� Trigger�ϴ����� ToTrigger�� �ִ� ���Ͱ� BeginPlay()���� �߰���.
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
