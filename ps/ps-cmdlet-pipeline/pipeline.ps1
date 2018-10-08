function Test-Pipeline
{
    param
    (
        [Parameter(Mandatory=$true, ValueFromPipeline=$true)]
        [String]$Value,
        [String]$Name
    )

    Write-Host "Name = '$Name'"
    Write-Host "Value = '$Value'"
}



function Test-FullPipeline
{
    param
    (
        [Parameter(Mandatory=$true, ValueFromPipeline=$true)]
        [Object]$Value,
        [String]$Name
    )

    begin
    {
        Write-Host "= begin ="
        Write-Host "Name = '$Name'"
    }

    process
    {
        Write-Host "= process ="
        Write-Host "Value = '$Value'"
        Write-Host "Name = '$Name'"
    }

    end
    {
        Write-Host "= end ="
        Write-Host "Name = '$Name'"
    }
}


# # # Simple pipeline cmdlet
"1123asdf" | Test-Pipeline -Name "Asdf"
Write-Host

Test-Pipeline -Value "Asdf"
Write-Host

Test-Pipeline -Value "Asdf" -Name "trtr"
Write-Host

# # # Pipeline input with both parameters
"123dsf" | Test-FullPipeline -Name 241635468654465645
Write-Host

# # # Usage in foreach
$q = "lol", "123dsf"
foreach ($ble in $q)
{
    Test-FullPipeline -Value $ble
} 
Write-Host

# Calling by parameter name
Test-FullPipeline -Value "lol"
Test-FullPipeline -Value "123dsf"
Write-Host

# Enumerable
$q | Test-FullPipeline
Write-Host