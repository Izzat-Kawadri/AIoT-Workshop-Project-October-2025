# ⚙️ ESP32 AIoT   

This project connects an **ESP32** to a **Flask server** to send sensor data and receive actuator commands.  
It’s part of an **AIoT (Artificial Intelligence of Things)** system where the ESP32 acts as the hardware interface for sensing and control.

---

## 🧩 Features  
- Reads data from:  
  - **DHT22** (Temperature & Humidity)  
  - **LDR** (Light Intensity)  
  - **PIR** (Motion Sensor)  
- Sends sensor data to a Flask API endpoint (`/data`)  
- Receives control commands from another Flask endpoint (`/actuator`)  
- Controls **two relays** (e.g., fan, LED) based on server instructions  
- Uses **Wi-Fi communication** for IoT connectivity  

---

## 🔧 Hardware Setup  
| Component | ESP32 Pin | Description |
|------------|------------|-------------|
| DHT22 | GPIO12 | Temperature & humidity sensor |
| LDR | GPIO34 | Light sensor  |
| PIR | GPIO32 | Motion sensor |
| Relay 1 | GPIO2 | Controls device 1 |
| Relay 2 | GPIO27 | Controls device 2 |

---

## 🌐 Server Endpoints  
- **POST** → `/data` → Sends sensor readings to Flask server  
- **GET** → `/actuator` → Fetches relay control commands from Flask server  

Example response from `/actuator`:  
```json
{
  "relay1": "ON",
  "relay2": "OFF"
}
```

## 📡 Wi-Fi & Server Configuration

Edit these lines in the code before uploading to your ESP32:
```
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

String serverDataURL = "http://<YOUR_SERVER_IP>:5000/data";
String serverActuatorURL = "http://<YOUR_SERVER_IP>:5000/actuator";
```

## ▶️ How to Run

1. Connect your sensors and relays as described above.

2. Update Wi-Fi and server IP in the code.

3. Upload the code to your ESP32 board using Arduino IDE.

4. Run your Flask server.

4. Open the Serial Monitor at 115200 baud to view data and relay states.

## 🧠 Project Logic Flow

1. ESP32 collects sensor data.

2. Sends data to Flask via POST request.

3. Flask processes data and sends actuator decisions via GET.

4. ESP32 updates relay states accordingly.
