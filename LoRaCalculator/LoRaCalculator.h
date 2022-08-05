//
// Created by root on 05.08.22.
//

#ifndef CALCULATELORA_LORACALCULATOR_H
#define CALCULATELORA_LORACALCULATOR_H


#include <cstdint>
#include <cmath>
#include <ostream>

#define LORA_PAYLOAD_MIN 1
#define LORA_PAYLOAD_MAX 255

#define LORA_PREAMBLE_MIN 6
#define LORA_PREAMBLE_MAX 65535

#define LORA_SPREAD_FACTOR_6 6
#define LORA_SPREAD_FACTOR_7 7
#define LORA_SPREAD_FACTOR_8 8
#define LORA_SPREAD_FACTOR_9 9
#define LORA_SPREAD_FACTOR_10 10
#define LORA_SPREAD_FACTOR_11 11
#define LORA_SPREAD_FACTOR_12 12

#define LORA_CRC_ENABLE 1
#define LORA_CRC_DISABLE 0

#define LORA_IMPLICIT_HEADER_ENABLE 1
#define LORA_IMPLICIT_HEADER_DISABLE 0

#define LORA_DATA_LOW_RATE_OPTIMIZATION_ENABLE 1
#define LORA_DATA_LOW_RATE_OPTIMIZATION_DISABLE 0

#define LORA_CODING_RATE_4_5 1
#define LORA_CODING_RATE_4_6 2
#define LORA_CODING_RATE_4_7 3
#define LORA_CODING_RATE_4_8 4

#define LORA_BANDWIDTH_7_8_KHZ  7.8
#define LORA_BANDWIDTH_10_4_KHZ 10.4
#define LORA_BANDWIDTH_15_6_KHZ 15.6
#define LORA_BANDWIDTH_20_8_KHZ 20.8
#define LORA_BANDWIDTH_31_2_KHZ 31.2
#define LORA_BANDWIDTH_41_7_KHZ 41.7
#define LORA_BANDWIDTH_62_5_KHZ 62.5
#define LORA_BANDWIDTH_125_KHZ 125
#define LORA_BANDWIDTH_250_KHZ 250
#define LORA_BANDWIDTH_500_KHZ 500


/*
 * SX1276 module
 * TODO: Добавить расчёт SNR и RSSI
 * TODO: Добавить референсы в doxygen
 * TODO: Всё задокументировать в doxygen
 */

namespace LoRa {
/**
    * \struct LoRa::Parameters
    * \brief input parameters of LoRa
    */
    struct Parameters {
        uint8_t Npayload; /**< Number of bytes in payload; 1-255*/
        uint16_t Npreamble; /**< Number of bytes in preamble; 6-65535*/
        uint8_t spreadFactor; /**< Spread Factor; 6-12*/
        uint8_t CRC; /**< Cyclic redundancy check, if enable 1, else 0; 0-1*/
        uint8_t codingRate; /**< Coding Rate, where 1 corresponding to 4/5, 4 to 4/8; 1-4*/
        uint8_t implicitHeader; /**< Implicit Header mode, if implicit 1, else 0; 0-1*/
        uint8_t dataLowRateOptimization; /**< Low Data Rate Optimization uses in SF=11-12, if enable 1, else 0; 0-1*/
        double_t bandwidth; /**< Bandwidth in kHz; 7.8,10.4,15.6,20.8,31.2,41.7,62.5,125,250,500; */
    };

/*!
* \struct LoRa::Results
* \brief Calculation results of LoRa
*/
    struct Results {
        double_t symbolTime; /**< How much time dose it take to transmit a symbol. ms*/
        double_t symbolRate; /**< How much symbol can be transmited in one second. sym/sec.*/
        double_t preambleSymbols; /**< How much symbols does the preamble take place. symbols*/
        double_t preambleTime; /**< How much air-time does the preamble take. ms*/
        int64_t payloadSymbols; /**< How much symbols does the ramain part in packet take place. symbols*/
        double_t payloadTime; /**< How much air-time does the payload take. ms*/
        double_t timeOnAir; /**< How much air-time does the whole packet take. ms*/
        double_t speedLoRa; /**< LoRa speed. bits/second*/
    };
}
/*! \brief LoRa calculator.
 *         For SX1776 module.
 *
 *  Detailed description starts here.
 */
class LoRaCalculator {
private:
    LoRa::Parameters parameters{};
    LoRa::Results results{};

public:
    explicit LoRaCalculator(const LoRa::Parameters &parameters);

    LoRaCalculator();

    friend std::ostream &operator<<(std::ostream &os, const LoRaCalculator &calculator);

    const LoRa::Parameters &getParameters() const;

    const LoRa::Results &getResults() const;

    void setParameters(const LoRa::Parameters &parameters);

    void calculate();

public:
    static int64_t calc_Npayload(uint8_t payload, uint8_t SpeadFact, uint8_t CRC, uint8_t ImpHead, uint8_t DataLowRate,
                                 uint8_t CodingRate, int8_t *err);

    static double_t time_symbol(uint8_t SpeadFact, double_t BandWidth);

    static double_t time_preamble(uint16_t Npreamble, double_t TimeSymbol);

    static double_t time_payload(int64_t NPayload, double_t TimeSymbol);

    static double_t time_packet(uint8_t payload, uint8_t SpeadFact, uint8_t CRC, uint8_t ImpHead, uint8_t DataLowRate,
                                uint8_t CodingRate, uint16_t Npreamble, double_t BandWidth, int8_t *err);

    static double_t speed_LoRa(uint8_t payload, double_t timePacket);

};


#endif //CALCULATELORA_LORACALCULATOR_H
