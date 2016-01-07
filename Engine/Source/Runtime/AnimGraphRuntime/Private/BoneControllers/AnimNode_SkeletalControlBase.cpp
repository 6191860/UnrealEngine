// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "AnimGraphRuntimePrivatePCH.h"
#include "BoneControllers/AnimNode_SkeletalControlBase.h"
#include "AnimationUtils.h"
#include "AnimationRuntime.h"
#include "Animation/AnimInstanceProxy.h"

/////////////////////////////////////////////////////
// FAnimNode_SkeletalControlBase

void FAnimNode_SkeletalControlBase::Initialize(const FAnimationInitializeContext& Context)
{
	FAnimNode_Base::Initialize(Context);

	ComponentPose.Initialize(Context);
}

void FAnimNode_SkeletalControlBase::CacheBones(const FAnimationCacheBonesContext& Context) 
{
	InitializeBoneReferences(Context.AnimInstanceProxy->GetRequiredBones());
	ComponentPose.CacheBones(Context);
}

void FAnimNode_SkeletalControlBase::UpdateInternal(const FAnimationUpdateContext& Context)
{
}

void FAnimNode_SkeletalControlBase::Update(const FAnimationUpdateContext& Context)
{
	ComponentPose.Update(Context);

	if (IsLODEnabled(Context.AnimInstanceProxy, LODThreshold))
	{
		EvaluateGraphExposedInputs.Execute(Context);

		// Apply the skeletal control if it's valid
		const float ActualAlpha = AlphaScaleBias.ApplyTo(Alpha);
		if ((ActualAlpha >= ZERO_ANIMWEIGHT_THRESH) && IsValidToEvaluate(Context.AnimInstanceProxy->GetSkeleton(), Context.AnimInstanceProxy->GetRequiredBones()))
		{
			UpdateInternal(Context);
		}
	}
}

bool ContainsNaN(const TArray<FBoneTransform> & BoneTransforms)
{
	for (int32 i = 0; i < BoneTransforms.Num(); ++i)
	{
		if (BoneTransforms[i].Transform.ContainsNaN())
		{
			return true;
		}
	}

	return false;
}

void FAnimNode_SkeletalControlBase::EvaluateComponentSpace(FComponentSpacePoseContext& Output)
{
	// Evaluate the input
	ComponentPose.EvaluateComponentSpace(Output);

	if (IsLODEnabled(Output.AnimInstanceProxy, LODThreshold))
	{
		// Apply the skeletal control if it's valid
		const float ActualAlpha = AlphaScaleBias.ApplyTo(Alpha);
		if ((ActualAlpha >= ZERO_ANIMWEIGHT_THRESH) && IsValidToEvaluate(Output.AnimInstanceProxy->GetSkeleton(), Output.AnimInstanceProxy->GetRequiredBones()))
		{
			USkeletalMeshComponent* Component = Output.AnimInstanceProxy->GetSkelMeshComponent();

#if WITH_EDITORONLY_DATA
			// save current pose before applying skeletal control to compute the exact gizmo location in AnimGraphNode
			ForwardedPose = Output.Pose;
#endif // #if WITH_EDITORONLY_DATA

			BoneTransforms.Reset(BoneTransforms.Num());
			EvaluateBoneTransforms(Component, Output.Pose, BoneTransforms);

			checkSlow(!ContainsNaN(BoneTransforms));

			if (BoneTransforms.Num() > 0)
			{
				const float BlendWeight = FMath::Clamp<float>(ActualAlpha, 0.f, 1.f);
				Output.Pose.LocalBlendCSBoneTransforms(BoneTransforms, BlendWeight);
			}
		}
	}
}

void FAnimNode_SkeletalControlBase::AddDebugNodeData(FString& OutDebugData)
{
	const float ActualAlpha = AlphaScaleBias.ApplyTo(Alpha);
	OutDebugData += FString::Printf(TEXT("Alpha: %.1f%%"), ActualAlpha*100.f);
}