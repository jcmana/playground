function Get-IniContent
{
    [CmdletBinding()]
    param
    (
        [ValidateNotNullOrEmpty()]
        [ValidateScript({(Test-Path $_) -and ((Get-Item $_).Extension -eq ".ini")})]
        [Parameter(ValueFromPipeline=$true, Mandatory=$true)]
        [String]$FilePath
    )

    Process
    {
        Write-Verbose "$($MyInvocation.MyCommand.Name):: Processing file: $Filepath"

        $ini = @{}
        switch -regex -file $FilePath
        {
             # Section
            "^\[(.+)\]$" {
                $section = $matches[1]
                $ini[$section] = @{}
                $CommentCount = 0
            }

            # Comment
            "^(;.*)$" {
                if (!($section))
                {
                    $section = "No-Section"
                    $ini[$section] = @{}
                }

                $value = $matches[1]
                $CommentCount = $CommentCount + 1
                $name = "Comment" + $CommentCount
                $ini[$section][$name] = $value
            }

            # Key
            "(.+?)\s*=\s*(.*)" {
                if (!($section))
                {
                    $section = "No-Section"
                    $ini[$section] = @{}
                }

                $name,$value = $matches[1..2]
                $ini[$section][$name] = $value
            }
        }

        Write-Verbose "$($MyInvocation.MyCommand.Name):: Finished Processing file: $FilePath"
        return $ini
    }
}