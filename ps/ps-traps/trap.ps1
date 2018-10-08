function Fuk-Tis
{
	trap
	{
		Write-Host "local trap"
	}
}

<#
trap
{
	Write-Host "global trap"
}
#>

Fuk-Tis
sdf