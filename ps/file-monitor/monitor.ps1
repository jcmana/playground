function global:Monitor-File
{
    <#
        .Description
            Watches changes in file and writes them line-by-line in real-time.
    #>
    param
    (
        [Parameter(Position=0, Mandatory=$true)]
        [String]$Path
    )

    if (-not (Test-Path $Path))
    {
        Write-Error "Couldn't find path '$Path'"
    }

    $pathParent = Get-Item $Path | Split-Path -Parent
    $pathLeaf   = Get-Item $Path | Split-Path -Leaf

    # Setup FileSystemWatcher for our file
    $fsw = New-Object IO.FileSystemWatcher @($pathParent, $pathLeaf) -Property @{IncludeSubdirectories = $false; NotifyFilter = [IO.NotifyFilters]'FileName, LastWrite'}

    Register-ObjectEvent $fsw Deleted -SourceIdentifier FileDeleted -Action {

        Write-Host "deleted"

        exit
    }

    Register-ObjectEvent $fsw Changed -SourceIdentifier FileChanged -Action {

        Write-Host "changed"

        # Load current content
        $currentContent = Get-Content $global:currentPath -Raw

        # Output new content
        for ($global:currentChar; $global:currentChar -lt $currentContent.Length; $global:currentChar++)
        {
            Write-Host ($currentContent[$global:currentChar]) -NoNewLine
        }
    }

    # # # Main application body
    # Wrapped in try-finally to ensure that Unregister-Event cmdlets are always called
    try
    {
        $currentContent = Get-Content $Path -Raw

        $global:currentPath = $Path
        $global:currentChar = $currentContent.Length

        Clear-Host

        # Just loop to make it look like application is running
        while ($true)
        {
            Sleep -Milliseconds 500
        }
    }
    finally
    {
        Unregister-Event FileDeleted
        Unregister-Event FileChanged

        Remove-Variable -Name "currentPath" -Scope "global"
        Remove-Variable -Name "currentChar" -Scope "global"
    }
}
