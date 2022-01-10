# FilteredWidget

FilteredWidget is an Unreal Engine 4 plugin introducing UFilteredWidgetComponent, a customized UWidgetComponent with mipmap generation and filtering settings. Tested in UE 4.26.2, but should work with other engine versions too.

Blog post about this technique:
[https://hollowdilnik.com/2021/01/09/widget-filtering.html](https://hollowdilnik.com/2021/01/09/widget-filtering.html)

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

## Performance
This approach can get pretty heavy.
Generating mipmaps for fifteen 1k render targets costs me about 9ms on Oculus Quest 2 (0.6ms per widget).
Updating one widget every frame is probably fine if it is really needed, but anything more than that can quickly become too expensive.
If you need to update a lot of widgets, you should probably spread out the updates across multiple frames.
A couple of widgets with manual redraw are not going to be a big deal though.
