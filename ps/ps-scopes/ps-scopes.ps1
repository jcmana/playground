$local:local = "value"

# scope
foreach($i in 1)
{
    $local:local = "different value"

    Write-Host $local:local
}

Write-Host $local:local


# # # # # # # # # # # # # # # # 

$local:local = "value"

1 | ForEach-Object {
    $local:local = "different value"

    Write-Host $local:local
}

Write-Host $local:local