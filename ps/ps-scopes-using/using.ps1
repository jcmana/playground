$localvar = "hovno"


Write-Host $localvar
Write-Host $using:localvar

# A Using variable cannot be retrieved. A Using variable can be used only with Invoke-Command, Start-Job, or
# InlineScript in the script workflow. When it is used with Invoke-Command, the Using variable is valid only if
# the script block is invoked on a remote computer.