#include "serial_lib.hpp"
#include "log.hpp"


Serial::Serial(std::string device_name, int baud_rate, data_bits dbits, parity par, stop_bits sb)
: tty(), rc(0), device_name(device_name), baud_rate(baud_rate), par(par), sbits(sbits), dbits(dbits)
{
    
}

Serial::~Serial() {}

void Serial::open()
{
    int serial_port = open(device_name, type);

    // Check for errors
    if (serial_port < 0) 
    {
        rc = -1;
        LOG_ERROR("Error: {} from open: {}", errno, strerror(errno));
        return;
    }

    // Read in existing settings, and handle any error
    if(tcgetattr(serial_port, &tty) != 0) 
    {
        rc = -1;
        LOG_ERROR("Error: {} from tcgetattr: {}", errno, strerror(errno));
        return;
    }

    switch (par)
    {
        case parity::none:
        {
            tty.c_cflag &= ~PARENB;
            break;
        }
        case parity::even:
        {
            tty.c_cflag |= PARENB;
            tty.c_cflag &= ~PARODD;
            break;
        }
        case parity::odd:
        {
            tty.c_cflag |= PARENB;
            tty.c_cflag |= PARODD;
            break;
        }
        default:
        {
            LOG_ERROR("Unknown parity declaration for: {}", device_name);
            rc = -1;
            return;
        }
    }

    switch (sbits)
    {
        case stop_bits::one:
        {
            tty.c_cflag |= CSTOPB;
            break;
        }
        case stop_bits::two:
        {
            tty.c_cflag &= ~CSTOPB;
            break;
        }
        default:
        {
            LOG_ERROR("Unknown stop bits declaration for: {}", device_name);
            rc = -1;
            return;
        }
    }

    tty.c_cflag &= ~(CS5 | CS6 | CS7 | CS8);
    switch (dbits)
    {
        case data_bits::CS5:
        {
            tty.c_cflag |= CS5;
            break;
        }
        case data_bits::CS6:
        {
            tty.c_cflag |= CS5;
            break;
        }
        case data_bits::CS7:
        {
            tty.c_cflag |= CS7;
            break;
        }
        case data_bits::CS8:
        {
            tty.c_cflag |= CS8;
            break;
        }
        default:
        {
            LOG_ERROR("Unknown data bits declaration for: {}", device_name);
            rc = -1;
            return;
        }
    }



}