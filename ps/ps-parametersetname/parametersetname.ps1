function parametersetname
{
    param(
        [Parameter(ParameterSetName="name", Position=0)]
        [String]$Name,

        [Parameter(ParameterSetName="id", Position=0)]
        [Int[]]$Id
    )

    Write-Host $PsCmdlet.ParameterSetName
}

parametersetname 164, 456, 46