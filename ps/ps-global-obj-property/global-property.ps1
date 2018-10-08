$global:pipelineObject = "text"

$script:object = New-Object PSObject

$script:object | Add-Member ScriptProperty ChromiumSession {
    $global:pipelineObject = "jiny text"
    return $global:pipelineObject
}

$script:object