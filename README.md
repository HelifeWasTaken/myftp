# myftp
An basic RFC959 Compliant fully documented in C

EPITECH MyFTP Protocol (RFC959 Extract)

You MUST have an "Anonymous" account with an empty password

Each bonus MUST be RFC959 compliant

The following are the mandatory FTP commands from client to server:

            USER <SP> <username> <CRLF>   : Specify user for authentication
            PASS <SP> <password> <CRLF>   : Specify password for authentication

            CWD  <SP> <pathname> <CRLF>   : Change working directory
            CDUP <CRLF>                   : Change working directory to parent directory

            QUIT <CRLF>                   : Disconnection

            DELE <SP> <pathname> <CRLF>   : Delete file on the server

            PWD  <CRLF>                   : Print working directory

            PASV <CRLF>                   : Enable "passive" mode for data transfer

            PORT <SP> <host-port> <CRLF>  : Enable "active" mode for data transfer


            HELP [<SP> <string>] <CRLF>   : List available commands

            NOOP <CRLF>                   : Do nothing

            (the following are commands using data transfer )

            RETR <SP> <pathname> <CRLF>   : Download file from server to client
            STOR <SP> <pathname> <CRLF>   : Upload file from client to server
            LIST [<SP> <pathname>] <CRLF> : List files in the current working directory 


Command-Reply Sequences (Mandatory part)

The server reply is defined to contain the 3-digit code, followed by Space <SP>, followed by one line of text, and terminated by the Telnet end-of-line code <CRLF>.

Example :

-> USER <SP> Username <CRLF>
<- 331 <SP> User name okay, need password. <CRLF>
-> PASS <SP> Password <CRLF>
<- 230 <SP> User logged in, proceed. <CRLF>

-> : Request command from client to server
<- : Reply from server to client


            Connection Establishment
               <- 220

            Login
               USER ->
                  <- 230
                  <- xxx
                  <- 331
               PASS ->
                  <- 230
                  <- 332
                  <- xxx
               CWD ->
                  <- 250
                  <- xxx
               CDUP ->
                  <- 200
                  <- xxx

            Logout
               QUIT ->
                  <- 221
                  <- xxx

            Transfer parameters
               PORT ->
                 <- 200
                 <- xxx
               PASV ->
                 <- 227
                 <- xxx

            File action commands
               STOR ->
                  <- 150
                     <- 226
                     <- xxx
                  <- xxx
               RETR ->
                  <- 150
                     <- 226
                     <- xxx
                  <- xxx
               LIST ->
                  <- 150
                     <- 226
                     <- xxx
                  <- xxx
               DELE ->
                  <- 250
                  <- xxx
               PWD ->
                  <- 257
                  <- xxx

            Informational commands
               HELP ->
                  <- 214
                  <- xxx

            Miscellaneous commands
               NOOP ->
                  <- 200
                  <- xxx
