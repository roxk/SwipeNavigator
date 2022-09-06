#include "pch.h"
#include "SwipeNavigatorTemplate.h"
#if __has_include("SwipeNavigatorTemplate.g.cpp")
#include "SwipeNavigatorTemplate.g.cpp"
#endif

using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::SwipeNavigation::implementation
{
    SwipeNavigatorTemplate::SwipeNavigatorTemplate()
    {
        InitializeComponent();
    }
}
