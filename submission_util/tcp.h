#ifndef WEBSOCKET_TCP_H
#define WEBSOCKET_TCP_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <map>
#include <string>
#include <sstream>

namespace network {
class HttpRequest {
	public:
		HttpRequest() = default;

		HttpRequest& SetMethod(std::string method)
		{
			method_ = std::move(method);
			return *this;
		}

		HttpRequest& SetPath(std::string path)
		{
			path_ = std::move(path);
			return *this;
		}

		HttpRequest& SetVersion(std::string version)
		{
			version_ = std::move(version);
			return *this;
		}

		HttpRequest& SetHeader(std::string key, std::string value)
		{
			headers_[std::move(key)] = std::move(value);
			return *this;
		}

		HttpRequest& SetBody(std::string body)
		{
			body_ = std::move(body);
			return *this;
		}

		std::string ToString() const
		{
        		std::stringstream ss;
        		ss << method_ << " " << path_ << " HTTP/1.1\r\n";

        		for (const auto& [key, value] : headers_) {
            			ss << key << ": " << value << "\r\n";
        		}

        		if (!body_.empty()) {
            			ss << "Content-Length: " << body_.size() << "\r\n";
        		}
        		ss << "\r\n";
        		ss << body_;
        		return ss.str();
    		}
	private:
		std::string method_;
		std::string path_;
		std::string version_;
		std::map<std::string, std::string> headers_;
		std::string body_;
};

struct HttpResponse {
	int status_code;
	std::string status_message;
	std::map<std::string, std::string> headers;
	std::string body;

	std::string GetHeader(const std::string& key)
	{
		return headers.count(key) ? headers[key] : "";
	}
};

class TcpClient {
	public:
		// Initializes a TCP connection by resolving the hostname to an IPv4 address
		// and creating a socket file descriptor.
		TcpClient(std::string_view hostname, std::string port) : hostname_(hostname), port_(port)
		{
			bool connected = false;
			ResolveHostname();

			for (struct addrinfo* p = addrinfo_; p != NULL; p = p->ai_next)
			{
				sock_fd_ = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
				if (sock_fd_ == -1) continue;
				
				// Success
				if (connect(sock_fd_, p->ai_addr, p->ai_addrlen) == 0)
				{
					connected = true;
					break;
				}
				close(sock_fd_);
				sock_fd_ = -1;
			}
			if (addrinfo_ != NULL)
			{
				freeaddrinfo(addrinfo_);
				addrinfo_ = NULL;
			}
			if (!connected) exit(-1);
		}

		// Release resources if they're in a valid state.
		virtual ~TcpClient()
		{
			if (sock_fd_ != -1)
			{
				close(sock_fd_);
			}
			if (addrinfo_ != NULL)
			{
				freeaddrinfo(addrinfo_);
			}
		}

		// Send message to hostname
		bool Send(std::string message);
		std::string Receive();
		HttpResponse ReceiveResponse();
	protected:
		std::string hostname_;
		std::string port_;
		int sock_fd_ = -1;
		struct addrinfo* addrinfo_ = NULL;

	private:
		void ResolveHostname();
		std::string ReceiveUntil(std::string delimiter);
};
}  // metwork

#endif  // WEBSOCKET_TCP_H
