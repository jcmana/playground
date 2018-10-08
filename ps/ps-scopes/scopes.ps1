function func1
{
    param($sb)
    
    & $sb
}

function func2
{
    $variable
}

$local:variable = 100

func1 { $variable = 200 }

$variable