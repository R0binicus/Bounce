// Fill out your copyright notice in the Description page of Project Settings.

#include "EventDispatcher.h"

#pragma region Event Manager
// Core event manager code
UEventDispatcher* UEventDispatcher::EventManagerSingleton = nullptr;

UEventDispatcher::UEventDispatcher()
{
    EventManagerSingleton = this;
}

UEventDispatcher::~UEventDispatcher()
{
    EventManagerSingleton = nullptr;
}

UEventDispatcher* UEventDispatcher::GetEventManagerSingleton()
{
    return UEventDispatcher::EventManagerSingleton;
}
#pragma endregion Event Manager

#pragma region Example
// These are examples of how another script would use the event dispatcher
// Essentially imagine that they are in a script other than the EventManager
void UEventDispatcher::ExampleNoParamsHandler()
{
    UE_LOG(LogTemp, Log, TEXT("ExampleNoParamsHandler"));
}

void UEventDispatcher::ExampleOneParamsHandler(bool MyBool)
{
    UE_LOG(LogTemp, Log, TEXT("ExampleOneParamsHandler - %s"), *FString(MyBool ? "True" : "False"));
}

void UEventDispatcher::ExampleTwoParamsHandler(bool MyBool, float MyFloat)
{
    UE_LOG(LogTemp, Log, TEXT("ExampleTwoParamsHandler - %s %f"), *FString(MyBool ? "True" : "False"), MyFloat);
}

void UEventDispatcher::ExampleBindDispatchersCpp()
{
    UEventDispatcher* Singleton = GetEventManagerSingleton();
    Singleton->Event_ExampleNoParams.AddUniqueDynamic(Singleton, &UEventDispatcher::ExampleNoParamsHandler);
    Singleton->Event_ExampleOneParams.AddUniqueDynamic(Singleton, &UEventDispatcher::ExampleOneParamsHandler);
    Singleton->Event_ExampleTwoParams.AddUniqueDynamic(Singleton, &UEventDispatcher::ExampleTwoParamsHandler);
}

void UEventDispatcher::ExampleUnbindDispatchersCpp()
{
    UEventDispatcher* Singleton = GetEventManagerSingleton();
    Singleton->Event_ExampleNoParams.RemoveDynamic(Singleton, &UEventDispatcher::ExampleNoParamsHandler);
    Singleton->Event_ExampleOneParams.RemoveDynamic(Singleton, &UEventDispatcher::ExampleOneParamsHandler);
    Singleton->Event_ExampleTwoParams.RemoveDynamic(Singleton, &UEventDispatcher::ExampleTwoParamsHandler);
}

void UEventDispatcher::ExampleCallDispatchersCpp()
{
    UEventDispatcher* Singleton = GetEventManagerSingleton();

    // Trigger event dispatchers
    Singleton->Event_ExampleNoParams.Broadcast();
    Singleton->Event_ExampleOneParams.Broadcast(FMath::RandBool());
    Singleton->Event_ExampleTwoParams.Broadcast(FMath::RandBool(), FMath::RandRange(0.0f, 100.0f));
}
#pragma endregion Example