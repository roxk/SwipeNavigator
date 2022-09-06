using SwipeNavigation;
using System;
using System.Collections.Generic;
using System.ComponentModel;
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
    public sealed partial class OverlayTestPage : Page, INotifyPropertyChanged
    {
        public bool ShowSaveConfirmationDialog { get; set; }

        private SwipeNavigator _navigator;

        public event PropertyChangedEventHandler PropertyChanged;

        public SwipeNavigator Navigator
        {
            get { return _navigator; }
            set
            {
                _navigator = value;
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(Navigator)));
            }
        }
        public OverlayTestPage()
        {
            this.InitializeComponent();
        }

        private void OnLoaded(object sender, RoutedEventArgs e)
        {
            Navigator = SwipeNavigator.Find(this);
            Navigator.BackRequested += OnBackRequested;
        }

        private void OnUnloaded(object sender, RoutedEventArgs e)
        {
            Navigator.BackRequested -= OnBackRequested;
        }

        private async void OnBackRequested(object sender, NavigationRequestedEventArgs e)
        {
            if (!ShowSaveConfirmationDialog)
            {
                return;
            }
            e.Handled = true;
            var dialog = new ContentDialog
            {
                Title = "Save changes?",
                PrimaryButtonText = "Save",
                SecondaryButtonText = "Don't Save",
                CloseButtonText = "Cancel",
            };
            var result = await dialog.ShowAsync();
            if (result != ContentDialogResult.None)
            {
                if (Navigator?.Frame is Frame frame)
                {
                    if (Frame.CanGoBack)
                    {
                        Frame.GoBack();
                    }
                }
            }
        }

        private void RadioButton_Checked(object sender, RoutedEventArgs e)
        {
            var radioButton = sender as RadioButton;
            if (radioButton == null)
            {
                return;
            }
            if (radioButton.Tag == null)
            {
                return;
            }
            var mode = (SwipeNavigation.NavigationMode) radioButton.Tag;
            if (Navigator != null)
            {
                Navigator.BackNavigationMode = mode;
            }
        }
    }
}
