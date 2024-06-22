![-2147483648_-211202](https://github.com/Petrosbid/Smart-Lock-System/assets/123873128/2575f8c2-78b4-4620-8be0-d8414852fe1f)# Smart Lock System

A comprehensive smart lock system using Arduino and ESP8266, featuring a web-based interface for management and logging. This system can be controlled via the Blynk app and supports unlocking through NFC and RFID.

## Features

- **Web Interface**: User-friendly web interface to monitor the lock system and view logs.
- **Arduino and ESP8266**: Integration with Arduino for controlling the lock mechanism and ESP8266 for Wi-Fi connectivity.
- **Blynk App Integration**: Control the lock remotely via the Blynk app.
- **NFC and RFID**: Unlock the system using NFC and RFID technology.
- **Logging**: PHP-based logging of access attempts, including successful and unsuccessful unlock attempts, and activation times.
- **Notifications**: Receive notifications via the Blynk app, email, and SMS for security alerts during unauthorized access attempts.

## Project Structure

- `index.html`: The main web interface.
- `lock_arduino.ino`: Arduino code for controlling the lock mechanism and integrating NFC/RFID.
- `lock_esp8266.ino`: ESP8266 code for Wi-Fi connectivity and Blynk integration.
- `log.php`: PHP script for handling access logs.
- `Logs.css`: CSS file for styling the logs page.
- `Logs.js`: JavaScript file for dynamic functionality in the logs page.

## Getting Started

### Prerequisites

- Arduino IDE
- ESP8266 Board
- NFC/RFID Module
- Blynk App (available on iOS and Android)
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
Configure Blynk:

Download and install the Blynk app from the App Store or Google Play.
Create a new project in Blynk and add the necessary widgets for controlling the lock.
Note the Auth Token provided by Blynk and update the lock_esp8266.ino file with this token.
Set Up Notifications:

Configure the Blynk app to send notifications.
Set up email and SMS notifications for security alerts in the Blynk app or your server.
Usage
Access the Web Interface:
Open your web browser and go to http://your-server-address/index.html.

Monitor Access Logs:
View the access logs by navigating to http://your-server-address/log.php.

Control via Blynk App:
Use the Blynk app to remotely control the lock.

Unlock with NFC/RFID:
Present an authorized NFC or RFID tag to unlock the system.

Receive Notifications:
Get alerts via the Blynk app, email, and SMS for any unauthorized access attempts.

### Screenshots



### Contributing
Contributions are welcome! Please open an issue or submit a pull request.

### License
This project is licensed under the MIT License - see the LICENSE file for details.

### Acknowledgments
Inspiration
References
markdown
Copy code

### Steps to Use This README

1. Replace `yourusername` with your GitHub username in the clone command.
2. Add paths to actual screenshots in the `Screenshots` section.
3. Create a `LICENSE` file in your repository root if you haven't already, and link it correctly.
4. Adjust any other project-specific details as necessary.

This updated `README.md` includes the notification features and provides a comprehensive overview of your project.
