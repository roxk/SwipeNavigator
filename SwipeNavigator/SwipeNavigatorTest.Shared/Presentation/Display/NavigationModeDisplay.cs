using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SwipeNavigatorTest.Presentation.Display
{
    public class NavigationModeDisplay
    {
        public static bool IsAutoVisible(SwipeNavigation.NavigationMode mode)
        {
            return mode == SwipeNavigation.NavigationMode.Auto;
        }
        public static bool IsEnabledVisible(SwipeNavigation.NavigationMode mode)
        {
            return mode == SwipeNavigation.NavigationMode.Enabled;
        }
        public static bool IsDisabledVisible(SwipeNavigation.NavigationMode mode)
        {
            return mode == SwipeNavigation.NavigationMode.Disabled;
        }
    }
}
