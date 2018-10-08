# # # VARIANT 1): Constructor as a object method

$global:objectConstructor = New-Object PSObject

$objectConstructor | Add-Member NoteProperty BaseObjectType "objectConstructor"

$objectConstructor | Add-Member ScriptMethod Create {

    foreach ($arg in $args)
    {
        Write-Host $arg
    }
}

$objectConstructor.Create(
    "Button Login on Login As Admin Overlay",
    "button.connect_login_button"
)


# # # VARIANT 2): Object-specific cmdlet constructor
function global:Zen-New-BaseChromiumObject
{
<#
    param
    (
        [String]$ID,
        [String]$LoggingInformation,
        [Int]$Port,
        [Int]$ChromiumSessionAlias
    )
#>

    param
    (
        $inputObect,
        $members
    )

    Write-Host "Zen-New-BaseChromiumObject"

    $PSBoundParameters
}

$params = @{
    "LoggingInformation" = "Button Login on Login As Admin Overlay";
    "ID" = "button.connect_login_button";
}

Zen-New-BaseChromiumObject 
