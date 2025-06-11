#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <memory>

using namespace std;

// Struct for a single data point
struct CNCDataPoint {
    float temperature;
    float dimensionVariation;
    int operationMinutes;
    int selfTestCode;
};


class TemperatureChecker {
public:
    void check(float temp) const {
        if (temp > 35.0) {
            cout << "ALERT: High temperature detected (Environment)" << endl;
        }
    }
};

class DimensionVariationChecker {
public:
    void check(float variation) const {
        if (variation > 0.05) {
            cout << "ALERT: Part-dimension variation too high (Machine)" << endl;
        }
    }
};

class OperationTimeChecker {
public:
    void check(int minutes) const {
        if (minutes > 360) {
            cout << "ALERT: Continuous operation exceeded 6 hours (Machine)" << endl;
        }
    }
};

class SelfTestCodeChecker {
public:
    void check(int code) const {
        switch (code) {
            case 0xFF:
                cout << "Self-test OK" << endl;
                break;
            case 0x00:
                cout << "ALERT: No data from machine (Environment)" << endl;
                break;
            case 0x01:
                cout << "ALERT: Controller board failure (Machine)" << endl;
                break;
            case 0x02:
                cout << "ALERT: Configuration data corrupted (Machine)" << endl;
                break;
            default:
                cout << "ALERT: Unknown self-test code (Machine)" << endl;
        }
    }
};

// --- CNC Machine Monitor aggregates all checkers ---

class CNCMachineMonitor {
public:
    CNCMachineMonitor() 
        : tempChecker(make_shared<TemperatureChecker>()),
          dimChecker(make_shared<DimensionVariationChecker>()),
          timeChecker(make_shared<OperationTimeChecker>()),
          testChecker(make_shared<SelfTestCodeChecker>()) {}

    void evaluate(const CNCDataPoint& data) const {
        tempChecker->check(data.temperature);
        dimChecker->check(data.dimensionVariation);
        timeChecker->check(data.operationMinutes);
        testChecker->check(data.selfTestCode);
    }

private:
    shared_ptr<TemperatureChecker> tempChecker;
    shared_ptr<DimensionVariationChecker> dimChecker;
    shared_ptr<OperationTimeChecker> timeChecker;
    shared_ptr<SelfTestCodeChecker> testChecker;
};

// --- CNC Data Reader ---

class CNCDataReader {
public:
    explicit CNCDataReader(const string& filename) : infile(filename) {
        if (!infile) {
            throw runtime_error("Error opening input file: " + filename);
        }
    }

    bool readNext(CNCDataPoint& point) {
        int selfTestHex;
        if (infile >> point.temperature >> point.dimensionVariation >> point.operationMinutes >> hex >> selfTestHex) {
            point.selfTestCode = selfTestHex;
            infile >> dec; // Reset back to decimal
            return true;
        }
        return false;
    }

private:
    ifstream infile;
};

// --- Main function ---

int main() {
    try {
        CNCDataReader reader("cnc_data.txt");
        CNCMachineMonitor monitor;
        CNCDataPoint data;

        while (reader.readNext(data)) {
            monitor.evaluate(data);
            cout << "---" << endl;
        }
    } catch (const exception& e) {
        cerr << e.what() << endl;
        return 1;
    }

    return 0;
}
