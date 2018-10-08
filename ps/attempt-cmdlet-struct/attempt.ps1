function global:Make-Attempt
{
    param (
        [Parameter(DontShow=$true)]
        $attemptNumber = 0

        [Parameter(DontShow=$true)]
        $attemptResult
    )

    # Current attempt result variable
    $attemptResult = @{
        "caseList" = @();
        "caseResults" = @();
    }

    # Do attempt code for this number
    # (this code should variate over current number of attempt
    switch ($attempt)
    {
        {$_ -lt 2 }
        {
            "less than two"
            break       # break is required in each case
        }

        { $_ -ge 2 -and $_ -lt 10 }
        {
            "greater or equal two and less than ten"
            break       # break is required in each case
        }

        # Terminating condition for maximum number of attempts
        default
        {
            return
        }
    }


    # Do attempt itself
    "attemptating"

    # Terminating condition
    # ...

    # Next attempt
    Make-Attempt -attempt ($attempt + 1)
}