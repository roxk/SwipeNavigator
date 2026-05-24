#include "pch.h"
#include "Generated Files/idlgen.impl.h"
#include "SwipeNavigator.h"
#include <winrt/SwipeNavigation.h>
#include <winrt/Windows.UI.Xaml.Controls.h>
#include <chrono>
#include <algorithm>

using namespace winrt;

using namespace std::chrono_literals;

constexpr auto propIsClosing = L"isClosing";
constexpr auto propCanGoBack = L"canGoBack";
constexpr auto propCanGoForward = L"canGoForward";

template<typename T>
T GetTemplatedChild(wuxc::IControlProtected const& control, std::wstring_view name)
{
	auto child = control.GetTemplateChild(name);
	if (child == nullptr) return nullptr;
	return child.try_as<T>();
}

namespace winrt::SwipeNavigation::author
{
	SwipeNavigator::SwipeNavigator()
	{
	}
	SwipeNavigator::~SwipeNavigator()
	{
		TryUnregisterFrameCallbacks();
	}
	void SwipeNavigator::InitializeComponent(winrt::author::ignore)
	{
		auto impl = self(this);
		impl->DefaultStyleKey(winrt::box_value(xaml_typename<SwipeNavigation::SwipeNavigator>().Name));
		impl->Loaded({ impl->get_weak(), &SwipeNavigator::OnLoaded });
	}
	void SwipeNavigator::OnApplyTemplate(winrt::author::override)
	{
		auto impl = self(this);
		auto compositor = wuxh::ElementCompositionPreview::GetElementVisual(*impl).Compositor();
		auto tracker = wuci::InteractionTracker::CreateWithOwner(compositor, *impl);
		mTracker = tracker;
		auto root = GetTemplatedChild<wux::FrameworkElement>(*impl, L"root");
		mRoot = root;
		auto uiElement = impl->try_as<wux::UIElement>();
		root.AddHandler(wux::UIElement::PointerPressedEvent(), box_value(wuxi::PointerEventHandler{ impl, &SwipeNavigator::OnRootPointerPressed }), true);
		root.SizeChanged({ impl->get_weak(), &SwipeNavigator::OnRootSizeChanged });
		auto backDropClip = GetTemplatedChild<wuxm::RectangleGeometry>(*impl, L"backDropClip");
		mBackDropClip = backDropClip;
		auto overlayClip = GetTemplatedChild<wuxm::RectangleGeometry>(*impl, L"overlayClip");
		mOverlayClip = overlayClip;
		auto rootVisual = wuxh::ElementCompositionPreview::GetElementVisual(root);
		auto source = wuci::VisualInteractionSource::Create(rootVisual);
		mSource = source;
		source.ManipulationRedirectionMode(wuci::VisualInteractionSourceRedirectionMode::CapableTouchpadOnly);
		source.PositionYSourceMode(wuci::InteractionSourceMode::Disabled);
		source.PositionXChainingMode(wuci::InteractionChainingMode::Never);
		source.PositionYChainingMode(wuci::InteractionChainingMode::Never);
		tracker.InteractionSources().Add(source);
		mSnapToBack = wuci::InteractionTrackerInertiaRestingValue::Create(compositor);
		mSnapToForward = wuci::InteractionTrackerInertiaRestingValue::Create(compositor);
		mSnapToContent = wuci::InteractionTrackerInertiaRestingValue::Create(compositor);
		// Anim
		auto content = GetTemplatedChild<wux::UIElement>(*impl, L"content");
		auto contentVisual = wuxh::ElementCompositionPreview::GetElementVisual(content);
		// Virtual content visual is used as an intermediate variable for overlay animation.
		auto virtualContentVisual = compositor.CreateSpriteVisual();
		wuxh::ElementCompositionPreview::SetElementChildVisual(content, virtualContentVisual);
		auto backIcon = GetTemplatedChild<wux::UIElement>(*impl, L"backIcon");
		auto backIconVisual = wuxh::ElementCompositionPreview::GetElementVisual(backIcon);
		auto backIconAnim = compositor.CreateExpressionAnimation(L"(content.Offset.X - backIcon.Size.X) * 0.5");
		backIconAnim.SetReferenceParameter(L"content", contentVisual);
		backIconAnim.SetReferenceParameter(L"backIcon", backIconVisual);
		backIconVisual.StartAnimation(L"Offset.X", backIconAnim);
		auto forwardIcon = GetTemplatedChild<wux::UIElement>(*impl, L"forwardIcon");
		auto forwardIconVisual = wuxh::ElementCompositionPreview::GetElementVisual(forwardIcon);
		auto forwardIconAnim = compositor.CreateExpressionAnimation(L"content.Size.X + ((content.Offset.X - forwardIcon.Size.X) * 0.5)");
		forwardIconAnim.SetReferenceParameter(L"content", contentVisual);
		forwardIconAnim.SetReferenceParameter(L"forwardIcon", forwardIconVisual);
		forwardIconVisual.StartAnimation(L"Offset.X", forwardIconAnim);
		// Icon clip
		auto backIconClip = compositor.CreateInsetClip();
		backIconVisual.Clip(backIconClip);
		auto backClipAnim = compositor.CreateExpressionAnimation(L"(backIcon.Size.X * 0.5) - content.Offset.X");
		backClipAnim.SetReferenceParameter(L"backIcon", backIconVisual);
		backClipAnim.SetReferenceParameter(L"content", contentVisual);
		backIconClip.StartAnimation(L"RightInset", backClipAnim);
		auto forwardIconClip = compositor.CreateInsetClip();
		forwardIconVisual.Clip(forwardIconClip);
		auto forwardClipAnim = compositor.CreateExpressionAnimation(L"(forwardIcon.Size.X * 0.5) + content.Offset.X");
		forwardClipAnim.SetReferenceParameter(L"forwardIcon", forwardIconVisual);
		forwardClipAnim.SetReferenceParameter(L"content", contentVisual);
		forwardIconClip.StartAnimation(L"LeftInset", forwardClipAnim);
		// Setup
		Update();
		// Input
		wuxi::KeyboardAccelerator altLeft;
		altLeft.Key(ws::VirtualKey::Left);
		altLeft.Modifiers(ws::VirtualKeyModifiers::Menu);
		impl->KeyboardAccelerators().Append(altLeft);
		impl->KeyboardAcceleratorPlacementMode(wuxi::KeyboardAcceleratorPlacementMode::Hidden);
		wu::Core::SystemNavigationManager::GetForCurrentView().BackRequested({ self(this)->get_weak(), &SwipeNavigator::OnSystemBackRequested });
	}
	winrt::author::setter SwipeNavigator::Frame(wuxc::Frame const& frame)
	{
		TryUnregisterFrameCallbacks();
		mFrame = frame;
		auto impl = self(this);
		mFrameCanGoBackToken = frame.RegisterPropertyChangedCallback(wuxc::Frame::CanGoBackProperty(), { impl->get_weak(), &SwipeNavigator::OnFrameCanGoBackChanged });
		mFrameCanGoForwardToken = frame.RegisterPropertyChangedCallback(wuxc::Frame::CanGoForwardProperty(), { impl->get_weak(), &SwipeNavigator::OnFrameCanGoForwardChanged });
		return {};
	}
	Windows::UI::Xaml::Controls::Frame SwipeNavigator::Frame(winrt::author::getter)
	{
		return mFrame;
	}
	winrt::author::setter SwipeNavigator::IconMode(author::IconMode mode)
	{
		if (mode == IconMode())
		{
			return {};
		}
		self(this)->SetValue(IconModeProperty(), box_value(std::bit_cast<SwipeNavigation::IconMode>(mode)));
		return {};
	}
	author::IconMode SwipeNavigator::IconMode(winrt::author::getter)
	{
		return std::bit_cast<author::IconMode>(unbox_value<SwipeNavigation::IconMode>(self(this)->GetValue(IconModeProperty())));
	}
	winrt::author::setter SwipeNavigator::BackNavigationMode(author::NavigationMode mode)
	{
		if (mode == BackNavigationMode())
		{
			return {};
		}
		self(this)->SetValue(BackNavigationModeProperty(), box_value(std::bit_cast<SwipeNavigation::NavigationMode>(mode)));
		return {};
	}
	author::NavigationMode SwipeNavigator::BackNavigationMode(winrt::author::getter)
	{
		return std::bit_cast<author::NavigationMode>(unbox_value<SwipeNavigation::NavigationMode>(self(this)->GetValue(BackNavigationModeProperty())));
	}
	winrt::author::setter SwipeNavigator::ForwardNavigationMode(author::NavigationMode mode)
	{
		if (mode == ForwardNavigationMode())
		{
			return {};
		}
		self(this)->SetValue(ForwardNavigationModeProperty(), box_value(std::bit_cast<SwipeNavigation::NavigationMode>(mode)));
		return {};
	}
	author::NavigationMode SwipeNavigator::ForwardNavigationMode(winrt::author::getter)
	{
		return std::bit_cast<author::NavigationMode>(unbox_value<SwipeNavigation::NavigationMode>(self(this)->GetValue(ForwardNavigationModeProperty())));
	}
	winrt::author::setter SwipeNavigator::CanGoBack(bool can)
	{
		self(this)->SetValue(CanGoBackProperty(), box_value(can));
		return {};
	}
	bool SwipeNavigator::CanGoBack(winrt::author::getter)
	{
		return unbox_value<bool>(self(this)->GetValue(CanGoBackProperty()));
	}
	winrt::author::setter SwipeNavigator::CanGoForward(bool can)
	{
		self(this)->SetValue(CanGoForwardProperty(), box_value(can));
		return {};
	}
	bool SwipeNavigator::CanGoForward(winrt::author::getter)
	{
		return unbox_value<bool>(self(this)->GetValue(CanGoForwardProperty()));
	}
	winrt::author::setter SwipeNavigator::RevealWidth(double width)
	{
		self(this)->SetValue(RevealWidthProperty(), box_value(width));
		return {};
	}
	double SwipeNavigator::RevealWidth(winrt::author::getter)
	{
		return unbox_value<double>(self(this)->GetValue(RevealWidthProperty()));
	}
	event_token SwipeNavigator::BackRequested(wf::TypedEventHandler<SwipeNavigation::SwipeNavigator, SwipeNavigation::NavigationRequestedEventArgs> const& handler)
	{
		return mBackRequested.add(handler);
	}
	void SwipeNavigator::BackRequested(event_token token)
	{
		mBackRequested.remove(token);
	}
	event_token SwipeNavigator::ForwardRequested(wf::TypedEventHandler<SwipeNavigation::SwipeNavigator, SwipeNavigation::NavigationRequestedEventArgs> const& handler)
	{
		return mForwardRequested.add(handler);
	}
	void SwipeNavigator::ForwardRequested(event_token token)
	{
		mForwardRequested.remove(token);
	}
	void SwipeNavigator::OnIconModeChanged(wf::IInspectable const& sender, wux::DependencyPropertyChangedEventArgs const& args)
	{
		auto instance = sender.try_as<implementation::SwipeNavigator>();
		if (instance == nullptr)
		{
			return;
		}
		if (args.OldValue().try_as<SwipeNavigation::IconMode>() == args.NewValue().try_as<SwipeNavigation::IconMode>())
		{
			return;
		}
		instance->UpdateIconMode();
	}
	void SwipeNavigator::OnBackNavigationModeChanged(wf::IInspectable const& sender, wux::DependencyPropertyChangedEventArgs const& args)
	{
		auto instance = sender.try_as<implementation::SwipeNavigator>();
		if (instance == nullptr)
		{
			return;
		}
		instance->UpdateCanNavigate();
	}
	void SwipeNavigator::OnForwardNavigationModeChanged(wf::IInspectable const& sender, wux::DependencyPropertyChangedEventArgs const& args)
	{
		auto instance = sender.try_as<implementation::SwipeNavigator>();
		if (instance == nullptr)
		{
			return;
		}
		instance->UpdateCanNavigate();
	}
	void SwipeNavigator::OnRevealWidthChanged(wf::IInspectable const& sender, wux::DependencyPropertyChangedEventArgs const& args)
	{
		auto instance = sender.try_as<implementation::SwipeNavigator>();
		if (instance == nullptr)
		{
			return;
		}
		if (args.OldValue().try_as<double>() == args.NewValue().try_as<double>())
		{
			return;
		}
		instance->UpdateRevealWidth();
	}
	void SwipeNavigator::Close(bool withAnimation)
	{
		auto& tracker = mTracker;
		if (tracker == nullptr)
		{
			return;
		}
		if (withAnimation)
		{
			// Snap back the icon to idle position
			auto compositor = wuxh::ElementCompositionPreview::GetElementVisual(*self(this)).Compositor();
			auto anim = compositor.CreateVector3KeyFrameAnimation();
			auto easing = IconMode() == IconMode::Overlay ? compositor.CreateCubicBezierEasingFunction({ 1.0f, 0.78f }, { 1.0f, 1.0f })
				: compositor.CreateCubicBezierEasingFunction({ 0.0f, 0.0f }, { 0.0f, 1.0f });
			anim.InsertKeyFrame(1.0f, { 0.0f, 0.0f, 0.0f }, easing);
			anim.Duration(300ms);
			tracker.TryUpdatePositionWithAnimation(anim);
			tracker.Properties().InsertBoolean(propIsClosing, true);
		}
		else
		{
			tracker.TryUpdatePosition({});
			tracker.Properties().InsertBoolean(propIsClosing, false);
		}
	}
	void SwipeNavigator::TryUnregisterFrameCallbacks()
	{
		if (mFrame != nullptr)
		{
			mFrame.UnregisterPropertyChangedCallback(wuxc::Frame::CanGoBackProperty(), mFrameCanGoBackToken);
			mFrame.UnregisterPropertyChangedCallback(wuxc::Frame::CanGoForwardProperty(), mFrameCanGoForwardToken);
		}
	}
	void SwipeNavigator::OnFrameCanGoBackChanged(wf::IInspectable const& sender, wux::DependencyProperty const& args)
	{
		UpdateCanNavigate();
	}
	void SwipeNavigator::OnFrameCanGoForwardChanged(wf::IInspectable const& sender, wux::DependencyProperty const& args)
	{
		UpdateCanNavigate();
	}
	void SwipeNavigator::Update()
	{
		UpdateCanNavigate();
		UpdateIconMode();
	}
	template<typename Func>
	bool CanNavigate(NavigationMode mode, wuxc::Frame const& frame, Func&& frameFunc)
	{
		return mode == NavigationMode::Enabled
			? true
			: mode == NavigationMode::Auto
			? frame == nullptr
			? true
			: frameFunc()
			: false;
	}
	void SwipeNavigator::UpdateCanNavigate()
	{
		auto& source = mSource;
		if (source == nullptr)
		{
			return;
		}
		auto& frame = mFrame;
		const auto canGoBack = CanNavigate(BackNavigationMode(), frame, [&]()
			{
				return frame.CanGoBack();
			});
		const auto canGoForward = CanNavigate(ForwardNavigationMode(), frame, [&]()
			{
				return frame.CanGoForward();
			});
		CanGoBack(canGoBack);
		CanGoForward(canGoForward);
		source.PositionXSourceMode(canGoBack || canGoForward ? wuci::InteractionSourceMode::EnabledWithInertia : wuci::InteractionSourceMode::Disabled);
		if (mState == State::Idle)
		{
			auto& tracker = mTracker;
			if (tracker != nullptr)
			{
				auto properties = tracker.Properties();
				properties.InsertBoolean(propCanGoBack, CanGoBack());
				properties.InsertBoolean(propCanGoForward, CanGoForward());
			}
		}
	}
	void SwipeNavigator::UpdateIconMode()
	{
		auto& tracker = mTracker;
		if (tracker == nullptr)
		{
			return;
		}
		UpdateRevealWidth();
	}
	void SwipeNavigator::UpdateRevealWidth()
	{
		auto& tracker = mTracker;
		if (tracker == nullptr)
		{
			return;
		}
		auto impl = self(this);
		auto compositor = wuxh::ElementCompositionPreview::GetElementVisual(*impl).Compositor();
		const auto revealWidth = static_cast<float>(RevealWidth());
		tracker.MinPosition({ -revealWidth, 0.0f, 0.0f });
		tracker.MaxPosition({ revealWidth, 0.0f, 0.0f });
		// Source
		// Tracker snap point
		auto halfWidthStr = to_hstring(revealWidth * 0.5f);
		auto& snapToBack = mSnapToBack;
		snapToBack.Condition(compositor.CreateExpressionAnimation(hstring(L"this.Target.canGoBack && this.Target.NaturalRestingPosition.X < -") + halfWidthStr + L""));
		snapToBack.RestingValue(compositor.CreateExpressionAnimation(to_hstring(-revealWidth)));
		auto& snapToForward = mSnapToForward;
		snapToForward.Condition(compositor.CreateExpressionAnimation(hstring(L"this.Target.canGoForward && this.Target.NaturalRestingPosition.X > ") + halfWidthStr + L""));
		snapToForward.RestingValue(compositor.CreateExpressionAnimation(to_hstring(revealWidth)));
		auto& snapToContent = mSnapToContent;
		snapToContent.Condition(compositor.CreateExpressionAnimation(L"true"));
		snapToContent.RestingValue(compositor.CreateExpressionAnimation(L"0"));
		tracker.ConfigurePositionXInertiaModifiers({ snapToBack, snapToForward, snapToContent });
		constexpr auto enlargeRatio = 2.0f;
		const auto swipeWidth = revealWidth * enlargeRatio;
		auto widthStr = to_hstring(swipeWidth);
		const auto swipeAnimStr = hstring(L"min(max(")
			+ widthStr + L" * (0.5 * (1 - pow((" + widthStr + L" - (-tracker.Position.X > " + widthStr + L" ? " + widthStr + L" : -tracker.Position.X" + L")) / " + widthStr + L", 2))), !tracker.canGoForward ? 0 : -" + widthStr + L"), !tracker.canGoBack ? 0 : " + widthStr + L")";
		auto anim = compositor.CreateExpressionAnimation(swipeAnimStr);
		anim.SetReferenceParameter(L"tracker", tracker);
		auto content = GetTemplatedChild<wux::UIElement>(*impl, L"content");
		auto contentVisual = wuxh::ElementCompositionPreview::GetElementVisual(content);
		auto overlayBackIcon = GetTemplatedChild<wux::UIElement>(*impl, L"overlayBackIcon");
		auto overlayBackIconVisual = wuxh::ElementCompositionPreview::GetElementVisual(overlayBackIcon);
		auto overlayForwardIcon = GetTemplatedChild<wux::UIElement>(*impl, L"overlayForwardIcon");
		auto overlayForwardIconVisual = wuxh::ElementCompositionPreview::GetElementVisual(overlayForwardIcon);
		if (IconMode() == IconMode::BackDrop)
		{
			// Anim
			contentVisual.StartAnimation(L"Offset.X", anim);
			overlayBackIconVisual.StopAnimation(L"Offset.X");
			overlayForwardIconVisual.StopAnimation(L"Offset.X");
		}
		else
		{
			contentVisual.StopAnimation(L"Offset.X");
			auto virtualContentVisual = wuxh::ElementCompositionPreview::GetElementChildVisual(content);
			virtualContentVisual.StartAnimation(L"Offset.X", anim);
			auto overlayBackAnim = compositor.CreateExpressionAnimation(L"virtualContent.Offset.X - overlayBackIcon.Size.X");
			overlayBackAnim.SetReferenceParameter(L"virtualContent", virtualContentVisual);
			overlayBackAnim.SetReferenceParameter(L"overlayBackIcon", overlayBackIconVisual);
			overlayBackIconVisual.StartAnimation(L"Offset.X", overlayBackAnim);
			auto overlayForwardAnim = compositor.CreateExpressionAnimation(L"virtualContent.Offset.X + content.Size.X");
			overlayForwardAnim.SetReferenceParameter(L"virtualContent", virtualContentVisual);
			overlayForwardAnim.SetReferenceParameter(L"content", contentVisual);
			overlayForwardIconVisual.StartAnimation(L"Offset.X", overlayForwardAnim);
		}
		if (mState == State::Back)
		{
			if (IconMode() == IconMode::BackDrop)
			{
				mTracker.TryUpdatePosition({ -revealWidth, 0.0f, 0.0f });
			}
		}
		else if (mState == State::Forward)
		{
			if (IconMode() == IconMode::BackDrop)
			{
				mTracker.TryUpdatePosition({ revealWidth, 0.0f, 0.0f });
			}
		}
		else if (mState == State::Idle)
		{
			// no-op
		}
		else
		{
			// If the anim is playing while width is changed, the tracker woudn't use the new resting value by default.
			// Force tracker to re-evaluate snap point condition and resting value by moving with a small velocity.
			// According to the doc, this would force the tracker to enter inertia state and thus evaluate modifiers.
			const auto velX = mState == State::BackRequested || mState == State::ForwardCancelled
				? -1.0f
				: 1.0f;
			mTracker.TryUpdatePositionWithAdditionalVelocity({ velX, 0.0f, 0.0f });
		}
	}
	template<typename Func>
	void author::SwipeNavigator::Navigate(event<wf::TypedEventHandler<SwipeNavigation::SwipeNavigator, SwipeNavigation::NavigationRequestedEventArgs>> &ev, bool canNavigate, Func&& navigate)
	{
		auto iconMode = IconMode();
		// If we are in overlay mode, animate back to idle regardless of Handled
		// Also, we want to animate overlay icon to idle regardless of canNavigate
		if (iconMode == IconMode::Overlay)
		{
			Close();
		}
		if (!canNavigate)
		{
			return;
		}
		auto args{ make_self<implementation::NavigationRequestedEventArgs>() };
		ev(*self(this), *args);
		if (args->Handled())
		{
			// If we are in backdrop mode, animate back to idle position, otherwise the backdrop would stay visible.
			if (iconMode == IconMode::BackDrop)
			{
				Close();
			}
			return;
		}
		navigate();
	}
	void author::SwipeNavigator::GoBack()
	{
		Navigate(mBackRequested, CanGoBack(), [this]()
			{
				if (mFrame != nullptr)
				{
					if (mFrame.CanGoBack())
					{
						mFrame.GoBack();
					}
				}
			});
	}
	void SwipeNavigator::GoForward()
	{
		Navigate(mForwardRequested, CanGoForward(), [this]()
			{
				if (mFrame != nullptr)
				{
					if (mFrame.CanGoForward())
					{
						mFrame.GoForward();
					}
				}
			});
	}
	void SwipeNavigator::ResetTrackerProperties(wuc::CompositionPropertySet const& properties)
	{
		properties.InsertBoolean(propIsClosing, false);
		properties.InsertBoolean(propCanGoBack, CanGoBack());
		properties.InsertBoolean(propCanGoForward, CanGoForward());
	}
	void SwipeNavigator::OnLoaded(wf::IInspectable const& sender, wux::RoutedEventArgs const& args)
	{
		auto& tracker = mTracker;
		if (tracker == nullptr)
		{
			return;
		}
		tracker.TryUpdatePosition({});
		ResetTrackerProperties(tracker.Properties());
	}
	void SwipeNavigator::OnSystemBackRequested(wf::IInspectable const&, wu::Core::BackRequestedEventArgs const&)
	{
		GoBack();
	}
	void SwipeNavigator::OnRootPointerPressed(wf::IInspectable const& sender, wuxi::PointerRoutedEventArgs const& args)
	{
		if (args.Pointer().PointerDeviceType() != wdi::PointerDeviceType::Touch)
		{
			return;
		}
		if (mSource == nullptr)
		{
			return;
		}
		mSource.TryRedirectForManipulation(args.GetCurrentPoint(mRoot));
	}
	void SwipeNavigator::OnRootSizeChanged(wf::IInspectable const& sender, wux::SizeChangedEventArgs const& args)
	{
		auto setClip = [&](wuxm::RectangleGeometry const& clip)
		{
			if (clip != nullptr)
			{
				clip.Rect(wf::Rect{ {0.0f, 0.0f }, args.NewSize() });
			}
		};
		setClip(mBackDropClip);
		setClip(mOverlayClip);
	}
	wux::DependencyProperty SwipeNavigator::IconModeProperty(winrt::author::getter)
	{
		static auto prop = wux::DependencyProperty::Register(
			L"IconMode",
			xaml_typename<SwipeNavigation::IconMode>(),
			xaml_typename<SwipeNavigation::SwipeNavigator>(),
			wux::PropertyMetadata{ box_value(std::bit_cast<SwipeNavigation::IconMode>(IconMode::BackDrop)), SwipeNavigator::OnIconModeChanged }
		);
		return prop;
	}
	wux::DependencyProperty SwipeNavigator::BackNavigationModeProperty(winrt::author::getter)
	{
		static auto prop = wux::DependencyProperty::Register(
			L"BackNavigationMode",
			xaml_typename<SwipeNavigation::NavigationMode>(),
			xaml_typename<SwipeNavigation::SwipeNavigator>(),
			wux::PropertyMetadata{ box_value(std::bit_cast<SwipeNavigation::NavigationMode>(NavigationMode::Auto)), SwipeNavigator::OnBackNavigationModeChanged }
		);
		return prop;
	}
	wux::DependencyProperty SwipeNavigator::ForwardNavigationModeProperty(winrt::author::getter)
	{
		static auto prop = wux::DependencyProperty::Register(
			L"ForwardNavigationMode",
			xaml_typename<SwipeNavigation::NavigationMode>(),
			xaml_typename<SwipeNavigation::SwipeNavigator>(),
			wux::PropertyMetadata{ box_value(std::bit_cast<SwipeNavigation::NavigationMode>(NavigationMode::Auto)), SwipeNavigator::OnForwardNavigationModeChanged }
		);
		return prop;
	}
	Windows::UI::Xaml::DependencyProperty SwipeNavigator::CanGoBackProperty(winrt::author::getter)
	{
		static auto prop = wux::DependencyProperty::Register(
			L"CanGoBack",
			xaml_typename<bool>(),
			xaml_typename<SwipeNavigation::SwipeNavigator>(),
			wux::PropertyMetadata{ box_value(false) }
		);
		return prop;
	}
	Windows::UI::Xaml::DependencyProperty SwipeNavigator::CanGoForwardProperty(winrt::author::getter)
	{
		static auto prop = wux::DependencyProperty::Register(
			L"CanGoForward",
			xaml_typename<bool>(),
			xaml_typename<SwipeNavigation::SwipeNavigator>(),
			wux::PropertyMetadata{ box_value(false) }
		);
		return prop;
	}
	wux::DependencyProperty SwipeNavigator::RevealWidthProperty(winrt::author::getter)
	{
		constexpr auto defaultRevealWidth = 300.0;
		static auto prop = wux::DependencyProperty::Register(
			L"RevealWidth",
			xaml_typename<double>(),
			xaml_typename<SwipeNavigation::SwipeNavigator>(),
			wux::PropertyMetadata{ box_value<double>(defaultRevealWidth), SwipeNavigator::OnRevealWidthChanged }
		);
		return prop;
	}
	SwipeNavigation::SwipeNavigator SwipeNavigator::Find(wux::FrameworkElement const& element)
	{
		for (auto current = element; current != nullptr;)
		{
			auto navigator = current.try_as<SwipeNavigation::SwipeNavigator>();
			if (navigator != nullptr)
			{
				return navigator;
			}
			current = current.Parent().try_as<wux::FrameworkElement>();
		}
		return nullptr;
	}
	void SwipeNavigator::OnKeyboardAcceleratorInvoked(wuxi::KeyboardAcceleratorInvokedEventArgs const&, winrt::author::override)
	{
		GoBack();
	}
	void SwipeNavigator::CustomAnimationStateEntered(wuci::InteractionTracker const& sender, wuci::InteractionTrackerCustomAnimationStateEnteredArgs const& args, winrt::author::ignore)
	{
	}
	void SwipeNavigator::IdleStateEntered(wuci::InteractionTracker const& sender, wuci::InteractionTrackerIdleStateEnteredArgs const& args, winrt::author::ignore)
	{
		auto position = sender.Position().x;
		mState = position == 0.0f
			? State::Idle
			: position == RevealWidth()
			? State::Forward
			: State::Back;
		auto& tracker = mTracker;
		if (tracker != nullptr)
		{
			auto properties = tracker.Properties();
			auto isClosing = false;
			properties.TryGetBoolean(propIsClosing, isClosing);
			if (isClosing)
			{
				ResetTrackerProperties(properties);
			}
		}
	}
	void SwipeNavigator::InertiaStateEntered(wuci::InteractionTracker const& sender, wuci::InteractionTrackerInertiaStateEnteredArgs const& args, winrt::author::ignore)
	{
		auto restingPosition = args.ModifiedRestingPosition().Value();
		if (restingPosition.x == 0.0f)
		{
			mState = sender.Position().x < 0 ? State::BackCancelled : State::ForwardCancelled;
			return;
		}
		const auto targetState = restingPosition.x < 0 ? State::BackRequested : State::ForwardRequested;
		if (mState == targetState)
		{
			return;
		}
		mState = targetState;
		if (targetState == State::BackRequested)
		{
			GoBack();
		}
		else
		{
			GoForward();
		}
	}
	void SwipeNavigator::InteractingStateEntered(wuci::InteractionTracker const& sender, wuci::InteractionTrackerInteractingStateEnteredArgs const& args, winrt::author::ignore)
	{
		mState = State::Idle;
	}
	void SwipeNavigator::RequestIgnored(wuci::InteractionTracker const& sender, wuci::InteractionTrackerRequestIgnoredArgs const& args, winrt::author::ignore)
	{
	}
	void SwipeNavigator::ValuesChanged(wuci::InteractionTracker const& sender, wuci::InteractionTrackerValuesChangedArgs const& args, winrt::author::ignore)
	{
	}
}

