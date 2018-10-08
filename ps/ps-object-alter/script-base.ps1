$script:base = (PSObject-Clone-Object $global:base)

$script:base.common = "lol"

Write-Host $global:base.common
Write-Host $script:base.common