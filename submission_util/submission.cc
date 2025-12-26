#include <memory>
#include <string>

#include <fmt/core.h>

#include "../file_utils/file_parser.cc"
#include "./tcp.cc"

#ifndef SUBMIT_H
#define SUBMIT_H

namespace submission::helper {

using ::network::HttpRequest;
using ::network::HttpResponse;
using ::network::TcpClient;

class Submission {
	public:
		Submission(std::string day, std::string year, std::string level) :
			path_(fmt::format("/{}/day/{}/answer", year, day)), level_(level), aoc_file_("../.aoc_session"), client_(hostname_, "80") {
				aoc_session_ = fmt::format("session={};", aoc_file_.GetLine());
			}
		~Submission() = default;

		std::string Submit(std::string answer)
		{
			// Construct the Advent of Code request
			auto req = network::HttpRequest()
    				.SetMethod("POST")
    				.SetPath(path_)
    				.SetHeader("Host", "adventofcode.com")
    				.SetHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7")
    				.SetHeader("Accept-Language", "en-US,en;q=0.9")
    				.SetHeader("Cache-Control", "max-age=0")
    				.SetHeader("Content-Type", "application/x-www-form-urlencoded")
    				.SetHeader("Origin", "https://adventofcode.com")
    				.SetHeader("Referer", "https://adventofcode.com/2025/day/3")
    				.SetHeader("User-Agent", "Mozilla/5.0 (X11; CrOS x86_64 14541.0.0) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/142.0.0.0 Safari/537.36")
    				.SetHeader("Cookie", aoc_session_)
    				.SetHeader("Sec-Fetch-Dest", "document")
    				.SetHeader("Sec-Fetch-Mode", "navigate")
    				.SetHeader("Sec-Fetch-Site", "same-origin")
    				.SetBody("level=1&answer=17043")
				.ToString();
			client_.Send(req);

			return client_.ReceiveResponse().ToString();
		}
	private:
		std::string hostname_ = "adventofcode.com";
		std::string path_;
		std::string level_;
		std::string aoc_session_;
		FileParser aoc_file_;
		TcpClient client_;
};
}  //  submission::helper

#endif
