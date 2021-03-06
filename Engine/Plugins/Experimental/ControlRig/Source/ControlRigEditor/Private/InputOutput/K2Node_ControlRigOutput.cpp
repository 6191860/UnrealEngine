// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "K2Node_ControlRigOutput.h"
#include "EdGraphSchema_K2.h"
#include "K2Node_VariableSet.h"
#include "K2Node_Event.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "KismetCompiler.h"
#include "BlueprintNodeSpawner.h"
#include "EditorCategoryUtils.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "ControlRig.h"
#include "Textures/SlateIcon.h"
#include "ControlRigField.h"

#define LOCTEXT_NAMESPACE "K2Node_ControlRigOutput"

UK2Node_ControlRigOutput::UK2Node_ControlRigOutput(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeTooltip = LOCTEXT("NodeTooltip", "Outputs animation parameters");
}

void UK2Node_ControlRigOutput::AllocateDefaultPins()
{
	Super::AllocateDefaultPins();

	const UEdGraphSchema_K2* Schema = GetDefault<UEdGraphSchema_K2>();

	TArray<TSharedRef<IControlRigField>> InputInfos = GetOutputVariableInfo(DisabledOutputs);

	for (const TSharedRef<IControlRigField>& InputInfo : InputInfos)
	{
		UEdGraphPin* InputPin = CreatePin(EGPD_Input, InputInfo->GetPinType(), InputInfo->GetPinString());
		Schema->SetPinDefaultValueBasedOnType(InputPin);
	}
}

FText UK2Node_ControlRigOutput::GetTooltipText() const
{
	return NodeTooltip;
}

FText UK2Node_ControlRigOutput::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("AnimationOutput_Title", "Output");
}

UK2Node_Event* UK2Node_ControlRigOutput::FindExistingEvaluateNode(UEdGraph* Graph)
{
	FMemberReference FunctionRef;
	FunctionRef.SetFromField<UFunction>(UControlRig::StaticClass()->FindFunctionByName(GET_FUNCTION_NAME_CHECKED(UControlRig, OnEvaluate)), true);

	TArray<UK2Node_Event*> Events;
	Graph->GetNodesOfClass<UK2Node_Event>(Events);

	for (UK2Node_Event* Event : Events)
	{
		if (Event->bInternalEvent && Event->EventReference.IsSameReference(FunctionRef))
		{
			return Event;
		}
	}

	return nullptr;
}

void UK2Node_ControlRigOutput::ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	Super::ExpandNode(CompilerContext, SourceGraph);

	if (CompilerContext.bIsFullCompile)
	{
		const UEdGraphSchema_K2* Schema = CompilerContext.GetSchema();

		// Add OnExecute event
		UK2Node_Event* Event = FindExistingEvaluateNode(SourceGraph);
		UEdGraphPin* ExecPath = nullptr;
		if (Event == nullptr)
		{
			// create new OnEvaluate entry point
			Event = CompilerContext.SpawnIntermediateEventNode<UK2Node_Event>(this, nullptr, SourceGraph);
			Event->bOverrideFunction = true;
			Event->bInternalEvent = true;
			UFunction* EventFunction = UControlRig::StaticClass()->FindFunctionByName(GET_FUNCTION_NAME_CHECKED(UControlRig, OnEvaluate));
			Event->EventReference.SetFromField<UFunction>(EventFunction, true);
			Event->AllocateDefaultPins();
			ExecPath = Event->FindPinChecked(Schema->PN_Then, EGPD_Output);
		}
		else
		{
			// trace existing exec path to find where to hook from
			ExecPath = Event->FindPinChecked(Schema->PN_Then, EGPD_Output);
			while (ExecPath->LinkedTo.Num() > 0)
			{
				ExecPath = ExecPath->LinkedTo[0]->GetOwningNode()->FindPinChecked(UEdGraphSchema_K2::PN_Then, EGPD_Output);
			}
		}

		TArray<UEdGraphPin*> OutputPins;
		TArray<TSharedRef<IControlRigField>> FieldInfo;
		GetOutputParameterPins(DisabledOutputs, OutputPins, FieldInfo);

		// Now expand pins according to the field type
		for (int32 PinIndex = 0; PinIndex < OutputPins.Num(); PinIndex++)
		{
			FieldInfo[PinIndex]->ExpandPin(GetControlRigClass(), CompilerContext, SourceGraph, this, OutputPins[PinIndex], nullptr, false, ExecPath);
		}

		BreakAllNodeLinks();

		// Remove all our pins
		while (Pins.Num() > 0)
		{
			RemovePinAt(0, EGPD_Input);
		}
	}
}

bool UK2Node_ControlRigOutput::IsCompatibleWithGraph(UEdGraph const* Graph) const
{
	UBlueprint* Blueprint = Cast<UBlueprint>(Graph->GetOuter());

	return Super::IsCompatibleWithGraph(Graph) && Blueprint && Blueprint->ParentClass && Blueprint->ParentClass->IsChildOf(UControlRig::StaticClass());
}

#undef LOCTEXT_NAMESPACE
