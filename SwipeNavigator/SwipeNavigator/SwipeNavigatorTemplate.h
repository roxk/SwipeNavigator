#pragma once

#include <winrt/Windows.UI.Xaml.h>
#include <winrt/author/base.h>
#include <functional>
#include "ns.h"

namespace winrt::SwipeNavigation::author
{
    struct SwipeNavigatorTemplate : winrt::author::runtimeclass<wux::ResourceDictionary>
    {
    };
}