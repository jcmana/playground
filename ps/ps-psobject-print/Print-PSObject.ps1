function global:Print-PSCustomObject
{
    param
    (
        [Parameter(Mandatory=$true, ValueFromPipeline=$true, ValueFromPipelineByPropertyName=$true)]
        [PSObject]$InputObject,
        [Int]$Depth = 0
    )

    $psobjectUserMembers = ( $InputObject.psobject.Members | Where-Object { $_.MemberType -in "NoteProperty", "ScriptProperty" } )

    foreach ($psobjectMember in $psobjectUserMembers)
    {
        $member = $InputObject.($psobjectMember.Name)

        if ( $member -is [PSObject])
        {
            Write-Host ("|    " * $Depth) -NoNewLine
            Write-Host ("+ " + $psobjectMember.Name)
            $member | Print-PSCustomObject -Depth ($Depth + 1)
        }
        else
        {
            Write-Host ("|    " * ($Depth + 1)) -NoNewLine
            Write-Host ("+ " + $psobjectMember.Name) -NoNewLine
            Write-Host ($psobjectMember.Name + " = " + $member)
        }
    }
}