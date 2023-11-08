#include "spi.hpp"
#include "log.hpp"


void DataTransferSPI::Prepare()
{
	spi_config.mode = 0;
	spi_config.speed = 1000000;
	spi_config.delay = 0;
	spi_config.bits_per_word = 8;
}


void DataTransferSPI::SetDataExchangePeriod(time_t cycle)
{
	cycle_period = cycle;
	LOG_TRACE("{} cycle period set to {} ms", class_name, cycle_period);
}


void DataTransferSPI::ReadingFunction()
{
	memset(tx_buffer, 0, TX_BUFFER_SIZE);
	memset(rx_buffer, 0, RX_BUFFER_SIZE);

	mySPI = new SPI("/dev/spidev1.0", &spi_config);
	if (!mySPI->begin())
	{
		LOG_TRACE("{} unable to start SPI communication.", class_name);
		keep_running = false;
		return
	}
	LOG_TRACE("{} started SPI communication.", class_name);
	
	while (keep_running && recieved_value)
	{
		std::unique_lock lock(mtx);
		mySPI->xfer(tx_buffer, strlen((char*)tx_buffer), rx_buffer, strlen((char*)tx_buffer));
		lock.unlock();

		std::this_thread::sleep_for(std::chrono::milliseconds(cycle_period));
	}
	// mySPI->end();
	delete mySPI; 
}


void DataTransferSPI::Start()
{
	keep_running = true;
	spi_thread = std::thread(ReadingFunction);
}


void DataTransferSPI::Stop()
{
	keep_running = false
	spi_thread.join();
}