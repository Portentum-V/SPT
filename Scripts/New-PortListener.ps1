function New-Portlistener {
    param (
        [parameter(Mandatory = $true, HelpMessage = "Enter the port you want to use to listen on")]
        [ValidatePattern('^[0-9]+$')]
        [ValidateRange(0, 65535)]
        [int]$Port,

        [parameter(ParameterSetName="tcp")]
        [switch]$TCP,

        [parameter(ParameterSetName="udp")]
        [switch]$UDP
    )

    if ((-not $TCP) -and (-not $UDP)) {
        $TCP = $true
    }

    if ($TCP) { $Proto = "TCP" }
    if ($UDP) { $Proto = "UDP" }

    try {
        $endpoint = new-object System.Net.IPEndPoint([ipaddress]::Any, $Port)
        write-host "Endpoint: $endpoint" -ForegroundColor Yellow 
    }
    catch {
        $_
        "Failed to create IPEndPoint"
        return
    }

    try {
        if ($TCP) {
            $listener = new-object System.Net.Sockets.TcpListener $endpoint
            $listener.start()
        }
        elseif ($UDP) {
            $listener = new-object System.Net.Sockets.UdpClient $Port
            $listener.Client.ReceiveTimeout = 100
        }
    }
    catch {
        $_
        "Failed to setup $Proto listener"
        return
    }

    write-host "Listener: $listener" -ForegroundColor Yellow 

    Write-Host ("Started listening on {0}:{1}" -f $Proto, $Port) -ForegroundColor Cyan
    Write-Host ("Press Escape to stop") -ForegroundColor Green

    try {
        while ( $true ) {
            if ($host.ui.RawUi.KeyAvailable) {
                $key = $host.ui.RawUI.ReadKey("NoEcho,IncludeKeyUp,IncludeKeyDown")
                if ($key.VirtualKeyCode -eq 27 ) {
                    if ($TCP) { $listener.stop() }
                    elseif ($UDP) { $listener.close() }
                    Write-Host ("Stopped listening on {0}:{1}" -f $Proto, $Port) -ForegroundColor Red
                    return
                }
            }

        try { # Socket Timeout is ok
            if ($TCP) {
                $client = $listener.AcceptTcpClient()
                $stream = $client.GetStream()
                $bytes = New-Object System.Byte[] 1024
                
                while (($i = $stream.Read($bytes,0,$bytes.Length)) -ne 0) {
                    $EncodedText = New-Object System.Text.ASCIIEncoding
                    $content = $Encodedtext.GetString($bytes, 0, $i)
                    Write-Host $content
                }
                $stream.close()
            }

            if ($UDP) {              
                    $content = $listener.Receive( [ref]$endpoint )
                    Write-Host "$([Text.Encoding]::ASCII.GetString($content))"
            }
        }
        catch [System.Net.Sockets.SocketException]
        {
            if ( $_.Exception.SocketErrorCode -ne 'TimedOut') {
                throw
            }
        }

        }
    }
    catch {
        $_
        return
    }
}
