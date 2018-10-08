$string = "this is string which represent some random string representing string variable content in powershell"

#$file = "F:\share\development\ps-encoding\output.txt"
$file = (Join-Path $PWD "output.txt")

$encoding = New-Object System.Text.UTF8Encoding($false)
[System.IO.File]::WriteAllText($file,$string,$encoding)