$listenerJob = Start-Job {

    $listener = New-Object System.Net.HttpListener
    $listener.Prefixes.Add("http://127.0.0.1:80/")

    $listener.Start()

    while ($listener.IsListening)
    {
        $context = $listener.GetContext()

        Write-Host $context.Request.Url

        # Send empty response (to respect HTTP procol flow)
        $responseBuffer = [System.Text.Encoding]::UTF8.GetBytes("")

        $context.Response.ContentLength64 = $responseBuffer.Length
        $context.Response.OutputStream.Write($responseBuffer, 0, $responseBuffer.Length)

        # HTTP listener STOP request
        if ($context.Request.Url.LocalPath -eq "/stop")
        {
            break
        }
    }

    $listener.Stop()
}

$listenerJob | Wait-Job | Remove-Job