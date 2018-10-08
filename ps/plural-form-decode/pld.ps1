function Zen-Match-PluralForm
{
    param
    (
        [String]$pluralForm,
        [String]$text,
        [Int]$number
    )

    # Parse full plural-form expression
    if ($pluralForm -match '^([^,]+)((,\[\d\]:e:([^,]+))+)$')
    {
        # Replace variables with general matching regexp
        $pluralFormBase = ($Matches[1]  -replace '\{\{[a-zA-Z0-9]+\}\}', '(.+)')

        # Parse plural-form definition
        $pluralFormList = $Matches[2].Split(",")
        $pluralFormDefinitionList = @{}
        foreach ($pluralFormExpression in $pluralFormList)
        {
            if ($pluralFormExpression -match '^\[(\d)\]:e:([^,]+)$')
            {
                $pluralFormDefinitionList[$Matches[1]] = $Matches[2]
            }
        }

        # Find most fitting plural-form
        [String]$selectedPluralFormNumber = $number
        $selectedPluralFormNumberDifference = $number
        foreach ($pluralFormDefinitionNumber in $pluralFormDefinitionList.Keys)
        {
            $difference = ($number - ([Int]$pluralFormDefinitionNumber))
            if (($difference -lt $selectedPluralFormNumberDifference) -and ($difference -ge 0))
            {
                [String]$selectedPluralFormNumber = $pluralFormDefinitionNumber
                $selectedPluralFormNumberDifference = $difference
            }
        }

        $selectedPluralForm = $pluralFormDefinitionList[$selectedPluralFormNumber]

        # Match resolved plural-form against source text
        $pluralFormCompare = "^$pluralFormBase$selectedPluralForm`$"

        $text
        $pluralFormCompare

        if ($text -cmatch $pluralFormCompare)
        {
            return $true
        }
    }

    return $false
}

Zen-Match-PluralForm 'Expires in {{daysToExpire}} ,[0]:e:day,[1]:e:days,[5]:e:dayum' 'Expires in 3 days' 15