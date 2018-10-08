New-Step "Check window appearance (ZEN) -Window {Window} -Image {Image} -MaskX {MaskX} -MaskY {MaskY}" {

    function local:Escape-BranchName
    {
        param(
            [Parameter(Mandatory=$true)]
            [String]$Name
        )
        return $Name.Replace('/','_') 
    }


    # # # Compose image paths
	$local:branchCurrent = (Escape-BranchName $RuntimeConfig.ZenBranch)
    $local:branchDefault = (Escape-BranchName "zen/devel")

    $local:imageNameBase = $Params.Image
	$local:imagePathNameBase = "imagecomparison"
	$local:imagePathSource = (Join-Path -Paths $TestingData,"ZEN","Image_comparison")
    $local:imagePathBaseSource = (Join-Path -Paths $imagePathSource,$__protocolName)
	$local:imagePathBaseLocal = (Join-Path -Paths $PWD,$imagePathNameBase)
	
	# Image from application windows
	$local:imageANameCurrent = $branchCurrent+"_"+$imageNameBase
	$local:imageAPathLocal = (Join-Path -Paths $imagePathBaseLocal,$imageANameCurrent)

	# Image from remote storage for comparison(expected)
	$local:imageBNameCurrent = $branchCurrent+"_"+$imageNameBase
	$local:imageBNameDefault = $branchDafeult+"_"+$imageNameBase
	$local:imageBPathSourceCurrent = (Join-Path -Paths $imagePathBaseSource,$branchCurrent,$imageBNameCurrent)
	$local:imageBPathSourceDefault = (Join-Path -Paths $imagePathBaseSource,$branchDefault,$imageBNameDefault)
	$local:imageBnameLocal = "expected_"+$imageBNameCurrent
	$local:imageBPathLocal = (Join-Path -Paths $imagePathBaseLocal,$imageBNameCurrent)

	# Image comparison difference
	$local:imageDiffNameCurrent = "difference_"+$imageNameBase
	$local:imageDiffPathLocal = (Join-Path -Paths $imagePathBaseLocal,$imageDiffNameCurrent)

	# Comparison masks
    $local:imageMaskXNameSource = $Params.MaskX
	$local:imageMaskXNameLocal = "maskX.png"
	$local:imageMaskYNameSource = $Params.MaskY
	$local:imageMaskYNameLocal = "maskY.png"
    $local:imageMaskXPathSource = ""
	$local:imageMaskXPathLocal = ""
	$local:imageMaskYPathSource = ""
	$local:imageMaskYPathLocal = ""


    # # # Check and prepare images for comparison
	# Check and prepare comparison masks

	# MaskX comparison mask
	if ($imageMaskXName -eq $imageNameBase)
	{
		# Mask is included in expected image(imageB)
		$imageMaskXPathLocal = $imageBPathLocal
	}
	else
	{
		# Mask is defined in external file
		$imageMaskXPathSource = (Join-Path -Paths $imagePathSource,$imageMaskXName)
		$imageMaskXPathLocal = (Join-Path -Paths $imagePathBaseLocal,$imageMaskXNameLocal)

		# And needs to be copied to local storage
		Net-Copy-Item $imageMaskXPathSource $imageMaskXPathLocal
	}

	# MaskY comparison mask
	if ($imageMaskYName -eq $imageNameBase)
	{
		# Mask is included in expected image(imageB)
		$imageMaskYPathLocal = $imageBPathLocal
	}
	else
	{
		# Mask is defined in external file
		$imageMaskYPathSource = (Join-Path -Paths $imagePathSource,$imageMaskYName)
		$imageMaskYPathLocal = (Join-Path -Paths $imagePathBaseLocal,$imageMaskYNameLocal)

		# And needs to be copied to local storage
		Net-Copy-Item $imageMaskYPathSource $imageMaskYPathLocal
	}


	# First, try expected image from current branch
	if (-not (Test-Path $imageBPathSourceCurrent))
    {
		# Second, try expected image from default branch
        if (-not (Test-Path $imageBPathSourceDefault))
		{
			# We require some image for comparison here, so end with exception
			throw ("Cannot find any image '$imageNameBase'")
		}

		# Copy expected image from default branch to current
		Net-Copy-Item $imageBPathSourceDefault $imageBPathSourceCurrent
    }

	# Make image comparison directory
	New-Item -Path $imagePathBaseLocal -ItemType "directory" -Force

	# Copy expected image to local storage
	Net-Copy-Item $imageBPathSourceCurrent $imageBPathLocal

	# Take screenshot of requested window
	$local:window = (Invoke-Command (Get-Variable -Name $Params.Window -Scope "global").Value.CustomPath)
	$local:windowBounds = ($window | Read-UIAControlBoundingRectangle)
	$local:windowRect = (New-Object System.Drawing.Rectangle($windowBounds.X, $windowBounds.Y, $windowBounds.Width, $windowBounds.Height))
	
	Log-Info ("Taking screenshot X:" + $windowRect.X + " Y:" + $windowRect.Y + " W:" + $windowRect.Width + " H:" + $windowRect.Height)
	
	# Set focus onto requested window
	$window | Set-UIAFocus

	# Save the screenshot of requested window as file
	(Get-Screenshot $windowRect).Save($imageAPathLocal)


	# # # Compare images and log results
	$local:difference = (Zen-Compare-Images -A $imageAPathLocal -B $imageBPathLocal -Diff $imageDiffPathLocal -MaskX $imageMaskXPathLocal -MaskY $imageMaskYPathLocal)

	# Attach all comparison files
	
}
