[CmdletBinding()]
param(
    [ValidateSet('x64')]
    [string] $Target = 'x64',
    [ValidateSet('Debug', 'RelWithDebInfo', 'Release', 'MinSizeRel')]
    [string] $Configuration = 'RelWithDebInfo',
    [switch] $SkipAll,
    [switch] $SkipBuild,
    [switch] $SkipDeps,
    [string] $BoostDirectory = 'D:\a'
)

$ErrorActionPreference = 'Stop'

if ( $DebugPreference -eq 'Continue' ) {
    $VerbosePreference = 'Continue'
    $InformationPreference = 'Continue'
}

if ( ! ( [System.Environment]::Is64BitOperatingSystem ) ) {
    throw "A 64-bit system is required to build the project."
}

if ( $PSVersionTable.PSVersion -lt '7.0.0' ) {
    Write-Warning 'The obs-deps PowerShell build script requires PowerShell Core 7. Install or upgrade your PowerShell version: https://aka.ms/pscore6'
    exit 2
}

function Download-Boost {
    param (
        [string] $Version,
        [string] $Destination
    )

    $BoostUrl = "https://archives.boost.io/release/1.74.0/source/boost_1_74_0.zip"
    $BoostZip = "${Destination}\boost_1_74_0.zip"
    $BoostExtractPath = "${Destination}\boost_1_74_0"

    if (-Not (Test-Path -Path $BoostExtractPath)) {
        Write-Host "Downloading Boost ${Version}..."
        Invoke-WebRequest -Uri $BoostUrl -OutFile $BoostZip

        Write-Host "Extracting Boost..."
        Expand-Archive -Path $BoostZip -DestinationPath $Destination -Force
        Remove-Item $BoostZip -Force
    } else {
        Write-Host "Boost ${Version} already exists at ${BoostExtractPath}"
    }

    return $BoostExtractPath
}

function Build-Boost {
    param (
        [string] $BoostPath
    )

    Push-Location -Path $BoostPath

    .\bootstrap.bat

    Write-Host "Building Boost..."
    & .\b2 || {
        Write-Error "Boost build failed."
        Pop-Location
        exit 1
    }

    Pop-Location
}

function Build {
    trap {
        Pop-Location -Stack BuildTemp -ErrorAction 'SilentlyContinue'
        Write-Error $_
        Log-Group
        exit 2
    }

    $ScriptHome = $PSScriptRoot
    $ProjectRoot = Resolve-Path -Path "$PSScriptRoot/../.."
    $BuildSpecFile = "${ProjectRoot}/buildspec.json"

    $UtilityFunctions = Get-ChildItem -Path $PSScriptRoot/utils.pwsh/*.ps1 -Recurse

    foreach($Utility in $UtilityFunctions) {
        Write-Debug "Loading $($Utility.FullName)"
        . $Utility.FullName
    }

    $BuildSpec = Get-Content -Path ${BuildSpecFile} -Raw | ConvertFrom-Json
    $ProductName = $BuildSpec.name
    $ProductVersion = $BuildSpec.version

    if ( ! $SkipDeps ) {
        Install-BuildDependencies -WingetFile "${ScriptHome}/.Wingetfile"
    }

    # Download and build Boost
    $BoostPath = Download-Boost -Destination $BoostDirectory
    Build-Boost -BoostPath $BoostPath

    Push-Location -Stack BuildTemp
    if ( ! ( ( $SkipAll ) -or ( $SkipBuild ) ) ) {
        Ensure-Location $ProjectRoot

        $CmakeArgs = @()
        $CmakeBuildArgs = @()
        $CmakeInstallArgs = @()

        if ( $VerbosePreference -eq 'Continue' ) {
            $CmakeBuildArgs += ('--verbose')
            $CmakeInstallArgs += ('--verbose')
        }

        if ( $DebugPreference -eq 'Continue' ) {
            $CmakeArgs += ('--debug-output')
        }

        $Preset = "windows-$(if ( $Env:CI -ne $null ) { 'ci-' })${Target}"

        $CmakeArgs += @(
            '--preset', $Preset
        )

        $CmakeBuildArgs += @(
            '--build'
            '--preset', $Preset
            '--config', $Configuration
            '--parallel'
            '--', '/consoleLoggerParameters:Summary', '/noLogo'
        )

        $CmakeInstallArgs += @(
            '--install', "build_${Target}"
            '--prefix', "${ProjectRoot}/release/${Configuration}"
            '--config', $Configuration
        )

        Log-Group "Configuring ${ProductName}..."
        Invoke-External cmake @CmakeArgs

        Log-Group "Building ${ProductName}..."
        Invoke-External cmake @CmakeBuildArgs
    }
    Log-Group "Install ${ProductName}..."
    Invoke-External cmake @CmakeInstallArgs

    Pop-Location -Stack BuildTemp
    Log-Group
}

Build
