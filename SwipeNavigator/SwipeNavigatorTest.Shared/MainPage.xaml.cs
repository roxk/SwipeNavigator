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
using Windows.UI.Xaml.Media.Animation;
using Windows.UI.Xaml.Navigation;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

namespace SwipeNavigatorTest
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page, INotifyPropertyChanged
    {
        private IconMode _iconMode = IconMode.BackDrop;
        public IconMode IconMode
        {
            get { return _iconMode; }
            set
            {
                _iconMode = value;
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(IconMode)));
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(IsBackDropFrameVisible)));
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(IsOverlayFrameVisible)));
            }
        }

        public bool IsBackDropFrameVisible { get { return _iconMode == IconMode.BackDrop; } }

        public bool IsOverlayFrameVisible
        {
            get
            {
                return _iconMode == IconMode.Overlay;
            }
        }

        public MainPage()
        {
            this.InitializeComponent();
            overlayFrame.Navigate(typeof(OverlayTestPage));
            backDropFrame.Navigate(typeof(BackDropTestPage));
        }

        public event PropertyChangedEventHandler PropertyChanged;

        private void NextPage_Click(object sender, RoutedEventArgs e)
        {
            var transitionInfo = new FrameNavigationOptions()
            {
                TransitionInfoOverride = new SlideNavigationTransitionInfo()
                {
                    Effect = SlideNavigationTransitionEffect.FromRight,
                },
                IsNavigationStackEnabled = true,
            };
            if (IconMode == IconMode.Overlay)
            {
                overlayFrame.NavigateToType(typeof(OverlayTestPage), null, transitionInfo);
            }
            else
            {
                backDropFrame.NavigateToType(typeof(BackDropTestPage), null, transitionInfo);
            }
        }

        private void CloseTest_Click(object sender, RoutedEventArgs e)
        {
            var transitionInfo = new FrameNavigationOptions()
            {
                TransitionInfoOverride = new SlideNavigationTransitionInfo()
                {
                    Effect = SlideNavigationTransitionEffect.FromRight,
                },
                IsNavigationStackEnabled = true,
            };
            if (IconMode != IconMode.BackDrop)
            {
                return;
            }
            backDropFrame.NavigateToType(typeof(BackdropNotHandledAndCloseTestPage), null, transitionInfo);
        }

        private void ScrollViewerTest_Click(object sender, RoutedEventArgs e)
        {
            var transitionInfo = new FrameNavigationOptions()
            {
                TransitionInfoOverride = new SlideNavigationTransitionInfo()
                {
                    Effect = SlideNavigationTransitionEffect.FromRight,
                },
                IsNavigationStackEnabled = true,
            };
            if (IconMode != IconMode.BackDrop)
            {
                return;
            }
            backDropFrame.NavigateToType(typeof(ScrollViewerTestPage), null, transitionInfo);
        }

        private void RadioButton_Checked(object sender, RoutedEventArgs e)
        {
            var radio = sender as RadioButton;
            if (radio == null)
            {
                return;
            }
            // Tag can be null during first render, guard against it.
            if (radio.Tag == null)
            {
                return;
            }
            switch (radio.GroupName)
            {
                case "IconMode":
                    IconMode = (SwipeNavigation.IconMode) radio.Tag;
                    break;
                case "BackNavMode":
                    navigator.BackNavigationMode = (SwipeNavigation.NavigationMode) radio.Tag;
                    break;
                case "ForwardNavMode":
                    navigator.ForwardNavigationMode = (SwipeNavigation.NavigationMode)radio.Tag;
                    break;
            }
        }
    }
}
