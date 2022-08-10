#include <iostream>
#include <cmath>
#include "LoRaCalculator/LoRaCalculator.h"

/**
 * TODO: !!!need optimize by moving && and maybe use constexpr!!!
 */


using namespace std;

void assert_bounds(int min, int max, int value);

void assert_bounds(int less, int more);

//inline void
//fill_matrix(double_t** matrix, unsigned int rows, unsigned int columns, unsigned int counter,
//            LoRa::Parameters p, LoRa::Results res) {
//    //SF
//    matrix[counter][0] = p.spreadFactor;
//}

int main() {
    double_t bws[] = {7.8, 10.4, 15.6, 20.8, 31.2, 41.7, 62.5, 125, 250, 500};


    //SF
    uint16_t SF_min = 0;
    uint16_t SF_max = 0;
    cout << "Enter SF_min(6-12):";
    cin >> SF_min;
    assert_bounds(LORA_SPREAD_FACTOR_6, LORA_SPREAD_FACTOR_12, SF_min);
    cout << "Enter SF_max(6-12):";
    cin >> SF_max;
    assert_bounds(LORA_SPREAD_FACTOR_6, LORA_SPREAD_FACTOR_12, SF_max);
    assert_bounds(SF_min, SF_max);
    uint8_t SF_count = SF_max - SF_min + 1;
//    cout << "SF_count: " << (short) SF_count << endl;

    //BW
    uint16_t bandwidth_min;
    uint16_t bandwidth_max;
    cout << "\nEnter bandwith" << endl;
    for (int i = 0; i < sizeof(bws) / sizeof(double_t); ++i) {
        cout << "*" << i << " is " << bws[i] << " kHz;" << endl;
    }
    cout << "Bandwidth_min:";
    cin >> bandwidth_min;
    assert_bounds(0, sizeof(bws) / sizeof(double_t), bandwidth_min);
    cout << "Bandwidth_max:";
    cin >> bandwidth_max;
    assert_bounds(0, sizeof(bws) / sizeof(double_t), bandwidth_max);
    assert_bounds(bandwidth_min, bandwidth_max);
    uint8_t bandwidth_count = bandwidth_max - bandwidth_min + 1;
//    cout << "Bandwidth_count: " << (short) bandwidth_count << endl;

    //Payload
    uint16_t payload_min = 0;
    uint16_t payload_max = 0;
    cout << "\nEnter payload_min(1-255):";
    cin >> payload_min;
    assert_bounds(LORA_PAYLOAD_MIN, LORA_PAYLOAD_MAX, payload_min);
    cout << "Enter payload_max(1-255):";
    cin >> payload_max;
    assert_bounds(LORA_PAYLOAD_MIN, LORA_PAYLOAD_MAX, payload_max);
    assert_bounds(payload_min, payload_max);
    uint8_t payload_count = payload_max - payload_min + 1;
//    cout << "Payload_count: " << (short) payload_count << endl;

    //Preamble
    uint16_t preamble_min = 0;
    uint16_t preamble_max = 0;
    cout << "\nEnter preamble_min(6-65535):";
    cin >> preamble_min;
    assert_bounds(LORA_PREAMBLE_MIN, LORA_PREAMBLE_MAX, preamble_min);
    cout << "\nEnter preamble_max(6-65535):";
    cin >> preamble_max;
    assert_bounds(LORA_PREAMBLE_MIN, LORA_PREAMBLE_MAX, preamble_max);
    assert_bounds(preamble_min, preamble_max);
    uint16_t preamble_count = preamble_max - preamble_min + 1;
//    cout << "preamble_count: " << preamble_count << endl;

    //Coding Rate
    uint16_t cr_min = 0;
    uint16_t cr_max = 0;
    cout << "\nEnter CR_min(1-4):";
    cin >> cr_min;
    assert_bounds(LORA_CODING_RATE_4_5, LORA_CODING_RATE_4_8, cr_min);
    cout << "\nEnter CR_max(1-4):";
    cin >> cr_max;
    assert_bounds(LORA_CODING_RATE_4_5, LORA_CODING_RATE_4_8, cr_max);
    assert_bounds(cr_min, cr_max);
    uint8_t cr_count = cr_max - cr_min + 1;
//    cout << "cr_count: " << (short) cr_count << endl;

    //CRC
    uint16_t crc_min = 0;
    uint16_t crc_max = 1;
    cout << "\nEnter CRC_min(0-1):";
    cin >> crc_min;
    assert_bounds(LORA_CRC_DISABLE, LORA_CRC_ENABLE, crc_min);
    cout << "\nEnter CRC_max(0-1):";
    cin >> crc_max;
    assert_bounds(LORA_CRC_DISABLE, LORA_CRC_ENABLE, crc_max);
    assert_bounds(crc_min, cr_max);
    uint8_t crc_count = crc_max - crc_min + 1;
//    cout << "crc_count: " << (short) crc_count << endl;

    //Implicit Header
    uint16_t ih_min = 0;
    uint16_t ih_max = 1;
    cout << "\nEnter IH_min(0-1):";
    cin >> ih_min;
    assert_bounds(LORA_IMPLICIT_HEADER_DISABLE, LORA_IMPLICIT_HEADER_ENABLE, ih_min);
    cout << "\nEnter IH_max(0-1):";
    cin >> ih_max;
    assert_bounds(LORA_IMPLICIT_HEADER_DISABLE, LORA_IMPLICIT_HEADER_ENABLE, ih_max);
    assert_bounds(ih_min, ih_max);
    uint8_t ih_count = ih_max - ih_min + 1;
//    cout << "ih_count: " << (short) ih_count << endl;

    //Low Data Rate Optimization
    uint16_t DE_min = 0;
    uint16_t DE_max = 1;
    cout << "\nEnter DE_min(0-1):";
    cin >> DE_min;
    assert_bounds(LORA_DATA_LOW_RATE_OPTIMIZATION_DISABLE, LORA_DATA_LOW_RATE_OPTIMIZATION_ENABLE, DE_min);
    cout << "\nEnter DE_max(0-1):";
    cin >> DE_max;
    assert_bounds(LORA_DATA_LOW_RATE_OPTIMIZATION_DISABLE, LORA_DATA_LOW_RATE_OPTIMIZATION_ENABLE, DE_max);
    assert_bounds(DE_min, DE_max);
    uint8_t DE_count = DE_max - DE_min + 1;
//    cout << "DE_count: " << (short) DE_count << endl;

    const unsigned int rows =
            SF_count * bandwidth_count * payload_count * preamble_count * cr_count * crc_count * ih_count * DE_count;
    cout << rows << endl;
    const unsigned int columns = 16; // magic)
//    double_t** matrix = ;

    LoRaCalculator LRcalc;
    LoRa::Parameters p = {};
    LoRa::Results res = {};
    int counter = 0;

    for (int i = SF_min; i <= SF_max; i++) {
        for (int j = bandwidth_min; j <= bandwidth_max; ++j) {
            for (int k = payload_min; k <= payload_max; ++k) {
                for (int l = preamble_min; l <= preamble_max; ++l) {
                    for (int m = cr_min; m <= cr_max; ++m) {
                        for (int n = crc_min; n <= crc_max; ++n) {
                            for (int i1 = ih_min; i1 <= ih_max; ++i1) {
                                if (i == 6 && i1 == 0) continue;
                                for (int k1 = DE_min; k1 <= DE_max; ++k1) {
                                    p.spreadFactor = i;
                                    p.bandwidth = bws[j];
                                    p.Npayload = k;
                                    p.Npreamble = l;
                                    p.codingRate = m;
                                    p.CRC = n;
                                    p.implicitHeader = i1;
                                    p.dataLowRateOptimization = k1;
                                    //!!!need optimize by moving && and maybe use constexpr!!!
                                    LRcalc.setParameters(p);
                                    LRcalc.calculate();
                                    //cout << LRcalc << endl;
                                    res = LRcalc.getResults();

                                    //matrix[counter++][0] =
                                }
                            }
                        }
                    }
                }
            }
        }

        cout << counter << endl;
    }

    return 0;
}

void assert_bounds(int min, int max, int value) {
    if (value < min) {
        cerr << "Error! Out of bounds!\n" << value << " less than " << min << endl;
        exit(-1);
    } else if (value > max) {
        cerr << "Error! Out of bounds!\n" << value << " more than " << max << endl;
        exit(-1);
    }
}

void assert_bounds(int less, int more) {
    if (less > more) {
        cerr << "Error! min > max (" << less << " > " << more << ")" << endl;
        exit(-2);
    }
}