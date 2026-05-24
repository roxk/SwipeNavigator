#pragma once

#include "ns.h"
#include <winrt/author/base.h>
#include <winrt/Windows.UI.Xaml.h>
#include <winrt/Windows.UI.Composition.Interactions.h>
#include "NavigationRequestedEventArgs.h"
#include <optional>

namespace winrt::SwipeNavigation
{
    struct SwipeNavigator;
    struct NavigationRequestedEventArgs;
}

namespace winrt::SwipeNavigation::author
{
    enum class IconMode : int
    {
        BackDrop,
        Overlay
    };

    enum class NavigationMode: int
    {
        Auto,
        Disabled,
        Enabled
    };

    struct SwipeNavigator : winrt::author::runtimeclass<wuxc::ContentControl, winrt::author::internal<wuci::IInteractionTrackerOwner>>
    {
        enum class State
        {
            Idle,
            BackCancelled,
            BackRequested,
            Back,
            ForwardCancelled,
            ForwardRequested,
            Forward
        };

        SwipeNavigator();
        ~SwipeNavigator();
        void InitializeComponent(winrt::author::ignore = {});
        void OnApplyTemplate(winrt::author::override = {});
        winrt::author::setter Frame(Windows::UI::Xaml::Controls::Frame const& frame);
        Windows::UI::Xaml::Controls::Frame Frame(winrt::author::getter = {});
        winrt::author::setter IconMode(author::IconMode mode);
        author::IconMode IconMode(winrt::author::getter = {});
        winrt::author::setter BackNavigationMode(author::NavigationMode mode);
        author::NavigationMode BackNavigationMode(winrt::author::getter = {});
        winrt::author::setter ForwardNavigationMode(author::NavigationMode mode);
        author::NavigationMode ForwardNavigationMode(winrt::author::getter = {});
        winrt::author::setter CanGoBack(bool can);
        bool CanGoBack(winrt::author::getter = {});
        winrt::author::setter CanGoForward(bool can);
        bool CanGoForward(winrt::author::getter = {});
        winrt::author::setter RevealWidth(double width);
        double RevealWidth(winrt::author::getter = {});
        event_token BackRequested(wf::TypedEventHandler<SwipeNavigation::SwipeNavigator, SwipeNavigation::NavigationRequestedEventArgs> const& handler);
        void BackRequested(event_token token);
        event_token ForwardRequested(wf::TypedEventHandler<SwipeNavigation::SwipeNavigator, SwipeNavigation::NavigationRequestedEventArgs> const& handler);
        void ForwardRequested(event_token token);
        static Windows::UI::Xaml::DependencyProperty IconModeProperty(winrt::author::getter = {});
        static Windows::UI::Xaml::DependencyProperty BackNavigationModeProperty(winrt::author::getter = {});
        static Windows::UI::Xaml::DependencyProperty ForwardNavigationModeProperty(winrt::author::getter = {});
        static Windows::UI::Xaml::DependencyProperty CanGoBackProperty(winrt::author::getter = {});
        static Windows::UI::Xaml::DependencyProperty CanGoForwardProperty(winrt::author::getter = {});
        static Windows::UI::Xaml::DependencyProperty RevealWidthProperty(winrt::author::getter = {});
        static Windows::UI::Xaml::Visibility BackDropIconVisibility(author::IconMode mode) { return mode == author::IconMode::BackDrop ? wux::Visibility::Visible : wux::Visibility::Collapsed; }
        static Windows::UI::Xaml::Visibility OverlayIconVisibility(author::IconMode mode) { return mode == author::IconMode::Overlay ? wux::Visibility::Visible : wux::Visibility::Collapsed; }
        static SwipeNavigation::SwipeNavigator Find(Windows::UI::Xaml::FrameworkElement const& element);
        void Close() { Close(true); }
        void Close(bool withAnimation);
        void OnKeyboardAcceleratorInvoked(wuxi::KeyboardAcceleratorInvokedEventArgs const& args, winrt::author::override = {});
        void CustomAnimationStateEntered(wuci::InteractionTracker const& sender, wuci::InteractionTrackerCustomAnimationStateEnteredArgs const& args, winrt::author::ignore = {});
        void IdleStateEntered(wuci::InteractionTracker const& sender, wuci::InteractionTrackerIdleStateEnteredArgs const& args, winrt::author::ignore = {});
        void InertiaStateEntered(wuci::InteractionTracker const& sender, wuci::InteractionTrackerInertiaStateEnteredArgs const& args, winrt::author::ignore = {});
        void InteractingStateEntered(wuci::InteractionTracker const& sender, wuci::InteractionTrackerInteractingStateEnteredArgs const& args, winrt::author::ignore = {});
        void RequestIgnored(wuci::InteractionTracker const& sender, wuci::InteractionTrackerRequestIgnoredArgs const& args, winrt::author::ignore = {});
        void ValuesChanged(wuci::InteractionTracker const& sender, wuci::InteractionTrackerValuesChangedArgs const& args, winrt::author::ignore = {});
    private:
        wuci::VisualInteractionSource mSource{ nullptr };
        wuci::InteractionTracker mTracker{ nullptr };
        wux::FrameworkElement mRoot{ nullptr };
        wuxm::RectangleGeometry mBackDropClip{ nullptr };
        wuxm::RectangleGeometry mOverlayClip{ nullptr };
        wuci::InteractionTrackerInertiaRestingValue mSnapToBack{ nullptr };
        wuci::InteractionTrackerInertiaRestingValue mSnapToForward{ nullptr };
        wuci::InteractionTrackerInertiaRestingValue mSnapToContent{ nullptr };
        State mState{ State::Idle };
        int64_t mFrameCanGoBackToken{};
        int64_t mFrameCanGoForwardToken{};
        static void OnIconModeChanged(wf::IInspectable const& sender, wux::DependencyPropertyChangedEventArgs const& args);
        static void OnBackNavigationModeChanged(wf::IInspectable const& sender, wux::DependencyPropertyChangedEventArgs const& args);
        static void OnForwardNavigationModeChanged(wf::IInspectable const& sender, wux::DependencyPropertyChangedEventArgs const& args);
        static void OnRevealWidthChanged(wf::IInspectable const& sender, wux::DependencyPropertyChangedEventArgs const& args);
        void TryUnregisterFrameCallbacks();
        void OnFrameCanGoBackChanged(wf::IInspectable const& sender, wux::DependencyProperty const& args);
        void OnFrameCanGoForwardChanged(wf::IInspectable const& sender, wux::DependencyProperty const& args);
        void Update();
        void UpdateCanNavigate();
        void UpdateIconMode();
        void UpdateRevealWidth();
        template<typename Func>
        void Navigate(event<wf::TypedEventHandler<SwipeNavigation::SwipeNavigator, SwipeNavigation::NavigationRequestedEventArgs>>& event, bool canNavigate, Func&& navigate);
        void GoBack();
        void GoForward();
        void ResetTrackerProperties(wuc::CompositionPropertySet const& properties);
        void OnLoaded(wf::IInspectable const& sender, wux::RoutedEventArgs const& args);
        void OnSystemBackRequested(wf::IInspectable const& sender, wu::Core::BackRequestedEventArgs const& args);
        void OnRootPointerPressed(wf::IInspectable const& sender, wuxi::PointerRoutedEventArgs const& args);
        void OnRootSizeChanged(wf::IInspectable const& sender, wux::SizeChangedEventArgs const& args);
        wuxc::Frame mFrame;
        event<wf::TypedEventHandler<SwipeNavigation::SwipeNavigator, SwipeNavigation::NavigationRequestedEventArgs>> mBackRequested;
        event<wf::TypedEventHandler<SwipeNavigation::SwipeNavigator, SwipeNavigation::NavigationRequestedEventArgs>> mForwardRequested;
    };
}
