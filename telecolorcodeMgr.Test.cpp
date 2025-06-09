/ test_color_map.cpp
#include <gtest/gtest.h>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
 
extern "C" {
    #include "color_map.h" // Header containing declarations for generateColorMap, printColorMap, ColorPair
}
 
#define MAX_COLORS 25
#define MAX_OUTPUT_SIZE 2048
 
class ColorMapTest : public ::testing::Test {
protected:
    ColorPair colorMap[MAX_COLORS];
    int totalPairs = 0;
 
    void SetUp() override {
        generateColorMap(colorMap, &totalPairs);
    }
 
    std::string captureOutputOfPrintColorMap() {
        // Redirect stdout
        int stdout_fd = dup(STDOUT_FILENO); // backup stdout
        int pipefd[2];
        pipe(pipefd);
        dup2(pipefd[1], STDOUT_FILENO);  // replace stdout with write end of pipe
        close(pipefd[1]);
 
        // Call the function that prints
        printColorMap(colorMap, totalPairs);
 
        // Flush and restore stdout
        fflush(stdout);
        dup2(stdout_fd, STDOUT_FILENO);
        close(stdout_fd);
 
        // Read output from pipe
        char buffer[MAX_OUTPUT_SIZE] = {0};
        read(pipefd[0], buffer, MAX_OUTPUT_SIZE);
        close(pipefd[0]);
 
        return std::string(buffer);
    }
};
 
TEST_F(ColorMapTest, GeneratesCorrectNumberOfPairs) {
    ASSERT_EQ(totalPairs, 25);
}
 
TEST_F(ColorMapTest, OutputHasExpectedFirstAndLastLines) {
    std::string output = captureOutputOfPrintColorMap();
 
    // Split by line
    std::istringstream stream(output);
    std::string line;
    std::vector<std::string> lines;
    while (std::getline(stream, line)) {
        if (!line.empty()) {
            lines.push_back(line);
        }
    }
 
    ASSERT_EQ(lines.size(), 25);
    EXPECT_EQ(lines[0], "0 | White | Blue");
    EXPECT_EQ(lines[24], "24 | Violet | Slate");
}