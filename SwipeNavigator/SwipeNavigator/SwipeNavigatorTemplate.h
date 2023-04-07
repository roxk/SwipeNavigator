#pragma once

#include "pch.h"
#include "SwipeNavigatorTemplate.g.h"

namespace winrt::SwipeNavigation::implementation
{
    struct
    [[clang::annotate("idlgen::attribute=default_interface")]]
    SwipeNavigatorTemplate : SwipeNavigatorTemplateT<SwipeNavigatorTemplate>, idlgen::base<wux::ResourceDictionary>
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
