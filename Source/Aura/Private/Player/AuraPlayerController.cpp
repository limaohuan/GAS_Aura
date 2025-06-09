// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "Interaction/EnemyInterface.h"
#include "GameplayTagContainer.h"
#include "Input/AuraInputComponent.h"

AAuraPlayerController::AAuraPlayerController()
{
	// ���Ա�֤�������������ݺ������ͷ����������Ŀͻ��˵�����Ҳ���޸�
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);

    CursorTrace();
}

void AAuraPlayerController::CursorTrace()
{
    // ���ڴ洢���߼�����ı���
    FHitResult CursorHit;
    // �������λ�÷������߼�⣬ֻ���ɼ�ͨ��(ECC_Visibility)�Ķ���
    // false������ʾ�������κθ��ӵ���ײ���
    GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
    // ���û�������κ��赲����
    if (!CursorHit.bBlockingHit) return;

    LastActor = ThisActor;
    ThisActor = CursorHit.GetActor();

    if (LastActor == nullptr)
    {
        if (ThisActor != nullptr)
        {
            ThisActor->HighlightActor();
        }
    }
    else
    {
        if (ThisActor == nullptr)
        {
            LastActor->UnHighlightActor();
        }
        else
        {
            if (LastActor != ThisActor)
            {
                LastActor->UnHighlightActor();
                ThisActor->HighlightActor();
            }
        }
    }
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
    GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Red, *InputTag.ToString());
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
    GEngine->AddOnScreenDebugMessage(2, 3.f, FColor::Blue, *InputTag.ToString());
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
    GEngine->AddOnScreenDebugMessage(3, 3.f, FColor::Green, *InputTag.ToString());
}

void AAuraPlayerController::BeginPlay()
{
    Super::BeginPlay();

    check(AuraContext);
    // ��ȡ������ҵ�Enhanced Input��ϵͳ
    UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

    if (Subsystem)
    {
        // ��AuraContext����ӳ����������ӵ���ϵͳ�У����ȼ�Ϊ0
        Subsystem->AddMappingContext(AuraContext, 0);
    }
    
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

	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
    // �����뱻��������ʱ(Triggered)����Move����
    AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
    // ��AbliityActions��Pressed��Released��Held��Ϊ
    AuraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
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


