$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
$executable = Join-Path $root 'out\build\vs2026\Debug\SpaceEngine.exe'

if (-not (Test-Path -LiteralPath $executable)) {
    throw "Game executable was not found: $executable"
}

& $executable
