# Job result variable
$localvar = "neco"

Write-Host "Starting Background job"

$bgjob = Start-Job {

    Write-Output @{"JobLogInfo" = "Code running in Background job. Should not be visible in main session"}
	Sleep -Second 1
	Write-Output @{"JobLogInfo" = "Quitting Background job"}
    Write-Output @{"JobResult" = $true}
}

Write-Host "Receiving data from Background job"

<#
$bgjob | Wait-Job | Receive-Job | Foreach-Object {

    $_.GetType()
    return

    $jobOutputEnumerator = $_.GetEnumerator()
    while ($jobOutputEnumerator.MoveNext())
    {
        switch ($jobOutputEnumerator.Key)
        {
            "JobLogInfo" { Write-Host $jobOutputEnumerator.Value }
            "JobResult" { Write-Host $jobOutputEnumerator.Value }
        }
    }
}
#>

Write-Host "Zen-Receive-Job"
$bgjobresult = $bgjob | Wait-Job | Zen-Receive-Job

if ($bgjobresult)
{
	Write-Host "Success"
}
else
{
	Write-Host "Failure"
}

Write-Host "Starting Background job which should timeout"

$bgjob = Start-Job {

	Write-Output @{"JobLogInfo" = "Background job sleeping for 15s"}
	Sleep -Second 15
	Write-Output @{"JobLogInfo" = "Background job should timeout before this"}

	Write-Output @{"JobResult" = $true}
}

Write-Host "Receiving data from Background job which should timeout during"
$bgjob | Wait-Job -Timeout 5

if ($bgjob.State -eq "Running")
{
    $bgjob | Zen-Receive-Job
    throw "Job timed out"
}

$bgjob | Zen-Receive-Job
