# FilteredWidget

FilteredWidget is an Unreal Engine 4 plugin introducing UFilteredWidgetComponent, a customized UWidgetComponent with mipmap generation and filtering settings.
This solves aliasing / flickering in world-placed widgets, which is especially noticeable in VR.
Tested in UE 4.26.2 with Oculus Quest 2 (native), but should work with other engine versions and hardware too.

Blog post about this technique:
[Fixing aliasing / flickering widgets in UE4](https://hollowdilnik.com/2021/01/09/widget-filtering.html)

![Example](example.png)

## Installation
Put the `FilteredWidget` folder into the `Plugins` folder of your project and build the project.

## Usage
This plugin provides `UFilteredWidgetComponent` and its blueprint child `BP_FilteredWidgetComponent`.
Just use these instead of `UWidgetComponent` and enjoy smooth widgets.
Make sure you use the included materials since the default widget material overrides filtering settings!
Setting these widgets to manual redraw is strongly recommended.

## Caveats
The current solution is not ideal as it creates an additional throwaway render target every time the render target is created (typically only once, when the widget is first created, so it is unlikely to cause any problems).
On the plus side, this implementation avoids copying parts of the engine source code (which is against UE EULA) and should work on most engine versions without modifications.
See [this comment](https://github.com/hollowdilnik/filtered-widget/blob/main/FilteredWidget/Source/FilteredWidget/Private/FilteredWidgetComponent.cpp#L11) for a possible fix.

## Getting swizzled
There is a feature in UE 4.26 that generously swaps R and B channels after every step of mipmap generation.
If this causes problems, replace [this line](https://github.com/EpicGames/UnrealEngine/blob/a47b87395132f0454c285dc6ec488dece4d45c9c/Engine/Shaders/Private/ComputeGenerateMips.usf#L36) in the mipmap generation shader with `MipOutUAV[DT_ID.xy] = outColor;`.
Apparently it was introduced at some point to counteract R and B channels being swapped by Vulkan, but is no longer needed and, ironically, causes R and B channels to get swapped.
This got fixed in UE 4.27.

## GENMIPS_SRGB in UE 4.27
UE 4.27 no longer swizzles your mips, which is great news.
However, life would be too simple if you didn't have to modify your shader code again, so another generous feature was added.
UE 4.27 overrides GENMIPS_SRGB on Android Vulkan and forces it to false.
This causes the mips to be wrong on Oculus Quest devices, and the easiest way to fix that is to ignore GENMIPS_SRGB in the shader code, just like we did with GENMIPS_SWIZZLE in UE 4.26.

## Performance
This approach can get pretty heavy.
Generating mipmaps for fifteen 1k render targets costs me about 9ms on Oculus Quest 2 (0.6ms per widget).
Updating one widget every frame is probably fine if it is really needed, but anything more than that can quickly become too expensive.
If you need to update a lot of widgets, you should probably spread out the updates across multiple frames.
A couple of widgets with manual redraw are not going to be a big deal though.
