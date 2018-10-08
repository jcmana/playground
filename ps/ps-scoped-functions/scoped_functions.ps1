function global:my-cmdlet
{
    param()
    Write-Host "Imma global"
}

function script:my-cmdlet
{
    param()
    Write-Host "I am not"
}

global:my-cmdlet
script:my-cmdlet