function global:PrioritySort-Object
{
    param
    (
        [Parameter(Position=1, Mandatory=$true, ValueFromPipeline=$true, ValueFromPipelineByPropertyName=$true)]
        [PSObject]$InputObject,

	[Parameter(Position=0)]
        [ScriptBlock[]]$Priority
    )

    begin
    {
        # Append least priority
        $Priority = $Priority + @({ return $true })

        # Initialize variables
        $local:inputArray = New-Object System.Collections.ArrayList
        $local:outputArray = New-Object System.Collections.ArrayList
        $local:prioritizedArray = New-Object System.Collections.ArrayList
        for ($priorityLevel = 0; $priorityLevel -lt $Priority.Length; $priorityLevel += 1)
        {
            $prioritizedArray.Add((New-Object System.Collections.ArrayList)) | Out-Null
        }
    }

    process
    {
        # We will process the input array as a whole
        $inputArray.Add(($InputObject)) | Out-Null
    }

    end
    {
        # Append matched elements into prioritized array level
        foreach ($inputArrayElement in $inputArray)
        {
            # Simulate current element as if on pipeline
            $_ = $inputArrayElement

            # Resolve priority level for this element
            for ($priorityLevel = 0; $priorityLevel -lt $Priority.Length; $priorityLevel += 1)
            {
                if ($Priority[$priorityLevel].Invoke())
                {
                    $prioritizedArray[$priorityLevel].Add(($inputArrayElement)) | Out-Null
                    break
                }
            }
        }

        # Compose linear priority-sorted array
        foreach ($priorityLevelArray in $prioritizedArray)
        {
            # Sort each priority level array
            $priorityLevelArray = $priorityLevelArray | Sort-Object

            # Append to linear output array
            foreach ($element in $priorityLevelArray)
            {
                $outputArray.Add($element) | Out-Null
            }
        }

        return $outputArray
    }
}
