#pragma once

#include "SwipeNavigatorTemplate.g.h"

namespace winrt::SwipeNavigation::implementation
{
    struct SwipeNavigatorTemplate : SwipeNavigatorTemplateT<SwipeNavigatorTemplate>
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
