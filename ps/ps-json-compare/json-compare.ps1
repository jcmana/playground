function local:Compare-JsonObject
{
    [CmdletBinding()]
    param(
        [PSObject]$ObjectA,
        [PSObject]$ObjectB
    )

    $DebugPreference = "Continue"; 

    # Variable for overall comparison result
    $local:comparisonResult = $true

    # Skip comparison of NULL objects
    if (($objectA -eq $null) -or ($objectB -eq $null))
    {
        if (($objectA -eq $null) -and ($objectB -eq $null))
        {
            return $true
        }

        return $false
    } 

    # Determine type of input objects
    $local:objectAType = $ObjectA.GetType()
    $local:objectBType = $ObjectB.GetType()

    Write-Debug "ObjectA type: $($objectAType.Name)"
    Write-Debug "ObjectB type: $($objectBType.Name)"

    # Both object have to have same type
    if ($objectAType.Name -ne $objectBType.Name)
    {
        return $false
    }

    # Choose comparison method according object's type
    switch -regex ($objectAType.Name)
    {
        "^PSCustomObject$"
        {
            # Get member list from both objects
            $local:memberListObjectA = $ObjectA.psobject.Properties | Where-Object {$_.MemberType -eq 'NoteProperty'}
            $local:memberListObjectB = $ObjectB.psobject.Properties | Where-Object {$_.MemberType -eq 'NoteProperty'}
        
            if ($memberListObjectA.Count -ne $memberListObjectB.Count)
            {
                Write-Debug "Object differs in size"
    
                $comparisonResult = $false
                break
            }

            # Compare immediate (first-level) members from ObjectA ...
            Write-Debug "Looking for equivalent to 'ObjectA.$($memberA.Name)'"

            foreach ($memberA in $memberListObjectA)
            {
                # Find equivalent member in ObjectB
                $memberB = $memberListObjectB | Where-Object {$_.Name -eq $memberA.Name}
        
                if ($memberB -eq $null)
                {
                    Write-Debug "No equivalent member for 'ObjectA.$($memberA.Name)'"

                    $comparisonResult = $false
                    continue
                }

                Write-Debug "Comparing 'ObjectA.$($memberA.Name)' with 'ObjectB.$($memberB.Name)'"

                # Compare JSON subtree
                $local:entryComparisonResult = Compare-JsonObject $memberA.Value $memberB.Value
        
                # No equivalent subtree found
                if (-not $entryComparisonResult)
                {
                    Write-Debug "No equivalent member for 'ObjectA.$($memberA.Name)'"

                    $comparisonResult = $false
                    break
                }
            }

            break
        }

        "^Object\[\]$"
        {
            if ($ObjectA.Count -ne $ObjectB.Count)
            {
                Write-Debug "Object differs in size"

                $comparisonResult = $false
                break
            }

            for ($local:memberIndex = 0; $memberIndex -lt $ObjectA.Count; $memberIndex++)
            {
                # Compare JSON subtree
                $local:entryComparisonResult = Compare-JsonObject $ObjectA[$memberIndex] $ObjectB[$memberIndex]

                # No equivalent subtree found
                if (-not $entryComparisonResult)
                {
                    Write-Debug "No equivalent member for 'ObjectA.$($memberA.Name)'"

                    $comparisonResult = $false
                    break
                }
            }

            break
        }

        "^String|Int|Int32|Decimal|Boolean$"
        {
            $comparisonResult = ($objectA -eq $objectB)

            break
        }


        default
        {
            Write-Error "Unknown subtree type '$($objectAType.Name)'"
        }
    }

    return $comparisonResult
}

$a = (ConvertFrom-Json (Get-Content "a.json" -Raw))
$b = (ConvertFrom-Json (Get-Content "b.json" -Raw))

$c = (ConvertFrom-Json (Get-Content "c.json" -Raw))
$d = (ConvertFrom-Json (Get-Content "d.json" -Raw))


Compare-JsonObject $a $b -Debug
Compare-JsonObject $c $d
