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
