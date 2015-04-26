// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "SPluginCategoryTreeItem.h"

/**
 * Tree view that displays all of the plugin categories and allows the user to switch views
 */
class SPluginCategories : public SCompoundWidget
{

public:

	SLATE_BEGIN_ARGS( SPluginCategories )
	{
	}

	SLATE_END_ARGS()


	/** Widget constructor */
	void Construct( const FArguments& Args, const TSharedRef< class SPluginBrowser > Owner );

	/** Destructor */
	~SPluginCategories();

	/** @return Gets the owner of this categories tree */
	SPluginBrowser& GetOwner();

	/** @return Returns the currently selected category item */
	TSharedPtr<FPluginCategory> GetSelectedCategory() const;

	/** Selects the specified category */
	void SelectCategory(const TSharedPtr<FPluginCategory>& CategoryToSelect);

	/** @return Returns true if the specified item is currently expanded in the tree */
	bool IsItemExpanded(const TSharedPtr<FPluginCategory> Item) const;

	/** Signal that the categories list needs to be refreshed */
	void SetNeedsRefresh();

private:
	
	/** Called to generate a widget for the specified tree item */
	TSharedRef<ITableRow> PluginCategoryTreeView_OnGenerateRow(TSharedPtr<FPluginCategory> Item, const TSharedRef<STableViewBase>& OwnerTable);

	/** Given a tree item, fills an array of child items */
	void PluginCategoryTreeView_OnGetChildren(TSharedPtr<FPluginCategory> Item, TArray<TSharedPtr<FPluginCategory>>& OutChildren);

	/** Called when the user clicks on a plugin item, or when selection changes by some other means */
	void PluginCategoryTreeView_OnSelectionChanged(TSharedPtr<FPluginCategory> Item, ESelectInfo::Type SelectInfo);

	/** Rebuilds the category tree from scratch */
	void RebuildAndFilterCategoryTree();

	/** Callback for refreshing the tree */
	EActiveTimerReturnType TriggerCategoriesRefresh(double InCurrentTime, float InDeltaTime);

private:

	/** Weak pointer back to its owner */
	TWeakPtr< class SPluginBrowser > OwnerWeak;

	/** The tree view widget for our plugin categories tree */
	TSharedPtr<STreeView<TSharedPtr<FPluginCategory>>> PluginCategoryTreeView;

	/** Root list of categories */
	TArray<TSharedPtr<FPluginCategory>> RootPluginCategories;
};

