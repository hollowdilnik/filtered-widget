// Copyright 2021 Dmitry Molchanov and Julia Molchanova. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "FilteredWidgetComponent.generated.h"

/**
 * Customized widget component with mipmap generation and filtering
 */
UCLASS(Blueprintable, ClassGroup="UserInterface", hidecategories=(Object,Activation,"Components|Activation",Sockets,Base,Lighting,LOD,Mesh), editinlinenew, meta=(BlueprintSpawnableComponent) )
class FILTEREDWIDGET_API UFilteredWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
protected:
	virtual void DrawWidgetToRenderTarget(float DeltaTime) override;
	virtual void UpdateRenderTarget(FIntPoint DesiredRenderTargetSize) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Render Target")
	bool bAutoGenerateMips = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Render Target")
	TEnumAsByte<TextureFilter> Filter = TF_Trilinear;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Render Target")
	TEnumAsByte<TextureGroup> LODGroup = TEXTUREGROUP_RenderTarget;

public:
	UFUNCTION(BlueprintCallable, Category="Render Target")
	void UpdateRenderTargetParams(bool bNewAutoGenerateMips,
		TEnumAsByte<TextureFilter> NewFilter,
		TEnumAsByte<TextureGroup> NewLODGroup);
};
