function global:Zen-Compare-Images
{
	param(
		[Parameter(Mandatory=$true)]
		[String]$A,

		[Parameter(Mandatory=$true)]
		[String]$B,

		[Parameter(Mandatory=$true)]
		[String]$Diff,

		[Parameter(Mandatory=$false)]
		[String]$MaskX,

		[Parameter(Mandatory=$false)]
		[String]$MaskY
	)

	# # # Prepare: Image comparison utility

    # Construct paths for comparison utility
    $local:imgcmpPath = "C:\testing\utils"
    $local:imgcmpExecutableName = "imgcmp_1.2.exe"
    $local:imgcmpExecutablePath = (Join-Path -Paths $imgcmpPath,$imgcmpExecutableName)
    $local:imgcmpSourcePath = (Join-Path -Paths $TestingData,"ZEN","ZenImgCmp",$imgcmpExecutableName)

    # Check(and download) the image comaparison utility
    if (-not (Test-Path -Path $imgcmpExecutablePath -PathType "leaf"))
    {
		Log-Info "Downloading 'imgcmp' from '$imgcmpSourcePath' to '$imgcmpPath'"

        # Download executable
        Net-Copy-Item -Source $imgcmpSourcePath -Dest $imgcmpExecutablePath
    }


	# # # Prepare: Image files

	# Imagepath regex
	$local:re_imagefile = '^([0-9a-zA-Z\-_]+)\.(png|jpg|bmp)$'
	$local:re_imagepath = '^((?:.+\\)?)(([0-9a-zA-Z\-_]+)\.(png|jpg|bmp))$'
	$local:re_imagemask = '^(([1-9][0-9]*),([1-9][0-9]*),([1-9][0-9]*),([1-9][0-9]*);)+$'

	# Imagepath declarations
	$local:localpath_imageBase = (Join-Path -Paths $PWD,"imagecomparison")
	$local:localpath_imageA = ""
	$local:localpath_imageB = ""
	$local:localpath_imageDiff = ""
	$local:localpath_imageMaskX = ""
	$local:localpath_imageMaskY = ""

	# Check parameters
	if ($A -match $re_imagepath)
	{
		$localpath_imageA = (Join-Path -Paths $localpath_imageBase,$Matches[2])
	}
	else
	{
		throw ("A: '$A' is not a valid path")
	}

	if ($B -match $re_imagepath)
	{
		$localpath_imageB = (Join-Path -Paths $localpath_imageBase,$Matches[2])
	}
	else
	{
		throw ("B: '$B' is not a valid path")
	}

	if ($Diff -match $re_imagefile)
	{
		$localpath_imageDiff = (Join-Path -Paths $PWD,$Diff)
	}
	else
	{
		throw ("Diff: '$Diff' is not a valid filename")
	}

	if ($MaskX)
	{
		if ($MaskX -match $re_imagepath)
		{
			$localpath_imageMaskX = (Join-Path -Paths $localpath_imageBase,$Matches[2])
		}
		else
		{
			throw ("MaskX: '$MaskX' is not a valid path")
		}
	}

	if ($MaskY)
	{
		if ($MaskY -match $re_imagepath)
		{
			$localpath_imageMaskY = (Join-Path $localpath_imageBase $Matches[2])
		}
		else
		{
			throw ("MaskY: '$MaskY' is not a valid path")
		}
	}

	# Make image comparison directory
	New-Item -Path $PWD -Name "imagecomparison" -ItemType "directory" -Force

	# Copy images to local storage
	Copy-Item $A $localpath_imageA
	Copy-Item $B $localpath_imageB
	if ($MaskX)
	{
		Copy-Item $MaskX $localpath_imageMaskX
	}
	if ($MaskY)
	{
		Copy-Item $MaskY $localpath_imageMaskY
	}

	
	# # # Execute the image comparison
	$local:imgcmpArguments = "$localpath_imageA $localpath_imageB $localpath_imageDiff $localpath_imageMaskX $localpath_imageMaskY"

	Log-Info ("Executing image comparison utility: '$imgcmpExecutablePath $imgcmpArguments'")
	$local:process = (Start-Process -FilePath $imgcmpExecutablePath -ArgumentList $imgcmpArguments -Wait -PassThru -NoNewWindow)

	# Cleanup
	Remove-Item -Path "imagecomparison" -Recurse 

	# Return number of different pixels
	return $process.ExitCode
}