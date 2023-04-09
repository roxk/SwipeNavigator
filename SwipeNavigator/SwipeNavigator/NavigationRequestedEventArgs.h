#pragma once

#include "pch.h"
#include "NavigationRequestedEventArgs.g.h"

namespace winrt::SwipeNavigation::implementation
{
    struct NavigationRequestedEventArgs : NavigationRequestedEventArgsT<NavigationRequestedEventArgs>
    {
        [[clang::annotate("idlgen::getter")]]
        bool Handled() const { return mIsHandled; }
        [[clang::annotate("idlgen::setter")]]
        void Handled(bool isHandled) { mIsHandled = isHandled; }
    private:
        bool mIsHandled{};
    };
}
