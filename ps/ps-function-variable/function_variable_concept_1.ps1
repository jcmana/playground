# # # CONCEPT #1
function Zen-Define-Step {}
function Zen-Define-Cmdlet {}
function Zen-Define-Mapping { 
	param($Name)

	Write-Host $Name
	Write-Host $Value
	Write-Host $Constraints
}

function Zen-Select-Autolib {}

$global:ZenAutolib = @{
	"cmdlet" = @();
	"step" = @();
	"object" = @();
}


Zen-Define-Cmdlet 'Zen-Version-Function' {

	Write-Host "Function version 1" 
}

Zen-Define-Step 'Call ZEN function (ZEN)' {
	
	Zen-Version-Function
}

Zen-Define-Mapping `
	-Name 'ZenUI.ButtonClose' `
	-Value {
		$Id = "#btn_close"
	} `
	-Constraints @{
		"ZenBuildNumber" = "531";

	}


Zen-Select-Autolib -Constraints @{
	"ZenBuildNumber" = "511";
	"FmwBuildNumber" = "375";
}