#include "./submission.cc"

int main()
{
	submission::helper::Submission sub("3", "2025", "2");
	std::cout << sub.Submit("Fake answer") << std::endl;
}
