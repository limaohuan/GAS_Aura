// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

AAuraPlayerController::AAuraPlayerController()
{
	// ���Ա�֤�������������ݺ������ͷ����������Ŀͻ��˵�����Ҳ���޸�
	bReplicates = true;
}

void AAuraPlayerController::BeginPlay()
{
    Super::BeginPlay();

    check(AuraContext);
    // ��ȡ������ҵ�Enhanced Input��ϵͳ
    UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

    check(Subsystem);
    // ��AuraContext����ӳ����������ӵ���ϵͳ�У����ȼ�Ϊ0
    Subsystem->AddMappingContext(AuraContext, 0);

    // ���������ɼ�
    bShowMouseCursor = true;
    DefaultMouseCursor = EMouseCursor::Default;

    // ������Ϸ��UI�������ģʽ
    FInputModeGameAndUI InputModeData;
    // ������겻�������ӿ���
    InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    // ���ò�������ʱ�����ع��
    InputModeData.SetHideCursorDuringCapture(false);
    // Ӧ������ģʽ����
    SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

    // �����뱻��������ʱ(Triggered)����Move����
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
    const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
    const FRotator Rotation = GetControlRotation();
    // ����һ��ֻ����Yaw��ˮƽ��ת������ת��������Pitch��Roll
    const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

    // ͨ����ת�����ȡǰ�����򣨻���YawRotation��X�ᣩ
    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    // ͨ����ת�����ȡ�ҷ��򣨻���YawRotation��Y�ᣩ
    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

    if (APawn* ControlledPawn = GetPawn<APawn>())
    {
        ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
        ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
    }
}
