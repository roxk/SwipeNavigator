#pragma once

#include <winrt/author/base.h>

namespace winrt::SwipeNavigation::author
{
    struct NavigationRequestedEventArgs : winrt::author::runtimeclass<>
    {
        bool Handled(winrt::author::getter = {}) const { return mIsHandled; }
        winrt::author::setter Handled(bool isHandled)
        {
            mIsHandled = isHandled;
            return {};
        }
        NavigationRequestedEventArgs(winrt::author::ignore = {}) {}
    private:
        bool mIsHandled{};
    };
}
