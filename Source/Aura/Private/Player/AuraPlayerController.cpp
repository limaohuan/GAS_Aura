// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "Interaction/EnemyInterface.h"
#include "GameplayTagContainer.h"
#include "Input/AuraInputComponent.h"

AAuraPlayerController::AAuraPlayerController()
{
	// 可以保证服务器更改数据后，其他和服务器相连的客户端的数据也被修改
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);

    CursorTrace();
}

void AAuraPlayerController::CursorTrace()
{
    // 用于存储射线检测结果的变量
    FHitResult CursorHit;
    // 从鼠标光标位置发射射线检测，只检测可见通道(ECC_Visibility)的对象
    // false参数表示不忽略任何复杂的碰撞检测
    GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
    // 如果没有命中任何阻挡物体
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
    // 获取本地玩家的Enhanced Input子系统
    UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

    if (Subsystem)
    {
        // 将AuraContext输入映射上下文添加到子系统中，优先级为0
        Subsystem->AddMappingContext(AuraContext, 0);
    }
    
    // 设置鼠标光标可见
    bShowMouseCursor = true;
    DefaultMouseCursor = EMouseCursor::Default;

    // 创建游戏和UI混合输入模式
    FInputModeGameAndUI InputModeData;
    // 设置鼠标不锁定在视口内
    InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    // 设置捕获输入时不隐藏光标
    InputModeData.SetHideCursorDuringCapture(false);
    // 应用输入模式设置
    SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
    // 当输入被持续触发时(Triggered)调用Move函数
    AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
    // 绑定AbliityActions的Pressed、Released、Held行为
    AuraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
    const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
    const FRotator Rotation = GetControlRotation();
    // 创建一个只包含Yaw（水平旋转）的旋转量，忽略Pitch和Roll
    const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

    // 通过旋转矩阵获取前进方向（基于YawRotation的X轴）
    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    // 通过旋转矩阵获取右方向（基于YawRotation的Y轴）
    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

    if (APawn* ControlledPawn = GetPawn<APawn>())
    {
        ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
        ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
    }
}


