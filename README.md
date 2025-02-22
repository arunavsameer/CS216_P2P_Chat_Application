# 🚀 P2P Chat System

## 🔹 Overview
The P2P Chat System is a decentralized peer-to-peer (P2P) messaging application that enables users to communicate over a network without relying on a central server.

💬 Features: Secure messaging, peer discovery, blacklisting, active peer tracking, real-time communication, and *group chat support*.

🐟 Technology Used: C++ (Winsock2 API for networking).

---

## ✨ Features

✅ Peer-to-Peer Communication – Direct chat between peers without a central server.  
✅ Server Mode – Each peer runs a lightweight server to accept messages.  
✅ Automatic Peer Discovery – Detects and maintains a list of active peers.  
✅ 🔒 Blacklist Functionality – Block unwanted IPs from messaging.  
✅ 🕒 Inactive Peer Removal – Peers inactive for more than 10 minutes are removed.  
✅ 📢 Broadcast Messaging – Send a message to all connected peers at once.  
✅ 🤝 Connection Requests – Connect with active peers dynamically.  
✅ 🖥 Beautiful Console Output – Well-formatted UI for a smooth experience.  
✅ 💬 *Group Chat Support* – Create and join chat groups for multi-peer conversations.  

---

## 💞 Installation & Setup

### 🛠 Requirements
- Windows OS (Required for Winsock2 API)
- C++ Compiler (MinGW or MSVC)

### 💻 Compiling & Running
1. Open Command Prompt or Terminal  
2. Compile the program using:
   ```sh
   g++ -o p2p_chat p2p_chat.cpp -lws2_32
   
4. Run the executable:  
   ```sh
   p2p_chat.exe
   
5. Enter your team name and port number when prompted.  
6. Start chatting! 🎉  

---

## 📝 Commands & Functionality

| 🔢 Option | 📝 Description |
|-----------|--------------|
| 1️⃣ Send message | Send a message to a specific peer. |
| 2️⃣ Query active peers | Show all currently connected peers. |
| 3️⃣ Connect to active peers | Send connection requests to known peers. |
| 4️⃣ Broadcast message | Send a message to all connected peers. |
| 5️⃣ Add IP to blacklist | Block an IP from sending messages. |
| 6️⃣ Remove IP from blacklist | Unblock an IP address. |
| 7️⃣ Show blacklist | Display all blacklisted IPs. |
| 8️⃣ *Create group* | Create a new chat group. |
| 9️⃣ *Join group* | Join an existing chat group. |
| 0️⃣ Quit | Exit the application. |

---

## 🛠 How It Works

📰 *1. Starting the Application*  
- The user runs the program and enters a team name and port number.  
- The application starts a server on the specified port to listen for incoming messages.  

💬 *2. Message Transmission*  
- Users can send messages to specific peers or broadcast messages to all connected peers.  
- Messages include a timestamp and sender details.  
- *Group messages* are sent to all members of a chat group.  

🔍 *3. Peer & Group Discovery*  
- The system automatically tracks active peers and groups.  
- Peers inactive for 600 seconds (10 minutes) are removed.  

🔒 *4. Blacklist & Group Moderation*  
- Users can add or remove IPs from the blacklist.  
- Messages from blacklisted IPs are blocked and ignored.  
- Group admins can block specific peers from a chat group.  

---

## 🎮 Example Usage

https://github.com/user-attachments/assets/cf661ae4-d44a-4013-b49d-a31ee81e4752

---

## Code Structure

🔹 *Peer Management*  
   - Tracks active peers and groups.  
   - Removes inactive peers after 10 minutes.  

🔹 *Networking (Winsock2 API)*  
   - Uses TCP sockets for P2P communication.  
   - Runs a server to accept messages.  

🔹 *Blacklist & Group Management*  
   - Users can add/remove IPs from the blacklist.  
   - Group admins can manage members.  

🔹 *🖥 Console Output*  
   - Well-structured messages for better readability.  

---

## 🚀 Future Enhancements
🔹 🖥 *GUI Interface* - Replace console-based interaction with a graphical UI.  
🔹 📂 *File Sharing* - Add support for peer-to-peer file transfers.  
🔹 🔐 *Encryption* - Secure messages with end-to-end encryption.  
🔹 🌍 *Cross-Platform Support* - Make it work on Linux & macOS.  

---

## 👥 Contributors
- Abhitulya Mishra (ee230002002)  
- Arunav Sameer (cse230001010)  
- Anmol Joshi (cse230001007)
