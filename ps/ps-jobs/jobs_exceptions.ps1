$job = Start-Job {

    Write-Output "Before job exception"
    throw "Job failure"
    Write-Output "After job exception"
}

$job | Receive-Job