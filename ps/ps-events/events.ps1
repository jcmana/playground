$s = (New-PSSession -Computername "Server01", "Server02")

Invoke-Command -Session $s -Filepath "ProcessCreationEvent.ps1"
Invoke-Command -Session $s { Get-Event } # ProcessCreationEvent.ps1

function Enable-ProcessCreationEvent{
    $query = (New-Object System.Management.WqlEventQuery "__InstanceCreationEvent", (New-Object TimeSpan 0,0,1), "TargetInstance isa 'Win32_Process'")
    
    $processWatcher = (New-Object System.Management.ManagementEventWatcher $query)
    
    $identifier = "WMI.ProcessCreated"

    Register-ObjectEvent -input $processWatcher -eventName "EventArrived" -sourceIdentifier $identifier -messageData "Test" -forward
}

Enable-ProcessCreationEvent