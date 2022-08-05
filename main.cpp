#include <iostream>
#include <cmath>
#include "LoRaCalculator/LoRaCalculator.h"

using namespace std;

int main() {
    LoRa::Parameters p = {};
    int payload = 255;
    int preamble = 7;
    int preamble_max = 13;
    int sf = 6;
    int sf_max = 10;
    int crc = 0;
    int crc_max = 2;
    int cr = 1;
    int cr_max = 5;
    int ih = 0;
    int ih_max = 1;
    int de = 0;
    int de_max = 1;
    double_t bws[] = {7.8, 10.4, 15.6, 20.8, 31.2, 41.7, 62.5, 125, 250, 500};
    int bw_i = 7;
    int bw_i_max = 10;

    const int rows = 1 * 6 * 4 * 2 * 4 * 1 * 1 * 3;
    const int columns = 16;

    double matrix[rows][columns];

    int counter = 0;

    LoRaCalculator calc;

    for (int i = preamble; i < preamble_max; i++) {
        for (int j = sf; j < sf_max; j++) {
            for (int k = bw_i; k < bw_i_max; ++k) {
                for (int l = crc; l < crc_max; ++l) {
                    for (int m = cr; m < cr_max; ++m) {

                        p.implicitHeader = 1;
                        if (j == 6)
                            p.implicitHeader = 1;
                        p.Npayload = 255;
                        p.Npreamble = i;
                        p.spreadFactor = j;
                        p.bandwidth = bws[k];
                        p.CRC = l;
                        p.codingRate = m;
                        p.dataLowRateOptimization = 0;
                        calc.setParameters(p);
                        calc.calculate();
                        matrix[counter++][0] = p.Npreamble;
                    }
                }
            }
        }
    }


    for (int i = 0; i < 2304; ++i) {
        cout << matrix[i][0] << endl;
    }


    cout << counter << endl;
    cout << rows << endl;
    return 0;
}


