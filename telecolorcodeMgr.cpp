#include <telecolorcodeMgr.h>
 
#define MAX_COLORS 25
 
typedef struct {
    int pairNumber;
    const char* majorColor;
    const char* minorColor;
} ColorPair;
 
void generateColorMap(ColorPair colorMap[], int* totalPairs) {
    const char* majorColor[] = {"White", "Red", "Black", "Yellow", "Violet"};
    const char* minorColor[] = {"Blue", "Orange", "Green", "Brown", "Slate"};
 
    int count = 0;
    for(int i = 0; i < 5; i++) {
        for(int j = 0; j < 5; j++) {
            colorMap[count].pairNumber = count;
            colorMap[count].majorColor = majorColor[i];
            colorMap[count].minorColor = minorColor[j];
            count++;
        }
    }
    *totalPairs = count;
}
 
void printColorPair(const ColorPair* pair) {
    printf("%d | %s | %s\n", pair->pairNumber, pair->majorColor, pair->minorColor);
}
 
void printColorMap(const ColorPair colorMap[], int totalPairs) {
    for(int i = 0; i < totalPairs; i++) {
        printColorPair(&colorMap[i]);
    }
}
 
int main() {
    ColorPair colorMap[MAX_COLORS];
    int totalPairs = 0;
 
    generateColorMap(colorMap, &totalPairs);
    printColorMap(colorMap, totalPairs);
 
    assert(totalPairs == 25);
    printf("All is well (maybe!)\n");
    return 0;
}