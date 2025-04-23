https://xtermjs.org/
https://github.com/xtermjs/xterm.js

Overview
Frontend: xterm.js terminal embedded in a webpage.

Backend: Node.js server with WebSocket that accepts commands from the terminal but only executes ls /tmp regardless of input.

Security: The backend ignores any input other than ls and always runs ls /tmp to prevent arbitrary command execution.

Step 1: Frontend (HTML + JavaScript)
xml
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8" />
  <title>Jailed Web Terminal</title>
  <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/xterm/css/xterm.css" />
  <script src="https://cdn.jsdelivr.net/npm/xterm/lib/xterm.js"></script>
</head>
<body>
  <div id="terminal" style="width: 100%; height: 400px;"></div>

  <script>
    const term = new Terminal({
      cursorBlink: true,
      cols: 80,
      rows: 24,
    });
    term.open(document.getElementById('terminal'));

    // Connect to backend WebSocket server
    const socket = new WebSocket('ws://localhost:8080');

    socket.onopen = () => {
      term.write('Welcome to jailed terminal. Only "ls" command allowed.\r\n$ ');
    };

    socket.onmessage = (event) => {
      term.write(event.data);
      term.write('\r\n$ ');
    };

    let command = '';

    term.onKey(e => {
      const char = e.key;

      if (char === '\r') { // Enter key
        // Send command to backend
        socket.send(command);
        command = '';
      } else if (char === '\u007F') { // Backspace
        if (command.length > 0) {
          command = command.slice(0, -1);
          term.write('\b \b');
        }
      } else {
        command += char;
        term.write(char);
      }
    });
  </script>
</body>
</html>
Step 2: Backend (Node.js WebSocket server)
javascript
const WebSocket = require('ws');
const { exec } = require('child_process');

const wss = new WebSocket.Server({ port: 8080 });

wss.on('connection', ws => {
  ws.on('message', message => {
    const cmd = message.toString().trim();

    // Only allow "ls" command, ignore anything else
    if (cmd === 'ls') {
      exec('ls /tmp', (error, stdout, stderr) => {
        if (error) {
          ws.send(`Error: ${stderr || error.message}`);
        } else {
          ws.send(stdout || '(empty)');
        }
      });
    } else {
      ws.send('Command not allowed. Only "ls" is permitted.');
    }
  });
});

console.log('WebSocket server running on ws://localhost:8080');
Explanation
The frontend uses xterm.js to provide a terminal UI.

User input is captured and sent via WebSocket to the backend.

The backend only executes ls /tmp if the user types ls. Any other command is rejected.

This effectively "jails" the terminal to only allow listing files in /tmp.





  #############################################################



  To create a web terminal using GoTTY (gotti.js) that is jailed and only allows listing files in the /tmp directory with the ls command, you can run GoTTY with a restricted shell or command that only executes ls /tmp. GoTTY itself shares a terminal session over the web but does not provide built-in command filtering or sandboxing, so the restriction must be enforced by the command you run.

Here is a simple example of how to do this:

bash
gotty --permit-write --title-format "Jailed ls /tmp" ls /tmp
Explanation:

gotty starts the web terminal server.

ls /tmp is the only command allowed to run. The user cannot run anything else because the terminal is just running this command.

The user will see the output of ls /tmp but cannot execute other commands.

If you want a more interactive experience where the user can type commands but only ls in /tmp is allowed, you would need to create a restricted shell script that only accepts ls commands targeting /tmp and rejects everything else, then run GoTTY with that script.

Example of a simple restricted shell script (restricted-shell.sh):

bash
#!/bin/bash
while true; do
  echo -n "$ "
  read cmd args
  if [[ "$cmd" == "ls" && "$args" == "/tmp" ]]; then
    ls /tmp
  else
    echo "Command not allowed"
  fi
done
Make it executable:

bash
chmod +x restricted-shell.sh
Then run GoTTY with this script:

bash
gotty ./restricted-shell.sh
This will open a web terminal where the user can only run ls /tmp and any other command will be rejected.

Note:

GoTTY itself does not sandbox or jail commands; it simply shares the terminal session. The security and command restrictions must be handled by the command or script you run under GoTTY.

For more secure sandboxing of JavaScript code, the jailed library can be used, but it is designed for sandboxing JavaScript code execution, not shell commands.

Combining GoTTY with a restricted shell script is the practical approach to limit commands in a web terminal.

In summary, use GoTTY to serve a restricted shell script that only allows ls /tmp, as GoTTY itself does not provide command filtering or jail capabilities. This approach effectively creates a jailed web terminal limited to the ls /tmp command.

Related
How do I configure GoTTY to only allow the ls command in the /tmp directory
Can I customize the terminal type when using GoTTY
What are the security implications of using GoTTY for a jailed terminal
How do I set up a jailed terminal with GoTTY on a remote server
Are there any limitations to using GoTTY for a web terminal



