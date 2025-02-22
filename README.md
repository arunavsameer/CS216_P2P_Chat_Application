Here's a GitHub-friendly **README.md** file. You can copy and paste it directly into your repository.  

```md
# 🚀 P2P Chat System

## 🔹 Overview
The **P2P Chat System** is a decentralized **peer-to-peer (P2P) messaging application** that enables users to communicate over a network without relying on a central server.

💬 **Features:** Secure messaging, peer discovery, blacklisting, active peer tracking, and real-time communication.

📡 **Technology Used:** C++ (Winsock2 API for networking).

---

## ✨ Features

✅ **Peer-to-Peer Communication** – Direct chat between peers without a central server.  
✅ **Server Mode** – Each peer runs a lightweight server to accept messages.  
✅ **Automatic Peer Discovery** – Detects and maintains a list of active peers.  
✅ **🔒 Blacklist Functionality** – Block unwanted IPs from messaging.  
✅ **🕒 Inactive Peer Removal** – Peers inactive for more than **10 minutes** are removed.  
✅ **📢 Broadcast Messaging** – Send a message to all connected peers at once.  
✅ **🤝 Connection Requests** – Connect with active peers dynamically.  
✅ **🖥️ Beautiful Console Output** – Well-formatted UI for a smooth experience.  

---

## 📥 Installation & Setup

### 🛠 Requirements
- **Windows OS** (Required for Winsock2 API)
- **C++ Compiler** (MinGW or MSVC)

### 💻 Compiling & Running
1. **Open Command Prompt or Terminal**  
2. **Compile the program** using:  
   ```sh
   g++ -o p2p_chat p2p_chat.cpp -lws2_32
   ```
3. **Run the executable**:  
   ```sh
   p2p_chat.exe
   ```
4. **Enter your team name and port number** when prompted.  
5. **Start chatting! 🎉**  

---

## 📝 Commands & Functionality

| 🔢 Option | 📝 Description |
|-----------|--------------|
| `1️⃣ Send message` | Send a message to a specific peer. |
| `2️⃣ Query active peers` | Show all currently connected peers. |
| `3️⃣ Connect to active peers` | Send connection requests to known peers. |
| `4️⃣ Broadcast message` | Send a message to all connected peers. |
| `5️⃣ Add IP to blacklist` | Block an IP from sending messages. |
| `6️⃣ Remove IP from blacklist` | Unblock an IP address. |
| `7️⃣ Show blacklist` | Display all blacklisted IPs. |
| `0️⃣ Quit` | Exit the application. |

---

## 🛠 How It Works

📡 **1. Starting the Application**  
- The user runs the program and **enters a team name and port number**.  
- The application starts a **server on the specified port** to listen for incoming messages.  

💬 **2. Message Transmission**  
- Users can send messages to **specific peers** or **broadcast** messages to all connected peers.  
- Messages include a **timestamp** and sender details.  

🔍 **3. Peer Discovery & Management**  
- The system **automatically tracks active peers**.  
- Peers inactive for **600 seconds (10 minutes)** are **removed**.  

🔒 **4. Blacklist Functionality**  
- Users can **add** or **remove** IPs from the blacklist.  
- Messages from blacklisted IPs are **blocked and ignored**.  

---

## 🎮 Example Usage

### 🔥 Starting the Chat System
```
Enter your team name: Team Alpha  
Enter your port number: 5050  
===================================================
  🚀 Your local IP address: 192.168.1.100  
===================================================
```

### ✉ Sending a Message
```
***** 📢 Chat Menu *****  
1️⃣ Send message  
2️⃣ Query active peers  
3️⃣ Connect to active peers  
4️⃣ Broadcast message  
5️⃣ Add IP to blacklist  
6️⃣ Remove IP from blacklist  
7️⃣ Show blacklist  
0️⃣ Quit  
Enter choice: 1  

Enter recipient's IP: 192.168.1.101  
Enter recipient's port: 5051  
Enter your message: Hello from Team Alpha!  
===================================================
  ✅ Message sent to 192.168.1.101  
===================================================
```

### 📥 Receiving a Message
```
===================================================
  📩 Message received at 2024-02-22 14:30:15  
  📍 From: 192.168.1.101:5051  
  💬 "Hello from 192.168.1.101!"  
===================================================
```

---

## 🏗 Code Structure

📌 **Peer Management**  
   - Keeps track of active peers.  
   - Removes inactive peers after **10 minutes**.  

📌 **Networking (Winsock2 API)**  
   - Uses **TCP sockets** for P2P communication.  
   - Runs a **server to accept messages**.  

📌 **Blacklist Management**  
   - Users can **add/remove** IPs from the blacklist.  
   - **Blocked IPs cannot send messages**.  

📌 **🖥️ Console Output**  
   - Well-structured messages for **better readability**.  

---

## 🚀 Future Enhancements
🔹 **🖥️ GUI Interface** - Replace console-based interaction with a graphical UI.  
🔹 **📂 File Sharing** - Add support for **peer-to-peer file transfers**.  
🔹 **🔐 Encryption** - Secure messages with **end-to-end encryption**.  
🔹 **🌍 Cross-Platform Support** - Make it work on **Linux & macOS**.  

---

## 📜 License
This project is **open-source** and free to use. 🎉  

---

Hope this helps! 🚀 Copy and paste this into your **README.md** file on GitHub. 🎯  
```

Let me know if you need modifications! 🚀🔥
