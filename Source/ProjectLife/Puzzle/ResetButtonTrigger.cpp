// Fill out your copyright notice in the Description page of Project Settings.


#include "ResetButtonTrigger.h"
#include "PuzzleInterfaces.h"
#include "Components/SphereComponent.h"

AResetButtonTrigger::AResetButtonTrigger()
{

}

void AResetButtonTrigger::BeginPlay()
{
	//APuzzleTrigger�� BeginPlay()�� Call�Ǹ� �ȵ�.. ���� AActor�� BeginPlay()�� ȣ��Ǿ�� ��.
	//���� APuzzleTrigger�� BeginPlay()�� ȣ��Ǿ� ������, ���� ��ư�� FromTrigger�� ����ִ� �۾��� �ϰ� ��
	//�׷��� �ȴٸ�, ���� ��ư�� ������ �ʴ´ٸ� TriggerAction�� �۵����� �ʰ� �ɰ���.
	//�׷���, ���� ��ư�� Reset �۾��� �ϰ� �ǹǷ� �۵��� ���� �����.
	Super::Super::Super::BeginPlay();

	InitializeForTimeline();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AResetButtonTrigger::OnButtonBeginOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AResetButtonTrigger::OnButtonEndOverlap);
}

void AResetButtonTrigger::Tick(float DeltaTime)
{
	//ButtonTrigger�� ���� Tick()�� ���� Trigger() �� ���������� �����ϰ� �Ǿ� ����
	// Reset Button�� Tick()�� ���� Trigger()�� ���������� �۵��Ǿ�� �� ������ ����.
	//Call AActor's Tick();
	Super::Super::Super::Tick(DeltaTime);

	TimelineHandle.TickTimeline(DeltaTime);
}

void AResetButtonTrigger::ButtonUp()
{
	// �⺻ ��ư�� Trigger()�� ButtonUp()�� ���� �۵���. ���� ��ư�� �ʿ�����Ƿ� �ش� ����� �۵����� �ʵ��� �������̵���.
	
	bTriggerActive = false;
	ButtonMeshUp();
}

void AResetButtonTrigger::Trigger()
{
	//find Resetable Actor and Do Reset.
	for (AActor* i : ToTrigger)
	{
		bool bInterfaceValid = i->GetClass()->ImplementsInterface(UResetable::StaticClass());
		if (bInterfaceValid)
		{
			IResetable::Execute_Reset(i);
		}
	}
}