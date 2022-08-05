#include <iostream>
#include <cmath>
#include "LoRaCalculator/LoRaCalculator.h"

using namespace std;

int main() {
    LoRa::Parameters p = {};
    double_t bws[] = {7.8, 10.4, 15.6, 20.8, 31.2, 41.7, 62.5, 125, 250, 500};


    //SF
    uint16_t SF_min = 0;
    uint16_t SF_max = 0;
    cout << "Enter SF_min(6-12):";
    cin >> SF_min;
    if (SF_min < 6 || SF_max > 12) {
        cerr << "Error!" << endl;
        return -1;
    }
    cout << "Enter SF_max(6-12)";
    cin >> SF_max;
    if (SF_max < 6 || SF_max > 12 || SF_max < SF_min) {
        cerr << "Error!" << endl;
        return -1;
    }
    uint8_t SF_count = SF_max - SF_min + 1;
//    cout << "SF_count: " << (short)SF_count << endl;

    //BW
    uint16_t bandwidth_min;
    uint16_t bandwidth_max;
    cout << "\nEnter bandwith" << endl;
    for (int i = 0; i < sizeof(bws) / sizeof(double_t); ++i) {
        cout << "*" << i << " is " << bws[i] << " kHz;" << endl;
    }
    cout << "Bandwidth_min: ";
    cin >> bandwidth_min;
    if (bandwidth_min < 0 || bandwidth_min > sizeof(bws) / sizeof(double_t)) {
        cerr << "Error!" << endl;
    }
    cout << "Bandwidth_max: ";
    cin >> bandwidth_max;
    if (bandwidth_max < 0 || bandwidth_max > sizeof(bws) / sizeof(double_t) || bandwidth_max < bandwidth_min) {
        cerr << "Error!" << endl;
    }
    uint8_t bandwidth_count = bandwidth_max - bandwidth_min + 1;
//    cout << "Bandwidth_count: " << (short) bandwidth_count << endl;

    //Payload
    uint16_t payload_min = 0;
    uint16_t payload_max = 0;
    cout << "\nEnter payload_min(1-255):";
    cin >> payload_min;
    if (payload_min < 1 || payload_min > 255) {
        cerr << "Error!" << endl;
        return -1;
    }
    cout << "Enter payload_max(1-255)";
    cin >> payload_max;
    if (payload_max < 1 || payload_max > 255 || payload_min > payload_max) {
        cerr << "Error!" << endl;
        return -1;
    }
    uint8_t payload_count = payload_max - payload_min + 1;
    //cout << "Payload_count: " << (short)payload_count << endl;






    return 0;
}


