#pragma once

#include <stdio>
#include <stdlib>
#include <atomic>
#include <thread>
#include <mutex>
#include <string>
#include <spidev_lib++.h>

#define TX_BUFFER_SIZE 32
#define RX_BUFFER_SIZE 32

typedef uint8_t byte;



class DataTransferSPI
{
private:
    const std::string class_name = "[DataTransferSPI]"
    SPI* mySPI = nullptr;
    spi_config_t spi_config;
    byte tx_buffer[TX_BUFFER_SIZE];
    byte rx_buffer[RX_BUFFER_SIZE];
    time_t cycle_period;
    std::atomic_bool keep_running{ false };
    std::thread spi_thread;
    std::mutex mtx;

    void Prepare();
    void ReadingFunction();

public:
    DataTransferSPI() = default;
    ~DataTransferSPI() = default;
    void SetDataExchangePeriod(time_t cycle);
    void Start();
    void Stop();
    

};