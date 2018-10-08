function global:Do-Stuff
{
	[CmdletBinding()]
	param()

	Write-Host "Doing stuff!"

	throw "Fuck you"
}