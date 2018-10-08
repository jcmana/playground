$global:root = New-Object PSObject

$root | Add-Member NoteProperty child (Invoke-Command {
    $obj = New-Object PSObject
    $obj | Add-Member NoteProperty val "value 123" 
    return $obj
})