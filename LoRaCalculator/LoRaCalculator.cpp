//
// Created by root on 05.08.22.
//

#include "LoRaCalculator.h"
#include <stdexcept>
#include <iostream>

using namespace LoRa;

int64_t
LoRaCalculator::calc_Npayload(uint8_t payload, uint8_t SpeadFact, uint8_t CRC, uint8_t ImpHead, uint8_t DataLowRate,
                              uint8_t CodingRate, int8_t *err) {
    if ((SpeadFact < 6 || SpeadFact > 12) || (CRC < 0 || CRC > 1) || (ImpHead < 0 || ImpHead > 1) ||
        (DataLowRate < 0 || DataLowRate > 1) || (CodingRate < 1 || CodingRate > 4)) {
        *err = -1;
        return -1;
    }
    if (SpeadFact > 10 && DataLowRate != 0) {
        *err - 2;
        return -2;
    }
    double result = 0;
    result = 8 * payload - 4 * SpeadFact + 28 + 16 * CRC - 20 * ImpHead;
    result /= 4 * (SpeadFact - 2 * DataLowRate);
    result = ceil(result);
    result *= CodingRate + 4;
    result = fmax(result, 0);
    result += 8;
    return (int64_t) result;
}

double_t LoRaCalculator::time_symbol(uint8_t SpeadFact, double_t BandWidth) {
    if (SpeadFact < 6 || SpeadFact > 12 || BandWidth < 0 || BandWidth > 500)
        return -1;
    return pow(2, SpeadFact) / BandWidth;
}

double_t LoRaCalculator::time_preamble(uint16_t Npreamble, double_t TimeSymbol) {
    if (Npreamble < 6)
        return -1;
    return (Npreamble + 4.25) * TimeSymbol;
}

double_t LoRaCalculator::time_payload(int64_t NPayload, double_t TimeSymbol) {
    return (double_t) NPayload * TimeSymbol;
}

double_t
LoRaCalculator::time_packet(uint8_t payload, uint8_t SpeadFact, uint8_t CRC, uint8_t ImpHead, uint8_t DataLowRate,
                            uint8_t CodingRate, uint16_t Npreamble, double_t BandWidth, int8_t *err) {
    double_t TimeSymbol = time_symbol(SpeadFact, BandWidth); //optimize
    return time_payload(calc_Npayload(payload, SpeadFact, CRC, ImpHead, DataLowRate, CodingRate, err), TimeSymbol) +
           time_preamble(Npreamble, TimeSymbol);
}

double_t LoRaCalculator::speed_LoRa(uint8_t payload, double_t timePacket) {
    return payload / timePacket * 1000 * 8;
}

/** \brief Constructor with default values;
 *  @var uint8_t Npayload
 *  Number of bytes in payload; 1-255
 *  @var uint16_t Npreamble
 *  Number of bytes in preamble; 6-65535
 *  @var uint8_t SF
 *  Spread Factor; 6-12
 *  @var uint8_t CRC
 *  Cyclic redundancy check, if enable 1, else 0; 0-1
 *  @var uint8_t Coding Rate
 *  Coding Rate, where 1 corresponding to 4/5, 4 to 4/8; 1-4
 *  @var uint8_t Implicit Header
 *  Implicit Header mode, if implicit 1, else 0; 0-1
 *  @var uint8_t Data Low Rate Optimization
 *  Low Data Rate Optimization uses in SF=11-12, if enable 1, else 0; 0-1
 *  @var double_t Bandwidth
 *  Bandwidth in kHz; 7.8,10.4,15.6,20.8,31.2,41.7,62.5,125,250,500;
 *
 */

LoRaCalculator::LoRaCalculator(const Parameters &parameters) : parameters(parameters) {
    if (parameters.Npayload < LORA_PAYLOAD_MIN)
        throw std::invalid_argument("Npayload less then 1!");
    if (parameters.Npreamble < LORA_PREAMBLE_MIN)
        throw std::invalid_argument("Npreamble less then 6!");
    if (parameters.spreadFactor < LORA_SPREAD_FACTOR_6 || parameters.spreadFactor > LORA_SPREAD_FACTOR_12)
        throw std::invalid_argument("Spread Factor invalid value!");
    if (parameters.CRC < LORA_CRC_DISABLE || parameters.CRC > LORA_CRC_ENABLE)
        throw std::invalid_argument("CRC invalid value!");
    if (parameters.codingRate < LORA_CODING_RATE_4_5 || parameters.codingRate > LORA_CODING_RATE_4_8)
        throw std::invalid_argument("Coding Rate invalid value!");
    if (parameters.implicitHeader < LORA_IMPLICIT_HEADER_DISABLE ||
        parameters.implicitHeader > LORA_IMPLICIT_HEADER_ENABLE)
        throw std::invalid_argument("Implicit Header invalid value!");
    if (parameters.dataLowRateOptimization < LORA_DATA_LOW_RATE_OPTIMIZATION_DISABLE ||
        parameters.dataLowRateOptimization > LORA_DATA_LOW_RATE_OPTIMIZATION_ENABLE)
        throw std::invalid_argument("Data Low Rate Optimization invalid value!");
    if (parameters.bandwidth != LORA_BANDWIDTH_7_8_KHZ && parameters.bandwidth != LORA_BANDWIDTH_10_4_KHZ &&
        parameters.bandwidth != LORA_BANDWIDTH_15_6_KHZ && parameters.bandwidth != LORA_BANDWIDTH_20_8_KHZ &&
        parameters.bandwidth != LORA_BANDWIDTH_31_2_KHZ && parameters.bandwidth != LORA_BANDWIDTH_41_7_KHZ &&
        parameters.bandwidth != LORA_BANDWIDTH_62_5_KHZ && parameters.bandwidth != LORA_BANDWIDTH_125_KHZ &&
        parameters.bandwidth != LORA_BANDWIDTH_250_KHZ && parameters.bandwidth != LORA_BANDWIDTH_500_KHZ) {
        throw std::invalid_argument("Bandwitch invalid value!");
    }
    //SX1276 SF=6 required IH=1
    if (parameters.spreadFactor == LORA_SPREAD_FACTOR_6 && parameters.implicitHeader != LORA_IMPLICIT_HEADER_ENABLE) {
        throw std::invalid_argument("With SF=6 implicit header is required!");
    }

}

/** \brief Constructor with default values;
 *  @var Npayload = 1
 *  @var Npreamble = 8
 *  @var SF = 7
 *  @var CRC = 1
 *  @var Coding Rate = 1
 *  @var Implicit Header = 0
 *  @var Data Low Rate Optimization = 0
 *  @var Bandwidth = 125kHz
 */
LoRaCalculator::LoRaCalculator() {
    parameters = {1, 8, LORA_SPREAD_FACTOR_7, LORA_CRC_DISABLE, LORA_CODING_RATE_4_5, LORA_IMPLICIT_HEADER_DISABLE,
                  LORA_DATA_LOW_RATE_OPTIMIZATION_DISABLE, LORA_BANDWIDTH_125_KHZ};
}

const Parameters &LoRaCalculator::getParameters() const {
    return parameters;
}

const Results &LoRaCalculator::getResults() const {
    return results;
}

void LoRaCalculator::setParameters(const Parameters &parameters) {
    if (parameters.Npayload < LORA_PAYLOAD_MIN)
        throw std::invalid_argument("Npayload less then 1!");
    if (parameters.Npreamble < LORA_PREAMBLE_MIN)
        throw std::invalid_argument("Npreamble less then 6!");
    if (parameters.spreadFactor < LORA_SPREAD_FACTOR_6 || parameters.spreadFactor > LORA_SPREAD_FACTOR_12)
        throw std::invalid_argument("Spread Factor invalid value!");
    if (parameters.CRC < LORA_CRC_DISABLE || parameters.CRC > LORA_CRC_ENABLE)
        throw std::invalid_argument("CRC invalid value!");
    if (parameters.codingRate < LORA_CODING_RATE_4_5 || parameters.codingRate > LORA_CODING_RATE_4_8) {
        std::cout << (short)parameters.codingRate << std::endl;
        throw std::invalid_argument("Coding Rate invalid value!");
    }
    if (parameters.implicitHeader < LORA_IMPLICIT_HEADER_DISABLE ||
        parameters.implicitHeader > LORA_IMPLICIT_HEADER_ENABLE)
        throw std::invalid_argument("Implicit Header invalid value!");
    if (parameters.dataLowRateOptimization < LORA_DATA_LOW_RATE_OPTIMIZATION_DISABLE ||
        parameters.dataLowRateOptimization > LORA_DATA_LOW_RATE_OPTIMIZATION_ENABLE)
        throw std::invalid_argument("Data Low Rate Optimization invalid value!");
    if (parameters.bandwidth != LORA_BANDWIDTH_7_8_KHZ && parameters.bandwidth != LORA_BANDWIDTH_10_4_KHZ &&
        parameters.bandwidth != LORA_BANDWIDTH_15_6_KHZ && parameters.bandwidth != LORA_BANDWIDTH_20_8_KHZ &&
        parameters.bandwidth != LORA_BANDWIDTH_31_2_KHZ && parameters.bandwidth != LORA_BANDWIDTH_41_7_KHZ &&
        parameters.bandwidth != LORA_BANDWIDTH_62_5_KHZ && parameters.bandwidth != LORA_BANDWIDTH_125_KHZ &&
        parameters.bandwidth != LORA_BANDWIDTH_250_KHZ && parameters.bandwidth != LORA_BANDWIDTH_500_KHZ) {
        throw std::invalid_argument("Bandwitch invalid value!");
    }
    //SX1276 SF=6 required IH=1
    if (parameters.spreadFactor == LORA_SPREAD_FACTOR_6 && parameters.implicitHeader != LORA_IMPLICIT_HEADER_ENABLE) {
        throw std::invalid_argument("With SF=6 implicit header is required!");
    }
    this->parameters = parameters;
    this->results = {};
}

void LoRaCalculator::calculate() {
    results.symbolTime = pow(2, parameters.spreadFactor) / parameters.bandwidth; // formula Tsym=2^SF/BW
    results.symbolRate = 1 / results.symbolTime * 1000; // Rs= 1/Tsym * 1000

    results.preambleSymbols = parameters.Npreamble + 4.25; // formula N+4,25
    results.preambleTime = results.preambleSymbols * results.symbolTime;

    results.payloadSymbols = static_cast<int64_t> (8 + fmax(ceil(
                                                                    (8.0 * parameters.Npayload - 4.0 * parameters.spreadFactor + 28.0 + 16.0 * parameters.CRC -
                                                                     20.0 * parameters.implicitHeader) /
                                                                    (4 * (parameters.spreadFactor - 2 * parameters.dataLowRateOptimization))) * (parameters.codingRate + 4),
                                                            0)); // big formula from datasheet
    results.payloadTime = (double_t) results.payloadSymbols * results.symbolTime;
    results.timeOnAir = results.preambleTime + results.payloadTime;
    results.speedLoRa = parameters.Npayload / results.timeOnAir * 1000 * 8;// *1000 for seconds, *8 for bits
}

std::ostream &operator<<(std::ostream &os, const LoRaCalculator &calculator) {
    return os << "[Payload = " << (short) calculator.parameters.Npayload << ", Preamble = "
              << (short) calculator.parameters.Npreamble
              << ", SF = " << (short) calculator.parameters.spreadFactor << ", CRC = "
              << (short) calculator.parameters.CRC
              << ", Coding Rate = " << (short) calculator.parameters.codingRate << ", Implicit Header = "
              << (short) calculator.parameters.implicitHeader << ", Data Low Rate Optimization = "
              << (short) calculator.parameters.dataLowRateOptimization << ", Bandwidth = "
              << calculator.parameters.bandwidth << "]" << std::endl << "Symbol Time: "
              << calculator.results.symbolTime << " ms" << std::endl
              << "Symbol Rate: "
              << calculator.results.symbolRate << " symbol/sec" << std::endl << "Preamble Symbols: "
              << calculator.results.preambleSymbols << " symbols" << std::endl << "Preamble Time: "
              << calculator.results.preambleTime << " ms" << std::endl << "Payload symbols: "
              << calculator.results.payloadSymbols << " symbols" << std::endl << "Payload Time: "
              << calculator.results.payloadTime << " ms" << std::endl << "Time-on-Air: " << calculator.results.timeOnAir
              << " ms"
              << std::endl << "LoRa speed(payload): " << calculator.results.speedLoRa << " bits/sec" << std::endl;
}



