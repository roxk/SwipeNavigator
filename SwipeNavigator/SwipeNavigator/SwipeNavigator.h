#pragma once
#include "NavigationRequestedEventArgs.h"
#include "SwipeNavigator.g.h"
#include <optional>

//
//
namespace winrt::SwipeNavigation::implementation
{
    struct SwipeNavigator : SwipeNavigatorT<SwipeNavigator, wuci::IInteractionTrackerOwner>
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
        void OnApplyTemplate();
        void Frame(Windows::UI::Xaml::Controls::Frame const& frame);
        Windows::UI::Xaml::Controls::Frame Frame()
        {
            return mFrame;
        }
        void IconMode(SwipeNavigation::IconMode mode)
        {
            if (mode == IconMode())
            {
                return;
            }
            SetValue(IconModeProperty(), box_value(mode));
        }
        SwipeNavigation::IconMode IconMode()
        {
            return unbox_value<SwipeNavigation::IconMode>(GetValue(IconModeProperty()));
        }
        void BackNavigationMode(SwipeNavigation::NavigationMode mode)
        {
            if (mode == BackNavigationMode())
            {
                return;
            }
            SetValue(BackNavigationModeProperty(), box_value(mode));
        }
        SwipeNavigation::NavigationMode BackNavigationMode()
        {
            return unbox_value<SwipeNavigation::NavigationMode>(GetValue(BackNavigationModeProperty()));
        }
        void ForwardNavigationMode(SwipeNavigation::NavigationMode mode)
        {
            if (mode == ForwardNavigationMode())
            {
                return;
            }
            SetValue(ForwardNavigationModeProperty(), box_value(mode));
        }
        SwipeNavigation::NavigationMode ForwardNavigationMode()
        {
            return unbox_value<SwipeNavigation::NavigationMode>(GetValue(ForwardNavigationModeProperty()));
        }
        void CanGoBack(bool can)
        {
            SetValue(CanGoBackProperty(), box_value(can));
        }
        bool CanGoBack()
        {
            return unbox_value<bool>(GetValue(CanGoBackProperty()));
        }
        void CanGoForward(bool can)
        {
            SetValue(CanGoForwardProperty(), box_value(can));
        }
        bool CanGoForward()
        {
            return unbox_value<bool>(GetValue(CanGoForwardProperty()));
        }
        void RevealWidth(double width)
        {
            SetValue(RevealWidthProperty(), box_value(width));
        }
        double RevealWidth()
        {
            return unbox_value<double>(GetValue(RevealWidthProperty()));
        }
        event_token BackRequested(wf::TypedEventHandler<SwipeNavigation::SwipeNavigator, SwipeNavigation::NavigationRequestedEventArgs> const& handler)
        {
            return mBackRequested.add(handler);
        }
        void BackRequested(event_token token)
        {
            mBackRequested.remove(token);
        }
        event_token ForwardRequested(wf::TypedEventHandler<SwipeNavigation::SwipeNavigator, SwipeNavigation::NavigationRequestedEventArgs> const& handler)
        {
            return mForwardRequested.add(handler);
        }
        void ForwardRequested(event_token token)
        {
            mForwardRequested.remove(token);
        }
        static Windows::UI::Xaml::DependencyProperty IconModeProperty();
        static Windows::UI::Xaml::DependencyProperty BackNavigationModeProperty();
        static Windows::UI::Xaml::DependencyProperty ForwardNavigationModeProperty();
        static Windows::UI::Xaml::DependencyProperty CanGoBackProperty();
        static Windows::UI::Xaml::DependencyProperty CanGoForwardProperty();
        static Windows::UI::Xaml::DependencyProperty RevealWidthProperty();
        static Windows::UI::Xaml::Visibility BackDropIconVisibility(SwipeNavigation::IconMode mode) { return mode == SwipeNavigation::IconMode::BackDrop ? wux::Visibility::Visible : wux::Visibility::Collapsed; }
        static Windows::UI::Xaml::Visibility OverlayIconVisibility(SwipeNavigation::IconMode mode) { return mode == SwipeNavigation::IconMode::Overlay ? wux::Visibility::Visible : wux::Visibility::Collapsed; }
        static SwipeNavigation::SwipeNavigator Find(Windows::UI::Xaml::FrameworkElement const& element);
        void OnKeyboardAcceleratorInvoked(wuxi::KeyboardAcceleratorInvokedEventArgs const& args);
        void CustomAnimationStateEntered(wuci::InteractionTracker const& sender, wuci::InteractionTrackerCustomAnimationStateEnteredArgs const& args);
        void IdleStateEntered(wuci::InteractionTracker const& sender, wuci::InteractionTrackerIdleStateEnteredArgs const& args);
        void InertiaStateEntered(wuci::InteractionTracker const& sender, wuci::InteractionTrackerInertiaStateEnteredArgs const& args);
        void InteractingStateEntered(wuci::InteractionTracker const& sender, wuci::InteractionTrackerInteractingStateEnteredArgs const& args);
        void RequestIgnored(wuci::InteractionTracker const& sender, wuci::InteractionTrackerRequestIgnoredArgs const& args);
        void ValuesChanged(wuci::InteractionTracker const& sender, wuci::InteractionTrackerValuesChangedArgs const& args);
        void Close(bool withAnimation = true);
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
        static void OnIconModeChanged(IInspectable const& sender, wux::DependencyPropertyChangedEventArgs const& args);
        static void OnBackNavigationModeChanged(IInspectable const& sender, wux::DependencyPropertyChangedEventArgs const& args);
        static void OnForwardNavigationModeChanged(IInspectable const& sender, wux::DependencyPropertyChangedEventArgs const& args);
        static void OnRevealWidthChanged(IInspectable const& sender, wux::DependencyPropertyChangedEventArgs const& args);
        void TryUnregisterFrameCallbacks();
        void OnFrameCanGoBackChanged(IInspectable const& sender, wux::DependencyProperty const& args);
        void OnFrameCanGoForwardChanged(IInspectable const& sender, wux::DependencyProperty const& args);
        void Update();
        void UpdateCanNavigate();
        void UpdateIconMode();
        void UpdateRevealWidth();
        template<typename Func>
        void Navigate(event<wf::TypedEventHandler<SwipeNavigation::SwipeNavigator, SwipeNavigation::NavigationRequestedEventArgs>>& event, bool canNavigate, Func&& navigate);
        void GoBack();
        void GoForward();
        void ResetTrackerProperties(wuc::CompositionPropertySet const& properties);
        void OnLoaded(IInspectable const& sender, wux::RoutedEventArgs const& args);
        void OnSystemBackRequested(IInspectable const& sender, wu::Core::BackRequestedEventArgs const& args);
        void OnRootPointerPressed(IInspectable const& sender, wuxi::PointerRoutedEventArgs const& args);
        void OnRootSizeChanged(IInspectable const& sender, wux::SizeChangedEventArgs const& args);
        wuxc::Frame mFrame;
        event<wf::TypedEventHandler<SwipeNavigation::SwipeNavigator, SwipeNavigation::NavigationRequestedEventArgs>> mBackRequested;
        event<wf::TypedEventHandler<SwipeNavigation::SwipeNavigator, SwipeNavigation::NavigationRequestedEventArgs>> mForwardRequested;
    };
}
namespace winrt::SwipeNavigation::factory_implementation
{
    struct SwipeNavigator : SwipeNavigatorT<SwipeNavigator, implementation::SwipeNavigator>
    {
    };
}
