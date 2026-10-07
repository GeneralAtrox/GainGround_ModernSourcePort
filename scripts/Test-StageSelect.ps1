<#
.SYNOPSIS
Drives the real runtime through every stage with the Stage menu.

.DESCRIPTION
Starts gain_ground_runtime.exe with the opt-in diagnostic log, coins up and
starts a game, then for each stage index: selects it through the Stage menu,
waits for the game's own stage-start handshake and stage initializer, selects a
character so one spawns, plays for a few seconds, and checks that the phase
machine never fell into the continue countdown and that the runtime is alive.
A runtime stop is recorded with its fault report and the game is restarted so
the remaining stages are still exercised. Requires the imported ROM cache
(first run) and the MSYS2 runtime DLLs.

.EXAMPLE
.\scripts\Test-StageSelect.ps1 -From 0 -To 19
#>
param(
    [string]$Exe = (Join-Path (Split-Path -Parent $PSScriptRoot) 'build\gain_ground_runtime.exe'),
    [int]$From = 0,
    [int]$To = 39,
    [double]$PlaySeconds = 3,
    [int]$StageTimeoutSeconds = 20,
    [string]$ToolchainBin = 'C:\msys64\ucrt64\bin',
    [string]$Log = (Join-Path $env:TEMP 'gain-ground-stage-select-test.log'),
    # Sweep mode: run the game at 10x with an invulnerable player and, on each
    # stage, let the runtime move player 1 to every clear grid cell.
    [switch]$Sweep,
    [double]$Speed = 10,
    [int]$SweepFrames = 8,
    [int]$SweepTimeoutSeconds = 600
)
$ErrorActionPreference = 'Stop'
Add-Type -Namespace GgWin -Name Native -MemberDefinition @'
[DllImport("user32.dll")] public static extern bool PostMessageW(IntPtr h, uint msg, IntPtr w, IntPtr l);
'@
$WM_KEYDOWN = 0x100; $WM_KEYUP = 0x101; $WM_COMMAND = 0x111
# Default bindings (F credit, Q attack/join); GAIN_GROUND_DEFAULT_CONTROLS below
# makes the runtime ignore controls remapped in Settings > Controls.
$VK_F = 0x46; $VK_Q = 0x51; $StageCommandBase = 2000

if (Test-Path $Log) { Remove-Item $Log -Force }
$env:GAIN_GROUND_NAV_LOG = $Log
$env:GAIN_GROUND_DEFAULT_CONTROLS = '1'
if ($Sweep) { $env:GAIN_GROUND_TEST_SPEED = [string]$Speed; $env:GAIN_GROUND_INVULNERABLE = '1'; $env:GAIN_GROUND_SWEEP_FRAMES = [string]$SweepFrames }
else { Remove-Item Env:GAIN_GROUND_TEST_SPEED, Env:GAIN_GROUND_INVULNERABLE -ErrorAction SilentlyContinue }
$SweepStartCommand = 3001
$env:Path = $ToolchainBin + ';' + $env:Path
$failureDir = Join-Path (Split-Path $Exe) 'run\last-native-failure'
$failureReport = Join-Path $failureDir 'status.txt'

$script:process = $null; $script:window = [IntPtr]::Zero
function Key([int]$vk, [int]$holdMs = 120) {
    [void][GgWin.Native]::PostMessageW($script:window, $WM_KEYDOWN, [IntPtr]$vk, [IntPtr]0)
    Start-Sleep -Milliseconds $holdMs
    [void][GgWin.Native]::PostMessageW($script:window, $WM_KEYUP, [IntPtr]$vk, [IntPtr]([int64]0xC0000000))
    Start-Sleep -Milliseconds 150
}
function Stop-Game { if ($script:process) { $script:process.Refresh(); if (-not $script:process.HasExited) { Stop-Process -Id $script:process.Id -Force } } }
function Start-Game {
    if (Test-Path $failureDir) { Remove-Item $failureDir -Recurse -Force }
    $script:process = Start-Process -FilePath $Exe -WorkingDirectory (Split-Path $Exe) -PassThru
    $script:window = [IntPtr]::Zero
    for ($i = 0; $i -lt 100 -and $script:window -eq [IntPtr]::Zero; $i++) { Start-Sleep -Milliseconds 200; $script:process.Refresh(); $script:window = $script:process.MainWindowHandle }
    if ($script:window -eq [IntPtr]::Zero) { Stop-Game; throw 'Runtime window did not appear' }
    Start-Sleep -Seconds 6            # logo and boot to the title
    Key $VK_F; Key $VK_F              # two credits
    Key $VK_Q 200; Start-Sleep -Seconds 2; Key $VK_Q 200   # start, pick the first character
    Start-Sleep -Seconds 2
}
function LogLines { if (Test-Path $Log) { @(Get-Content $Log -ErrorAction SilentlyContinue) } else { @() } }
function WaitForLine([string]$pattern, [int]$after, [int]$timeoutSeconds) {
    $deadline = (Get-Date).AddSeconds($timeoutSeconds)
    while ((Get-Date) -lt $deadline) {
        $lines = LogLines
        for ($i = $after; $i -lt $lines.Count; $i++) { if ($lines[$i] -match $pattern) { return $i } }
        $script:process.Refresh(); if ($script:process.HasExited) { return -2 }
        if (Test-Path $failureReport) { return -3 }
        Start-Sleep -Milliseconds 100
    }
    return -1
}
function Frame([string]$line) { if ($line -match 'frame (\d+)') { [int]$Matches[1] } else { -1 } }
function FaultSummary {
    if (-not (Test-Path $failureReport)) { return 'runtime exited without a report' }
    $text = Get-Content $failureReport
    $pc = ($text | Select-String 'PC 0x([0-9A-F]+)' | Select-Object -First 1).Matches[0].Groups[1].Value
    $fn = ($text | Select-String 'Function (\d+)\s+Entry 0x([0-9A-F]+)' | Select-Object -First 1)
    $reason = ($text | Select-Object -First 1)
    if ($fn) { '{0} at PC {1} in function {2} (entry {3})' -f $reason, $pc, $fn.Matches[0].Groups[1].Value, $fn.Matches[0].Groups[2].Value } else { '{0} at PC {1}' -f $reason, $pc }
}

# The harness only fires, so the character can die. Continue with a credit
# and rejoin so the next selection still has a game to act on.
function Recover-Game {
    $before = (LogLines).Count
    Key $VK_F; Key $VK_Q 200                       # credit, continue
    # The continue leaves phase 4 for play (c16 4 -> 0); then pick a character.
    $back = WaitForLine 'offset 00c16 0004 -> 0000' $before 8
    if ($back -lt 0) { Start-Sleep -Seconds 2 }
    Start-Sleep -Seconds 2; Key $VK_Q 200; Start-Sleep -Milliseconds 400; Key $VK_Q 200; Start-Sleep -Seconds 2
}

Start-Game
$results = @()
for ($stage = $From; $stage -le $To; $stage++) {
    $label = 'Round {0} Stage {1}' -f (([int][math]::Floor($stage / 10)) + 1), (($stage % 10) + 1)
    $before = (LogLines).Count
    [void][GgWin.Native]::PostMessageW($script:window, $WM_COMMAND, [IntPtr]($StageCommandBase + $stage), [IntPtr]0)
    $selectedAt = Get-Date
    $status = 'pass'; $detail = ''
    $handshake = WaitForLine ('stage-start selector {0} ' -f ($stage + 1)) $before $StageTimeoutSeconds
    if ($handshake -eq -1) {
        # Probably no game in progress any more (title or continue screen): rejoin and retry once.
        Recover-Game
        $before = (LogLines).Count
        [void][GgWin.Native]::PostMessageW($script:window, $WM_COMMAND, [IntPtr]($StageCommandBase + $stage), [IntPtr]0)
        $handshake = WaitForLine ('stage-start selector {0} ' -f ($stage + 1)) $before $StageTimeoutSeconds
        if ($handshake -ge 0) { $detail = 'after rejoining; ' }
    }
    if ($handshake -lt -1) { $status = 'crash'; $detail = 'before the stage handshake: ' + (FaultSummary) }
    elseif ($handshake -eq -1) { $status = 'fail'; $detail = 'no stage-start handshake for the chosen stage' }
    else {
        $init = WaitForLine 'offset 00c16 0002 -> 0000' $handshake $StageTimeoutSeconds
        if ($init -lt -1) { $status = 'crash'; $detail = 'during the stage load: ' + (FaultSummary) }
        elseif ($init -eq -1) { $status = 'fail'; $detail = 'stage initializer never ran' }
        else {
            $lines = LogLines
            $clearLine = $handshake
            for ($i = $handshake; $i -ge $before; $i--) { if ($lines[$i] -match 'offset 00c02 ') { $clearLine = $i; break } }
            $detail += 'clear->init {0} frames' -f ((Frame $lines[$init]) - (Frame $lines[$clearLine]))
            $initFrame = Frame $lines[$init]
            Start-Sleep -Seconds 3            # stage title card
            Key $VK_Q 200; Start-Sleep -Milliseconds 400; Key $VK_Q 200   # character select -> spawn
            if ($Sweep) {
                Start-Sleep -Seconds 1
                $sweepStart = (LogLines).Count
                [void][GgWin.Native]::PostMessageW($script:window, $WM_COMMAND, [IntPtr]$SweepStartCommand, [IntPtr]0)
                $sweepEnd = WaitForLine 'sweep (done|no player record during|left stage during) stage' $sweepStart $SweepTimeoutSeconds
                if ($sweepEnd -ge 0) {
                    $line = (LogLines)[$sweepEnd]
                    if ($line -match 'sweep done stage \d+ teleports (\d+) skipped (\d+)') { $detail += ('; sweep {0} cells, {1} blocked' -f $Matches[1], $Matches[2]) }
                    else { $status = 'fail'; $detail += '; ' + ($line -replace '^sweep ', 'sweep ') }
                } elseif ($sweepEnd -eq -1) { $status = 'fail'; $detail += '; sweep did not finish' }
            } else {
                $playUntil = (Get-Date).AddSeconds($PlaySeconds)
                while ((Get-Date) -lt $playUntil) { Key $VK_Q 80 }
            }
            Start-Sleep -Milliseconds 700     # let a failure report land
            $script:process.Refresh()
            if ($script:process.HasExited -or (Test-Path $failureReport)) { $status = 'crash'; $detail += '; during play: ' + (FaultSummary) }
            else {
                $lines = LogLines
                for ($i = $init; $i -lt $lines.Count; $i++) {
                    if ($lines[$i] -match 'offset 00c16 0000 -> 0004') {
                        # Right after the initializer means nobody was carried over; later it is a death.
                        if (((Frame $lines[$i]) - $initFrame) -lt 120) { $status = 'fail'; $detail += '; continue countdown started (no player on the field)' }
                        else { $detail += '; character died in play, continued'; Recover-Game }
                        break
                    }
                }
            }
        }
    }
    $results += [pscustomobject]@{ Index = $stage; Stage = $label; Result = $status; Detail = $detail; Seconds = [math]::Round(((Get-Date) - $selectedAt).TotalSeconds, 1) }
    Write-Host ('{0,-18} {1,-5} {2}' -f $label, $status, $detail)
    if ($status -eq 'crash') { Stop-Game; if ($stage -lt $To) { Start-Game } }
}
Stop-Game
$results | Format-Table -AutoSize | Out-String | Write-Host
$failures = @($results | Where-Object Result -ne 'pass').Count
Write-Host ('{0} of {1} stages passed' -f (($results.Count) - $failures), $results.Count)
exit $failures
