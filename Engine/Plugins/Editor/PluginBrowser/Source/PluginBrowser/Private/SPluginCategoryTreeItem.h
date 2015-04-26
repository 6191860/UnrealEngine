// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

/**
 * Represents a category in the plugin category tree
 */
class FPluginCategory
{
public:
	/** Parent category item or NULL if this is a root category */
	TWeakPtr<FPluginCategory> ParentCategory;

	/** Name of the category */
	FString Name;

	/** Display name of the category */
	FText DisplayName;

	/** Child categories */
	TArray<TSharedPtr<FPluginCategory>> SubCategories;

	/** Plugins in this category */
	TArray<IPlugin*> Plugins;

	/** Constructor for FPluginCategory */
	FPluginCategory(TSharedPtr<FPluginCategory> InParentCategory, const FString& InName, const FText& InDisplayName)
		: ParentCategory(InParentCategory),
		  Name(InName),
		  DisplayName(InDisplayName)
	{
	}
};

/**
 * Widget that represents a single category in the category tree view
 */
class SPluginCategoryTreeItem : public SCompoundWidget
{
	SLATE_BEGIN_ARGS( SPluginCategoryTreeItem )
	{
	}

	SLATE_END_ARGS()

	/** Widget constructor */
	void Construct(const FArguments& Args, const TSharedRef<FPluginCategory>& Item);

private:

	/** @return Gets the icon brush to use for this item's current state */
	const FSlateBrush* GetIconBrush() const;

private:

	/** The item we're representing the in tree */
	TSharedPtr<FPluginCategory> Category;
};

