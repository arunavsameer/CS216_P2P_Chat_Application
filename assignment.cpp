#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <map>
#include <chrono>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <atomic>
#include <ctime>
#include <unordered_set>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

struct Peer{
    string ip;
    int port;
    chrono::steady_clock::time_point last_seen;
};

mutex peer_mutex;
map<string, Peer> peers;
string team_name;
const vector<pair<string, int>> mandatory_peers = {{"10.206.4.122", 1255}, {"10.206.5.228", 6555}};
atomic<bool> running(true);
SOCKET global_server_socket = INVALID_SOCKET;
unordered_set<string> blacklist;

void ping_peers(){
    while (running){
        this_thread::sleep_for(chrono::seconds(10));
        lock_guard<mutex> lock(peer_mutex);
        auto now = chrono::steady_clock::now();
        for (auto it = peers.begin(); it != peers.end();){
            if (chrono::duration_cast<chrono::seconds>(now - it->second.last_seen).count() > 600){
                cout << "\n===================================================" << endl;
                cout << "  Removing inactive peer: " << it->first << endl;
                cout << "===================================================\n" << endl;
                it = peers.erase(it);
            } else {
                ++it;
            }
        }
    }
}

string get_local_ip(){
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET)
        return "Unknown";

    sockaddr_in remoteAddr;
    memset(&remoteAddr, 0, sizeof(remoteAddr));
    remoteAddr.sin_family = AF_INET;
    remoteAddr.sin_addr.s_addr = inet_addr("8.8.8.8");
    remoteAddr.sin_port = htons(53);

    if (connect(sock, (sockaddr*)&remoteAddr, sizeof(remoteAddr)) == SOCKET_ERROR){
        closesocket(sock);
        return "Unknown";
    }

    sockaddr_in localAddr;
    int addrLen = sizeof(localAddr);
    if (getsockname(sock, (sockaddr*)&localAddr, &addrLen) == SOCKET_ERROR){
        closesocket(sock);
        return "Unknown";
    }

    string localIP = inet_ntoa(localAddr.sin_addr);
    closesocket(sock);
    return localIP;
}

void handle_peer(string ip, int port){
    lock_guard<mutex> lock(peer_mutex);
    string peer_key = ip + ":" + to_string(port);
    if (peers.find(peer_key) == peers.end()){
        peers[peer_key] = {ip, port, chrono::steady_clock::now()};
        cout << "\n===================================================" << endl;
        cout << "  Peer added: " << peer_key << endl;
        cout << "===================================================\n" << endl;
    } else {
        peers[peer_key].last_seen = chrono::steady_clock::now();
    }
}

void query_peers(){
    lock_guard<mutex> lock(peer_mutex);
    cout << "\n===================================================" << endl;
    cout << "  Connected peers:" << endl;
    for (const auto& peer : peers){
        cout << "  " << peer.first << endl;
    }
    cout << "===================================================\n" << endl;
}

void remove_peer(const string &peer_key){
    lock_guard<mutex> lock(peer_mutex);
    if (peers.erase(peer_key)){
        cout << "\n===================================================" << endl;
        cout << "  Peer removed: " << peer_key << endl;
        cout << "===================================================\n" << endl;
    }
}

string get_current_time(){
    auto now = chrono::system_clock::now();
    time_t now_c = chrono::system_clock::to_time_t(now);
    char buffer[100];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&now_c));
    return string(buffer);
}

void send_message(const string& message, const string& peer_ip, int peer_port, int my_port){
    if (blacklist.find(peer_ip) != blacklist.end()){
        cout << "\n===================================================" << endl;
        cout << "  Cannot send message to blacklisted IP: " << peer_ip << endl;
        cout << "===================================================\n" << endl;
        return;
    }
    
    struct in_addr addr;
    if (inet_pton(AF_INET, peer_ip.c_str(), &addr) != 1) {
        cout << "\n===================================================" << endl;
        cout << "  Invalid IP format. Exiting..." << endl;
        cout << "===================================================\n" << endl;
        exit(1);
    }

    SOCKET sock;
    struct sockaddr_in server;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET){
        cout << "\n===================================================" << endl;
        cout << "  Socket creation failed." << endl;
        cout << "===================================================\n" << endl;
        return;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(peer_port);
    inet_pton(AF_INET, peer_ip.c_str(), &server.sin_addr);

    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0){
        cout << "\n===================================================" << endl;
        cout << "  Connection failed to " << peer_ip << ":" << peer_port << endl;
        cout << "===================================================\n" << endl;
        string peer_key = peer_ip + ":" + to_string(peer_port);
        remove_peer(peer_key);
        closesocket(sock);
        return;
    }

    string formatted_message;
    string timestamp = get_current_time();
    if (message == "DISCONNECT")
        formatted_message = "<" + peer_ip + ":" + to_string(my_port) + "> [" + timestamp + "] DISCONNECT";
    else
        formatted_message = "<" + peer_ip + ":" + to_string(my_port) + "> [" + timestamp + "] @ <" + team_name + ">\n" + message + "\n";

    send(sock, formatted_message.c_str(), formatted_message.size(), 0);
    cout << "\n===================================================" << endl;
    cout << "  Message sent to " << peer_ip << endl;
    cout << "===================================================\n" << endl;
    closesocket(sock);
}

void start_server(int my_port){
    SOCKET server_socket, client_socket;
    struct sockaddr_in server, client;
    int c;

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET){
        cout << "\n===================================================" << endl;
        cout << "  Socket creation failed." << endl;
        cout << "===================================================\n" << endl;
        return;
    }
    global_server_socket = server_socket;

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(my_port);

    if (bind(server_socket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR){
        cout << "\n===================================================" << endl;
        cout << "  Binding failed." << endl;
        cout << "===================================================\n" << endl;
        closesocket(server_socket);
        return;
    }

    listen(server_socket, 3);
    cout << "\n===================================================" << endl;
    cout << "  Server listening on port " << my_port << endl;
    cout << "===================================================\n" << endl;

    c = sizeof(struct sockaddr_in);
    while (running){
        client_socket = accept(server_socket, (struct sockaddr*)&client, &c);
        if (client_socket == INVALID_SOCKET){
            if (!running)
                break;
            continue;
        }

        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client.sin_addr, client_ip, INET_ADDRSTRLEN);

        if (blacklist.find(client_ip) != blacklist.end()){
            cout << "\n===================================================" << endl;
            cout << "  Ignored message from blacklisted IP: " << client_ip << endl;
            cout << "===================================================\n" << endl;
            closesocket(client_socket);
            continue;
        }

        char buffer[1024] = {0};
        int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        string msg;
        if (bytes_received > 0){
            buffer[bytes_received] = '\0';
            msg = buffer;
        }

        int sender_port = 0;
        if (!msg.empty() && msg.front() == '<'){
            size_t gt = msg.find('>');
            if (gt != string::npos){
                size_t colon = msg.find(':', 1);
                if (colon != string::npos && colon < gt){
                    try {
                        sender_port = stoi(msg.substr(colon + 1, gt - colon - 1));
                    } catch (exception &e){
                        sender_port = ntohs(client.sin_port);
                    }
                }
            }
        }
        if (sender_port == 0)
            sender_port = ntohs(client.sin_port);

        handle_peer(client_ip, sender_port);

        string content = msg;
        size_t pos = content.find("> ");
        if (pos != string::npos)
            content = content.substr(pos + 2);

        if (content == "DISCONNECT"){
            string peer_key = string(client_ip) + ":" + to_string(sender_port);
            remove_peer(peer_key);
            cout << "\n===================================================" << endl;
            cout << "  Peer " << peer_key << " has disconnected." << endl;
            cout << "===================================================\n" << endl;
        } else {
            if (!msg.empty()){
                string timestamp = get_current_time();
                cout << "\n===================================================" << endl;
                cout << "  Message received at " << timestamp << " from " << client_ip << ":" << sender_port << " - " << endl << msg << endl;
                cout << "===================================================\n" << endl;
            } else {
                cout << "\n===================================================" << endl;
                cout << "  No message received from " << client_ip << endl;
                cout << "===================================================\n" << endl;
            }
        }
        closesocket(client_socket);
    }

    closesocket(server_socket);
}

void add_to_blacklist(const string& ip){
    blacklist.insert(ip);
    cout << "\n===================================================" << endl;
    cout << "  " << ip << " has been BLACKLISTED." << endl;
    cout << "===================================================\n" << endl;
}

void remove_from_blacklist(const string& ip){
    if (blacklist.erase(ip)){
        cout << "\n===================================================" << endl;
        cout << "  Removed " << ip << " from blacklist" << endl;
        cout << "===================================================\n" << endl;
    } else {
        cout << "\n===================================================" << endl;
        cout << "  " << ip << " not found in blacklist" << endl;
        cout << "===================================================\n" << endl;
    }
}

void show_blacklist(){
    if (blacklist.empty()){
        cout << "\n===================================================" << endl;
        cout << "  Blacklist is empty" << endl;
        cout << "===================================================\n" << endl;
    } else {
        cout << "\n===================================================" << endl;
        cout << "  Blacklisted IPs:" << endl;
        for (const auto& ip : blacklist){
            cout << "  " << ip << endl;
        }
        cout << "===================================================\n" << endl;
    }
}

int main(){
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0){
        cout << "\n===================================================" << endl;
        cout << "  Failed to initialize Winsock." << endl;
        cout << "===================================================\n" << endl;
        return 1;
    }

    cout << "Enter your team name: ";
    getline(cin, team_name);

    int my_port;
    cout << "Enter your port number: ";
    cin >> my_port;
    if(my_port < 1024 or my_port > 65535){
        cout << "\n===================================================" << endl;
        cout << "  Invalid port number. Range is 1024-65535" << endl;
        cout << "===================================================\n" << endl;
        return 1;
    }

    cout << endl;
    string local_ip = get_local_ip();
    cout << "\n===================================================" << endl;
    cout << "  Your local IP address: " << local_ip << endl;
    cout << "===================================================\n" << endl;

    thread server_thread(start_server, my_port);
    thread ping_thread(ping_peers);

    while (true){
        this_thread::sleep_for(chrono::seconds(1));
        cout << "\n***** Menu *****" << endl;
        cout << "1. Send message" << endl;
        cout << "2. Query active peers" << endl;
        cout << "3. Connect to active peers" << endl;
        cout << "4. Broadcast message" << endl;
        cout << "5. Add IP to blacklist" << endl;
        cout << "6. Remove IP from blacklist" << endl;
        cout << "7. Show blacklist" << endl;
        cout << "0. Quit" << endl;
        cout << "Enter choice: ";

        int choice;
        cin >> choice;

        if (choice == 1){
            string target_ip, message;
            int target_port;
            cout << "Enter recipient's IP: ";
            cin >> target_ip;
            cout << "Enter recipient's port: ";
            cin >> target_port;
            cout << "Enter your message: ";
            cin.ignore();
            getline(cin, message);
            send_message(message, target_ip, target_port, my_port);
        } else if (choice == 2){
            query_peers();
        } else if (choice == 3){
            {
                lock_guard<mutex> lock(peer_mutex);
                for (const auto& peer : peers){
                    thread(send_message, "CONNECTION_REQUEST", peer.second.ip, peer.second.port, my_port).detach();
                }
            }
            cout << "\n===================================================" << endl;
            cout << "  Connection requests sent to active peers." << endl;
            cout << "===================================================\n" << endl;
        } else if (choice == 4){
            cout << "Enter broadcast message: ";
            cin.ignore();
            string bmsg;
            getline(cin, bmsg);
            {
                lock_guard<mutex> lock(peer_mutex);
                for (const auto& peer : peers){
                    thread(send_message, bmsg, peer.second.ip, peer.second.port, my_port).detach();
                }
            }
            cout << "\n===================================================" << endl;
            cout << "  Broadcast message sent to all connected peers." << endl;
            cout << "===================================================\n" << endl;
        } else if (choice == 5){
            string ip;
            cout << "Enter IP to blacklist: ";
            cin >> ip;
            add_to_blacklist(ip);
        } else if (choice == 6){
            string ip;
            cout << "Enter IP to remove from blacklist: ";
            cin >> ip;
            remove_from_blacklist(ip);
        } else if (choice == 7){
            show_blacklist();
        } else if (choice == 0){
            cout << "\n===================================================" << endl;
            cout << "  Exiting..." << endl;
            cout << "===================================================\n" << endl;
            lock_guard<mutex> lock(peer_mutex);
            for (const auto& peer : peers){
                thread(send_message, "DISCONNECT", peer.second.ip, peer.second.port, my_port).detach();
            }
            running = false;
            if (global_server_socket != INVALID_SOCKET)
                closesocket(global_server_socket);
            break;
        } else {
            cout << "Invalid choice, try again." << endl;
        }
    }

    server_thread.join();
    ping_thread.join();
    WSACleanup();
    return 0;
}
