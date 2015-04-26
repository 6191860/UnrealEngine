// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

/**
 * Widget that represents a "tile" for a single plugin in our plugins list
 */
class SPluginListTile : public SCompoundWidget
{
	SLATE_BEGIN_ARGS( SPluginListTile )
	{
	}

	SLATE_END_ARGS()


	/** Widget constructor */
	void Construct( const FArguments& Args, const TSharedRef< class SPluginList > Owner, IPlugin* Plugin );


private:

	/** Returns the checked state for the enabled checkbox */
	ECheckBoxState IsPluginEnabled() const;

	/** Called when the enabled checkbox is clicked */
	void OnEnablePluginCheckboxChanged(ECheckBoxState NewCheckedState);

	/** Used to determine whether to show the edit and package buttons for this plugin */
	EVisibility GetAuthoringButtonsVisibility() const;

	/** Called when the 'edit' hyperlink is clicked */
	void OnEditPlugin();

	/** Called when the 'edit' hyperlink is clicked */
	FReply OnEditPluginFinished(UPluginMetadataObject* MetadataObject);

private:

	/** The item we're representing the in tree */
	IPlugin* Plugin;

	/** Weak pointer back to its owner */
	TWeakPtr< class SPluginList > OwnerWeak;

	/** Dialog for editing plugin properties */
	TSharedPtr< SWindow > PropertiesWindow;

	/** Brush resource for the image that is dynamically loaded */
	TSharedPtr< FSlateDynamicImageBrush > PluginIconDynamicImageBrush;
};

