for ($index = 0; $index -lt $fileContent.Length; $index++)
{
    $fileContent[$index]

    if ($fileContent[$index] -match '\[(.+)\]')
    {
        $section = $Matches[1]
    }

    if ($fileContent[$index] -match 'Severity = (.+)')
    {
        if ($section -eq "SQLDB")
        {
            $fileContent[$index] = ($fileContent[$index] -replace 'Severity = Info', 'Severity = Detail')
        }
        else
        {
            $fileContent[$index] = ($fileContent[$index] -replace 'Severity = Info', 'Severity = Debug')

        }

        continue
    }

    if ($fileContent[$index] -match 'Size = \d+')
    {
        $fileContent[$index] = ($fileContent[$index]  -replace 'Size = \d+', 'Size = 102400000')

        continue
    }
}