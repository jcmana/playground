param(
    [Parameter(Mandatory=$true,ParameterSetName="name")]
    [String]$ProcessName,

    [Parameter(Mandatory=$true,ParameterSetName="id")]
    [Int]$ProcessID,

    [Parameter(ParameterSetName="name")]
    [Parameter(ParameterSetName="id")]
    [String[]]$Watch = @("Timestamp","PrivateMemorySize"),

    [Parameter(ParameterSetName="name")]
    [Parameter(ParameterSetName="id")]
    [Int]$Interval = 60
)

# # # Resolving requested process
try
{
    switch($PSCmdlet.ParameterSetName)
    {
        "name" { $local:process = (Get-Process -Name $ProcessName) }
        "id"   { $local:process = (Get-Process -Id $ProcessID) }
    }
}
catch
{
    Write-Host "Process cannot be resolved..."
    exit
}


# # # Resolve output file name
$local:output_filename = ($local:process.ProcessName + "_" + (Get-Date -Format "yyyyMMdd-HHmmss") + ".csv")
$local:output_filepath = (Join-Path $PWD $local:output_filepath) 


# # # Print readings header to output file
$local:result_line = ""
foreach($reading_request in $Watch)
{
    # Get reading value from process object

    # Append reading to results line
    $local:result_line += ([String]$reading_request) + ","
}
# Remove last comma char
$local:result_line = $local:result_line.TrimEnd(",")

# Append collected statistics to file
$local:result_line | Out-File -FilePath $local:output_filename -Append


# # # Start monitor the process
Write-Host "Watching process '$($local:process.ProcessName)' ..."
while($true)
{
    $local:process = $null

    # Resolving requested process
    switch($PSCmdlet.ParameterSetName)
    {
        "name" { $local:process = (Get-Process -Name $ProcessName) }
        "id"   { $local:process = (Get-Process -Id $ProcessID) }
    }

    # Was process found?
    if($local:process)
    {
        # Collect statistics
        $local:result_line = ""
        foreach($reading_request in $Watch)
        {
            switch($reading_request)
            {
                "Timestamp" {
                    # Get current datetime
                    $reading_value = (Get-Date -Format "yyyy-MM-dd HH:mm:ss")
    
                    # Append reading to results line
                    $local:result_line += ([String]$reading_value) + ","
                }
    
                default {
                    # Get reading value from process object
                    $reading_value = $local:process.$reading_request
    
                    # Append reading to results line
                    $local:result_line += ([String]$reading_value) + ","
                }
            }
        }
    
        # Remove last comma char
        $local:result_line = $local:result_line.TrimEnd(",")
    
        # Append collected statistics to file
        $local:result_line | Out-File -FilePath $local:output_filename -Append
    }

    # Sleep for requested time before taking readings again
    Sleep -Seconds $Interval
}