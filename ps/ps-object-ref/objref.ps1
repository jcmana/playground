function script:somecmdlet
{
    param($objref)

    $objref | Add-Member NoteProperty decko (New-Object PSObject)
    $objref | Add-Member NoteProperty hodnota "Vlassy na assy"
}

$local:object = (New-Object PSObject)
$local:object | Add-Member NoteProperty hlavni "Nemam nechcu"

somecmdlet($local:object)
somecmdlet($local:object.decko)

$local:object.hlavni
(Invoke-Expression '$local:object').hodnota
(Invoke-Expression '$local:object.decko').hodnota