# Swipe Navigation Library for UWP

[![nuget](https://img.shields.io/nuget/v/SwipeNavigation.SwipeNavigator)](https://www.nuget.org/packages/SwipeNavigation.SwipeNavigator)
[![CI](https://github.com/roxk/SwipeNavigator/actions/workflows/ci.yaml/badge.svg)](https://github.com/roxk/SwipeNavigator/actions/workflows/ci.yaml)

A library for the long-awaited feature of swipe navigation. Built with beautiful composition animation and interaction tracker. It works with touch, but there is also system back event integration. Common key accelerators are also implemented:

- Alt + left

More key combos can be supported given enough evidence that they are widely used (PRs are welcome).

It is written in C++ like WinUI, so it's lightweight and fast. It works in all supported WinRT langauge projections that can use XAML.

## WinUI 3?
Coming soon™️. PRs are welcome. 

**Note**
If you want to submit a PR, note that the code base should be structured so that most of the difference between UWP and WinUI 3 versions are namespace of controls, etc, configured via pre-processor macros, aka side-by-side implementation. You can reference the implementation of side-by-side UWP and WinUI 3 support in [react native windows](https://github.com/microsoft/react-native-windows/tree/f3f26c6edcecb9e4fb9b9e35d12ef76854400de2).

## Usage

### Install the library

TODO: Nuget install instruction.

## Examples

### Backdrop Icon Indicator

TODO: Insert video/screenshot

A style that works best with `SlideNavigationTransitionInfo`. First, in _each_ of your page that would be in the back stack, add `SwipeNavigator` as the container for your content as follows:

```xaml
<!-- MyPage.xaml -->
<Page <!-- omitted -->
    xmlns:s="using:SwipeNavigation">                        <!-- Declare xmlns:s namespace at the root -->
    <s:SwipeNavigator Frame="{x:Bind Frame, Mode=OneWay}">  <!-- Bind to this page's Frame -->
        <!-- Your page content -->
    </s:SwipeNavigator>
</Page>
```

Then, in code, navigate to each page by setting transition:

```csharp
var transitionInfo = new FrameNavigationOptions()
{
    TransitionInfoOverride = new SlideNavigationTransitionInfo()
    {
        Effect = SlideNavigationTransitionEffect.FromRight,
    },
    IsNavigationStackEnabled = true,
};
frame.NavigateToType(typeof(MyPage), null, transitionInfo);
```

### Edge-like Overlay Icon Indicator

TODO: Insert video/screenshot

This style works best when you have other transitions that do not work well with the backdrop icon animation, or could not afford to modify each page.

Just set the frame as the content of `SwipeNavigator` like follows and you are set.

```xaml
<!-- Remember to declare xmlns:s namespace at the root -->
<s:SwipeNavigator
    IconMode="Overlay"                  <!-- Set icon mode to overlay -->
    Frame="{x:Bind frame, Mode=OneWay}">    <!-- Bind to the content frame -->
    <Frame x:Name="frame"/>
</s:SwipeNavigator>
```

You can also see SwipeNavigator/SwipeNavigatorTest for more samples.

## Getting Started

See [CONTRIBUTING.md](CONTRIBUTING.md).
