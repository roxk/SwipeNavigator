#pragma once

#include "NavigationRequestedEventArgs.g.h"

namespace winrt::SwipeNavigation::implementation
{
    struct NavigationRequestedEventArgs : NavigationRequestedEventArgsT<NavigationRequestedEventArgs>
    {
        NavigationRequestedEventArgs() = default;
        bool Handled() const { return mIsHandled; }
        void Handled(bool isHandled) { mIsHandled = isHandled; }
    private:
        bool mIsHandled;
    };
}
