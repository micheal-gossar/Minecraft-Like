param(
    [Parameter(Mandatory = $true)]
    [ValidateSet('Build', 'BuildAndRun', 'Clean')]
    [string]$Action
)

$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
$vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio\Installer\vswhere.exe'
if (-not (Test-Path -LiteralPath $vswhere)) {
    throw "Visual Studio Installer was not found: $vswhere"
}

$cmake = & $vswhere -latest -products '*' -version '[18.0,19.0)' `
    -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 `
    -find 'Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe' |
    Select-Object -First 1
if (-not $cmake -or -not (Test-Path -LiteralPath $cmake)) {
    throw 'Visual Studio 2026 with the C++ workload and CMake tools is required.'
}

Push-Location $root
try {
    if ($Action -eq 'Clean') {
        if (-not (Test-Path -LiteralPath 'out\build\vs2026\CMakeCache.txt')) {
            Write-Host 'Nothing to clean: the project has not been configured.'
            exit 0
        }
        & $cmake --build --preset debug --target clean
        exit $LASTEXITCODE
    }

    & $cmake --preset vs2026
    if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
    & $cmake --build --preset debug
    if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

    if ($Action -eq 'BuildAndRun') {
        $terminalScript = Join-Path $PSScriptRoot 'game-terminal.ps1'
        Start-Process -FilePath 'powershell.exe' `
            -WorkingDirectory $root `
            -ArgumentList "-NoExit -ExecutionPolicy Bypass -File `"$terminalScript`""
        exit 0
    }
    exit 0
}
finally {
    Pop-Location
}
