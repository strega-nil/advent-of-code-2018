[CmdletBinding()]
param(
  [String] $Day,
  [String] $Part
)

$dayPath = "day-$Day"

if (-not (Test-Path ".\$dayPath")) {
  throw "day '$Day' does not exist"
}

if (-not (Test-Path '.\build')) {
  cmake -B 'build'
  if (-not $?) {
    throw 'configure failed'
  }
}

cmake --build 'build'
if (-not $?) {
  throw 'build failed'
}

Write-Host

& ".\build\$dayPath.exe" $Part
