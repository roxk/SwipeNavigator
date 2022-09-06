. "$PSScriptRoot\get-msbuild"
&$msbuild $PSScriptRoot\..\SwipeNavigatorTestNuget\SwipeNavigatorTestNuget.csproj -t:Restore
&$msbuild $PSScriptRoot\..\SwipeNavigatorTestNuget\SwipeNavigatorTestNuget.csproj -p:Platform=x64,Configuration=Debug
