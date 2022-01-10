// Copyright 2021 Dmitry Molchanov and Julia Molchanova. All Rights Reserved.


#include "FilteredWidgetComponent.h"

#include "Engine/TextureRenderTarget2D.h"
#include "Slate/WidgetRenderer.h"

void UFilteredWidgetComponent::UpdateRenderTarget(FIntPoint DesiredRenderTargetSize)
{
	/**
	 * Copying and modifying the source code of UWidgetComponent::UpdateRenderTarget
	 * is probably a cleaner way to do it and avoids the creation of a throwaway render target
	 * To do it this way, simply put these lines right after calling NewObject<UTextureRenderTarget2D> there
	 
		RenderTarget->bAutoGenerateMips = bAutoGenerateMips;
		RenderTarget->Filter = Filter;
		RenderTarget->LODGroup = LODGroup;
		
     * The following code should work on most engine versions, but is less efficient
     * (it relies on a throwaway RenderTarget, created in the parent class)
     * Should not be a big deal unless the draw size is changing every frame
	 */
	
	UTextureRenderTarget2D* OldRenderTarget = RenderTarget;
	Super::UpdateRenderTarget(DesiredRenderTargetSize);

	// If the render target was not created or was not changed, we don't need to make a new one
	if ( !RenderTarget || OldRenderTarget == RenderTarget )
	{
		return;
	}
	
	UTextureRenderTarget2D* NewRenderTarget = NewObject<UTextureRenderTarget2D>(this);
	
	// Add new exposed parameters
	NewRenderTarget->bAutoGenerateMips = bAutoGenerateMips;
	NewRenderTarget->Filter = Filter;
	NewRenderTarget->LODGroup = LODGroup;
	
	// Copy existing parameters
	NewRenderTarget->ClearColor = RenderTarget->ClearColor;
	NewRenderTarget->InitCustomFormat(DesiredRenderTargetSize.X, DesiredRenderTargetSize.Y,
	                                  RenderTarget->GetFormat(), false);

	RenderTarget = NewRenderTarget;
	
	if ( MaterialInstance )
	{
		MaterialInstance->SetTextureParameterValue("SlateUI", RenderTarget);
	}
}

void UFilteredWidgetComponent::DrawWidgetToRenderTarget(float DeltaTime)
{
	Super::DrawWidgetToRenderTarget(DeltaTime);
	
	if (RenderTarget && RenderTarget->bAutoGenerateMips)
	{
		RenderTarget->UpdateResourceImmediate(false);
	}
}

void UFilteredWidgetComponent::UpdateRenderTargetParams(bool bNewAutoGenerateMips, TEnumAsByte<TextureFilter> NewFilter,
	TEnumAsByte<TextureGroup> NewLODGroup)
{
	// If no settings have been changed, do nothing
	if (bNewAutoGenerateMips == bAutoGenerateMips &&
		NewFilter == Filter &&
		NewLODGroup == LODGroup)
	{
		return;
	}

	bAutoGenerateMips = bNewAutoGenerateMips;
	Filter = NewFilter;
	LODGroup = NewLODGroup;

	// Invalidate the current render target and create a new one
	RenderTarget = nullptr;
	UpdateRenderTarget(CurrentDrawSize);
}
