function global:Zen-Start-Process
{
    param
	(
        [Parameter(Position = 1, Mandatory = $true)]
        [String]$FilePath,
        [Parameter(Position = 2, Mandatory = $false)]
        $ArgumentList = $null,
        [System.Management.Automation.PSCredential]$Credential = $null,
        [Switch]$LoadUserProfile,
        [Switch]$NoNewWindow,
        [Switch]$PassThru,
        $RedirectStandardError = $null,
        $RedirectStandardInput = $null,
        $RedirectStandardOutput = $null,
        [Switch]$UseNewEnvironment,
        $Verb = $null,
        $WindowStyle = $null,
        [String]$WorkingDirectory = $PWD,
        [Switch]$Wait,
        [Int32]$Timeout = -1
	)

	Start-Process @PSBoundParameters
}
    