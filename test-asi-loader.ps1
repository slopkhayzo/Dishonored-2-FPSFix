param(
    [Parameter(Mandatory = $true)]
    [string]$LoaderPath
)

$ErrorActionPreference = 'Stop'

$repositoryRoot = [System.IO.Path]::GetFullPath($PSScriptRoot)
$buildDirectory = Join-Path $repositoryRoot 'build'
$stageDirectory = Join-Path $buildDirectory 'asi-loader-test-stage'
$resolvedLoader = (Resolve-Path -LiteralPath $LoaderPath).Path
$plugin = Join-Path $buildDirectory 'Dishonored2HighFPSFix.asi'
$testHost = Join-Path $buildDirectory 'asi-loader-test.exe'

if (-not (Test-Path -LiteralPath $plugin -PathType Leaf) -or
    -not (Test-Path -LiteralPath $testHost -PathType Leaf)) {
    throw 'Build the project with build-msvc.cmd before running this test.'
}

$resolvedStage = [System.IO.Path]::GetFullPath($stageDirectory)
$requiredPrefix = $repositoryRoot.TrimEnd('\') + '\build\'
if (-not $resolvedStage.StartsWith($requiredPrefix,
        [System.StringComparison]::OrdinalIgnoreCase)) {
    throw "Refusing to recreate an unexpected staging path: $resolvedStage"
}

if (Test-Path -LiteralPath $resolvedStage) {
    Remove-Item -LiteralPath $resolvedStage -Recurse -Force
}
New-Item -ItemType Directory -Path $resolvedStage | Out-Null

Copy-Item -LiteralPath $resolvedLoader -Destination (Join-Path $resolvedStage 'dinput8.dll')
Copy-Item -LiteralPath $plugin -Destination $resolvedStage
Copy-Item -LiteralPath $testHost -Destination $resolvedStage

Push-Location $resolvedStage
try {
    & '.\asi-loader-test.exe'
    if ($LASTEXITCODE -ne 0) {
        throw "ASI loader integration test failed with exit code $LASTEXITCODE."
    }
}
finally {
    Pop-Location
}
