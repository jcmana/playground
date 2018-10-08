# # # CONCEPT #2



# Global autolib constraints
$global:ZenAutolibVersion = @{}



function global:Zen-Select-AutolibVersion
{
	param($Constraints)

	$global:ZenAutolibVersion = $Constraints
}

Zen-Select-AutolibVersion -Constraints @{
	"ZenBuildNumber" = "530";
}


function global:Version
{
	param(
		$Constraints,
		[ScriptBlock]$Code
	)

	# Check code constraints against global constraints
	foreach($cname in $Constraints.Keys)
	{
		# We require full regular match
		$re = '^' + $Constraints[$cname] + '$'

		Write-Host "Matching '" + $global:ZenAutolibVersion[$cname] + "' against '" + $re + "'"

		if ($global:ZenAutolibVersion[$cname] -notmatch $re)
		{
			return
		}
	}

	# Execute code
	. $Code
}


function global:Zen-Example-Cmdlet
{

    # Normal common code
    Write-Host "Executing common part of cmdlet 'Zen-Example-Cmdlet'"


    # Versioned part of cmdlet

    Version -Constraints @{"ZenBuildNumber" = "53[1-6]"} -Code {
        Write-Host "Executing part of cmdlet for versions ZenBuildNumber 531 to 536 only"
    }

    Version -Constraints @{"ZenBuildNumber" = "537"} -Code {
        Write-Host "Executing part of cmdlet for versions ZenBuildNumber 537"
    }

    # Rest of normal common code
    Write-Host "Executing another common part of cmdlet 'Zen-Example-Cmdlet'"

    return $true
}

Zen-Example-Cmdlet



function global:Zen-HumanExample-Cmdlet
{
	XP {
		# Code
		# ...
	}

	PSv2 {
		# Code
		#...
	}

	ZEN-2015-SU-01_GBN {
		# Code
		# ...
	}
}
