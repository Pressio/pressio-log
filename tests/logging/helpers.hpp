#include <sstream>
#include <fstream>
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

// Helper test to assert that the output is present if the condition is true
bool check_output(std::string output, std::string str, bool condition) {
    return condition == (output.find(str) != std::string::npos);
}

// Helper to read in a file and return the content as a string
std::string read_file(std::string fileName) {
    std::ifstream file(fileName);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + fileName);
    }
    std::ostringstream oss;
    oss << file.rdbuf();
    return oss.str();
}
