# Contributing

Apart from bug fixes, behavior changes or new feature/API should be discussed in issues first so that you don't waste time on PRs that would not be merged.

## Prerequisite

- Visual Studio 2022 - https://visualstudio.microsoft.com/vs/
- .NET 6
- UWP Workload
- C++ Components including C++/WinRT
- XAML Styler extension - https://marketplace.visualstudio.com/items?itemName=TeamXavalon.XAMLStyler2022
  - Formatting is checked in CI

### Nice-to-have

- WinRT Tools for C++ VS Extension - https://marketplace.visualstudio.com/items?itemName=MadsKristensen.MIDL

**Note**
If you are on VS 2019:

- C++/WinRT VS Extension for VS 2019 - https://marketplace.visualstudio.com/items?itemName=CppWinRTTeam.cppwinrt101804264
  - If you are on VS 2022, this is not needed - VS includes it by default since VS 2022

If you open the solution and VS complains there are missing components, just install them. Please file an issue mentioning the missing component so this page can be updated. PRs are welcome as well.

## Getting Started

- Open the solution in VS and nuget should be stored. 
- Make sure `SwipeNavigatorTest` is set as start up project.
- Configure Platform. On WoA machines you want ARM64. Use x64 otherwise.
- You can leave the Configuration to Debug for development.
- Click LocalMachine if your device has touch capability, or RemoteMachine if you want to deploy to another device equipped with remote debugger. Make sure you configure remote debugging correctly if so by right-clicking `SwipeNavigatorTest` -> Properties -> Debug.

## CI Check

Make sure your code pass CI. Run

```
cd SwipeNavigator
./scripts/ci
```

to check your code before submitting PRs.
