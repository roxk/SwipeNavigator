using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238

namespace SwipeNavigatorTest
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class BackdropNotHandledAndCloseTestPage : Page
    {
        public bool IsEventHandled { get; set; } = true;
        public BackdropNotHandledAndCloseTestPage()
        {
            this.InitializeComponent();
        }
        
        private void SwipeNavigator_BackRequested(SwipeNavigation.SwipeNavigator sender, SwipeNavigation.NavigationRequestedEventArgs args)
        {
            args.Handled = IsEventHandled;
        }

        private void SwipeNavigator_ForwardRequested(SwipeNavigation.SwipeNavigator sender, SwipeNavigation.NavigationRequestedEventArgs args)
        {
            args.Handled = IsEventHandled;
        }

        private void OnClose(object sender, RoutedEventArgs args)
        {
            swipeNavigator.Close(false);
        }

        private void OnCloseWithAnim(object sender, RoutedEventArgs args)
        {
            swipeNavigator.Close();
        }

        private void OnBack(object sender, RoutedEventArgs args)
        {
            if (Frame.CanGoBack)
            {
                Frame.GoBack();
            }
        }
        private void OnForward(object sender, RoutedEventArgs args)
        {
            if (Frame.CanGoForward)
            {
                Frame.GoForward();
            }
        }
    }
}
