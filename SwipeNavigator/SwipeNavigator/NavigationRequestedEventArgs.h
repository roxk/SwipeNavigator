#pragma once

#include "pch.h"
#include "NavigationRequestedEventArgs.g.h"

namespace winrt::SwipeNavigation::implementation
{
    struct NavigationRequestedEventArgs : NavigationRequestedEventArgsT<NavigationRequestedEventArgs>
    {
        bool Handled() const { return mIsHandled; }
        void Handled(bool isHandled) { mIsHandled = isHandled; }
    private:
        bool mIsHandled{};
    };
}
