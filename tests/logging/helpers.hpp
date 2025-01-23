#include <sstream>
#include <iostream>

// Helper class to redirect std::cout to a stringstream
class CoutRedirector {
public:
    CoutRedirector() {
        old_buf_ = std::cout.rdbuf();
        std::cout.rdbuf(ss_.rdbuf());
    }

    ~CoutRedirector() {
        std::cout.rdbuf(old_buf_);
    }

    std::string str() const { return ss_.str(); }

private:
    std::streambuf* old_buf_;
    std::stringstream ss_;
};

bool check_output(std::string output, std::string str, bool condition) {
    return condition == (output.find(str) != std::string::npos);
}