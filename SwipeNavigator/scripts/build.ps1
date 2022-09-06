param(
    [Parameter(Mandatory=$true)]
    [String]$version
)
$template = Get-Content $PSScriptRoot\..\build\SwipeNavigator.nuspec.template
$nuspec = $template.Replace("VERSION", $version)
Set-Content -path $PSScriptRoot\..\build\.nuspec -value $nuspec
. "$PSScriptRoot\get-msbuild"
nuget restore $PSScriptRoot\..\SwipeNavigator -PackagesDirectory $PSScriptRoot\..\packages
&$msbuild $PSScriptRoot\..\SwipeNavigator\SwipeNavigator.vcxproj -p:Platform=x64,Configuration=Release
&$msbuild $PSScriptRoot\..\SwipeNavigator\SwipeNavigator.vcxproj -p:Platform=ARM64,Configuration=Release
nuget pack $PSScriptRoot\..\build\.nuspec -outputDirectory $PSScriptRoot\..\build
