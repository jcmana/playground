# # # Universal deploy macro for AVG products # # #
param
(
    [String]$Path = '\\sonus\build\zen\package\2015_0904_105649_82179_ZEN-2016-Beta'
)


function local:Execute-ActionCopy
{
    param
    (
        [Parameter(Position=0)]
        [String]$Source,
        [Parameter(Position=1)]
        [String]$Destination,
        [Parameter(Position=2)]
        [String]$Re
    )

    # Value from included_products.ini
    if ($Source -cmatch '^INI\.([a-zA-Z-0-9]+)\.([a-zA-Z-0-9_]+)$')
    {
        $Source = $script:includedProducts.($Matches[1]).($Matches[2])
    }

    # Select files from sorce
    $files = Get-ChildItem $Source

    # Apply regex filter
    if (($re -ne $null) -and ($re -ne ""))
    {
        $files = ( $files | Where-Object { $_.Name -cmatch $re } )
    }

    # Copy files to destination
    foreach ($f in $files)
    {
        Write-Host "$f > $Destination"
    }
}

function local:Execute-ActionMove
{

}

function local:Execute-ActionRemove
{

}

function local:Execute-ActionVariable
{
    param
    (
        [Parameter(Position=0)]
        [String]$Name,
        [Parameter(Position=1)]
        [String]$Value
    )

    # Check if user variable storage exists
    if ($script:userVariables -eq $null)
    {
        $script:userVariables = @{}
    }

    # Set variable value
    $script:userVariables[$Name] = $Value
}

function local:Resolve-String
{
    param
    (
        [Parameter(Position=0)]
        [String]$String
    )

    $local:re_variableName = '%([^%]+)%'
    $local:variableMatchList = ($String | Select-String -Pattern $re_variableName -AllMatches)

    # Make every variable substitution
    foreach ($variableMatch in $variableMatchList.Matches)
    {
        $local:variableName = $variableMatch.Groups[1]

        # Check whether variable exists
    }
}

# Load additional libraries
. ".\get-inicontent.ps1"

# Login to build and sonus fileservers
$user = "CZ\tester"
$password = "test"
#net use '\\build.cz.avg.com\disk_l\' $password /USER:$username
#net use '\\sonus.cz.avg.com\build\' $password /USER:$username

# Load deploy definition XML
$script:deployDefinition = New-Object System.Xml.XmlDocument
$script:deployDefinition.Load((Join-Path $PWD "deployDefinition_v3.xml"))

# Check and load included_products.ini
$includedProductsPath = (Join-Path $Path "included_products.ini")

if (Test-Path $includedProductsPath)
{
    $script:includedProducts = Get-IniContent $includedProductsPath
}

# Print
foreach($node in $deployDefinition.deploy.actions.ChildNodes)
{
    # Get action name
    $action = $node.Name

    # Call cmdlet for current action
    switch ($action)
    {
        "copy"
        {
            Execute-ActionCopy -Source $node.source -Destination $node.destination -Re $node.re
            break
        }

        "move"
        {
            Execute-ActionMove
            break
        }

        "remove"
        {
            Execute-ActionRemove
            break
        }

        "var"
        {
            Execute-ActionVariable
            break
        }

        "genctf"
        {

        }

        "#comment"
        {
            break
        }

        default {
            Write-Host "Unknown action '$action'" -ForegroundColor "red"
            break
        }
    }
}