[CmdletBinding()]
param([string]$BuildDirectory = 'build', [string]$ToolchainRoot = 'C:\msys64\ucrt64')
$ErrorActionPreference = 'Stop'
$projectRoot = Split-Path -Parent $PSScriptRoot
$previousPath = $env:Path
try {
    $env:Path = (Join-Path $ToolchainRoot 'bin') + ';' + $env:Path
    & cmake -S (Join-Path $projectRoot 'native') -B $BuildDirectory -G Ninja `
        -DCMAKE_BUILD_TYPE=Release "-DCMAKE_CXX_COMPILER=$ToolchainRoot/bin/g++.exe" "-DZLIB_ROOT=$ToolchainRoot"
    if ($LASTEXITCODE -ne 0) { throw 'CMake configuration failed' }
    & cmake --build $BuildDirectory --parallel
    if ($LASTEXITCODE -ne 0) { throw 'Build failed' }
} finally { $env:Path = $previousPath }
