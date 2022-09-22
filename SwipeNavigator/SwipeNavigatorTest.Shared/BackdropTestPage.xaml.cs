using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using System.Threading.Tasks;
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
    public sealed partial class BackDropTestPage : Page
    {
        public BackDropTestPage()
        {
            this.InitializeComponent();
        }

        public bool IsConfirmationShown { get; set; } = false;

        /// <summary>
        /// 
        /// </summary>
        /// <param name="args"></param>
        /// <returns>true if should navigate</returns>
        private async Task<bool> ShowConfirmationDialog(SwipeNavigation.NavigationRequestedEventArgs args)
        {
            if (!IsConfirmationShown)
            {
                return false;
            }
            args.Handled = true;
            var dialog = new ContentDialog()
            {
                Title = "Save changes?",
                PrimaryButtonText = "Save",
                SecondaryButtonText = "Don't Save",
                CloseButtonText = "Cancel",
            };
            var result = await dialog.ShowAsync();
            return result == ContentDialogResult.Primary || result == ContentDialogResult.Secondary;
        }

        private async void SwipeNavigator_BackRequested(SwipeNavigation.SwipeNavigator sender, SwipeNavigation.NavigationRequestedEventArgs args)
        {
            var result = await ShowConfirmationDialog(args);
            if (result && sender.Frame is Frame frame)
            {
                if (frame.CanGoBack)
                {
                    frame.GoBack();
                }
            }
        }

        private async void SwipeNavigator_ForwardRequested(SwipeNavigation.SwipeNavigator sender, SwipeNavigation.NavigationRequestedEventArgs args)
        {
            var result = await ShowConfirmationDialog(args);
            if (result && sender.Frame is Frame frame)
            {
                if (frame.CanGoForward)
                {
                    frame.GoForward();
                }
            }
        }
    }
}
