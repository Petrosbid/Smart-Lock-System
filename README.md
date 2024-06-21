# Smart Lock System

A comprehensive smart lock system using Arduino and ESP8266, featuring a web-based interface for management and logging.

## Features

- **Web Interface**: User-friendly web interface to control and monitor the lock system.
- **Arduino and ESP8266**: Integration with Arduino for controlling the lock mechanism and ESP8266 for Wi-Fi connectivity.
- **Logging**: PHP-based logging of access attempts.
- **Custom Styling and Scripting**: CSS and JavaScript for enhanced user experience.

## Project Structure

- `index.html`: The main web interface.
- `lock_arduino.ino`: Arduino code for controlling the lock mechanism.
- `lock_esp8266.ino`: ESP8266 code for Wi-Fi connectivity.
- `log.php`: PHP script for handling access logs.
- `Logs.css`: CSS file for styling the logs page.
- `Logs.js`: JavaScript file for dynamic functionality in the logs page.

## Getting Started

### Prerequisites

- Arduino IDE
- ESP8266 Board
- Web Server with PHP support

### Installation

1. **Clone the repository:**

   ```sh
   git clone https://github.com/yourusername/smart-lock-system.git
   cd smart-lock-system
Upload Arduino Code:

Open lock_arduino.ino in the Arduino IDE.
Connect your Arduino board and upload the code.
Upload ESP8266 Code:

Open lock_esp8266.ino in the Arduino IDE.
Connect your ESP8266 module and upload the code.
Set Up the Web Server:

Copy index.html, log.php, Logs.css, and Logs.js to your web server directory.
Usage
Access the Web Interface:
Open your web browser and go to http://your-server-address/index.html.

Monitor Access Logs:
View the access logs by navigating to http://your-server-address/log.php.

Screenshots


Contributing
Contributions are welcome! Please open an issue or submit a pull request.

License
This project is licensed under the MIT License - see the LICENSE file for details.
