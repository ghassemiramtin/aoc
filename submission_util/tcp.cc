#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <sys/socket.h>
#include "./tcp.h"

namespace network {
void TcpClient::ResolveHostname()
{
	struct addrinfo hints;

	std::memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	if (getaddrinfo(hostname_.c_str(), port_.c_str(), &hints, &addrinfo_) == -1)
	{
		perror("getaddrinfo(hostname_, port_, &hints, &addrinfo_)");
		exit(-1);
	}
}

bool TcpClient::Send(std::string message)
{
	ssize_t total_sent = 0;
	const char*data = message.c_str();

	while (total_sent < message.size())
	{
		ssize_t sent = send(sock_fd_, data + total_sent, message.size() - total_sent, 0);
		if (sent == -1) return false;
		total_sent += sent;
	}
	return true;
}

std::string TcpClient::Receive() {
    char buffer[4096];
    ssize_t bytes_read = read(sock_fd_, buffer, sizeof(buffer) - 1);
    
    if (bytes_read < 0) {
        perror("Read error");
        return ""; 
    }
    if (bytes_read == 0) return ""; // Connection closed

    return std::string(buffer, bytes_read);
}

std::string TcpClient::ReceiveUntil(std::string delimiter) {
    std::string message;
    char buffer[1]; 
    ssize_t bytes_read;

    while (true) {
        bytes_read = recv(sock_fd_, buffer, 1, 0);
        if (bytes_read <= 0) break; 

        message += buffer[0];
        if (message.size() >= delimiter.size() &&
			message.compare(message.size() - delimiter.size(), delimiter.size(), delimiter) == 0) break;
    }
    return message;
}

HttpResponse TcpClient::ReceiveResponse() {
    HttpResponse res;
    
    std::string status_line = ReceiveUntil("\r\n");
    if (status_line.empty()) return res;

    std::stringstream ss(status_line);
    std::string version;
    ss >> version >> res.status_code;
    std::getline(ss, res.status_message);

    // 2. Parse Headers
    while (true) {
        std::string line = ReceiveUntil("\r\n");
        if (line == "\r\n" || line.empty()) break; // End of headers

        size_t colon = line.find(':');
        if (colon != std::string::npos) {
            std::string key = line.substr(0, colon);
            std::string value = line.substr(colon + 2); // Skip ": "
            // Trim the trailing \r if it exists
            if (!value.empty() && value.back() == '\r') value.pop_back();
            res.headers[key] = value;
        }
    }

    // 3. Parse Body based on Content-Length
    std::string len_str = res.GetHeader("Content-Length");
    if (!len_str.empty()) {
        size_t content_length = std::stoul(len_str);
        res.body.resize(content_length);
        
        size_t total_received = 0;
        while (total_received < content_length) {
            ssize_t received = recv(sock_fd_, &res.body[total_received], content_length - total_received, 0);
            if (received <= 0) break;
            total_received += received;
        }
    }

    return res;
}
}  // network
