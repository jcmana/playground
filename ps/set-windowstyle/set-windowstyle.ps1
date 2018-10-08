function global:Zen-Set-WindowState
{
    param
    (
        $state = 'SHOW',
        $mainWindowHandle = (Get-Process –id $pid).MainWindowHandle
    )

    $WindowStates = @{
        'FORCEMINIMIZE'   = 11
        'HIDE'            = 0
        'MAXIMIZE'        = 3
        'MINIMIZE'        = 6
        'RESTORE'         = 9
        'SHOW'            = 5
        'SHOWDEFAULT'     = 10
        'SHOWMAXIMIZED'   = 3
        'SHOWMINIMIZED'   = 2
        'SHOWMINNOACTIVE' = 7
        'SHOWNA'          = 8
        'SHOWNOACTIVATE'  = 4
        'SHOWNORMAL'      = 1
    }

    $local:memberDefinition = '[DllImport("user32.dll")] public static extern bool ShowWindowAsync(IntPtr hWnd, int nCmdShow);'

    $Win32ShowWindowAsync = Add-Type -name "Win32ShowWindowAsync" -namespace Win32Functions -passThru -memberDefinition $memberDefinition
    $Win32ShowWindowAsync::ShowWindowAsync($mainWindowHandle, $WindowStates[$state]) | Out-Null
}