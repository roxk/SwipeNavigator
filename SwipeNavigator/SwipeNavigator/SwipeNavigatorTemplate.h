#pragma once

#include "pch.h"
#include "SwipeNavigatorTemplate.g.h"

namespace winrt::SwipeNavigation::implementation
{
    struct
    [[clang::annotate("idlgen::attribute=default_interface")]]
    [[clang::annotate("idlgen::extend=Windows.UI.Xaml.ResourceDictionary")]]
    SwipeNavigatorTemplate : SwipeNavigatorTemplateT<SwipeNavigatorTemplate>
    {
        SwipeNavigatorTemplate();
    };
}

namespace winrt::SwipeNavigation::factory_implementation
{
    struct SwipeNavigatorTemplate : SwipeNavigatorTemplateT<SwipeNavigatorTemplate, implementation::SwipeNavigatorTemplate>
    {
    };
}
