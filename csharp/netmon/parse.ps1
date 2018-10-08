# Load capture file
$captureFile = [System.IntPtr]::Zero
[Microsoft.NetworkMonitor.NetmonAPI]::NmOpenCaptureFile("F:/share/development/ps/netmon/auto.cap", [ref]$captureFile)

# Load some frame from capture file
$rawFrame = [System.IntPtr]::Zero
[Microsoft.NetworkMonitor.NetmonAPI]::