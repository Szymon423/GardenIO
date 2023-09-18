#include "serial_lib.hpp"
#include "log.hpp"

#include <sys/ioctl.h>


/*
https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp/
*/
Serial::Serial(std::string device_name, BaudRate brate, DataBits dbits, Parity par, StopBits sbits)
: tty(), rc(0), device_name(device_name), brate(brate), par(par), sbits(sbits), dbits(dbits)
{

}

Serial::~Serial() 
{
    Close();
}

void Serial::Open()
{
    serial_port = open(device_name.c_str(), O_RDWR);

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
        case Parity::NONE:
        {
            tty.c_cflag &= ~PARENB;
            break;
        }
        case Parity::EVEN:
        {
            tty.c_cflag |= PARENB;
            tty.c_cflag &= ~PARODD;
            break;
        }
        case Parity::ODD:
        {
            tty.c_cflag |= PARENB;
            tty.c_cflag |= PARODD;
            break;
        }
        default:
        {
            LOG_ERROR("Unknown Parity declaration for: {}", device_name);
            rc = -1;
            return;
        }
    }

    switch (sbits)
    {
        case StopBits::ONE:
        {
            tty.c_cflag &= ~CSTOPB;
            break;
        }
        case StopBits::TWO:
        {
            tty.c_cflag |= CSTOPB;
            break;
        }
        default:
        {
            LOG_ERROR("Unknown stop bits declaration for: {}", device_name);
            rc = -1;
            return;
        }
    }

    // Clear all bits that set the data size 
    tty.c_cflag &= ~CSIZE;
    switch (dbits)
    {
        case DataBits::FIVE:
        {
            tty.c_cflag |= CS5;
            break;
        }
        case DataBits::SIX:
        {
            tty.c_cflag |= CS6;
            break;
        }
        case DataBits::SEVEN:
        {
            tty.c_cflag |= CS7;
            break;
        }
        case DataBits::EIGHT:
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
        
    tty.c_cflag |= CREAD | CLOCAL;              // Turn on READ & ignore ctrl lines (CLOCAL = 1)
    tty.c_lflag &= ~ICANON;                     // noncanonical mode
    tty.c_lflag &= ~ECHO;                       // Disable echo
    tty.c_lflag &= ~ECHOE;                      // Disable erasure
    tty.c_lflag &= ~ECHONL;                     // Disable new-line echo
    tty.c_lflag &= ~ISIG;                       // Disable interpretation of INTR, QUIT and SUSP
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);     // Turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

    tty.c_oflag &= ~OPOST;                      // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR;                      // Prevent conversion of newline to carriage return/line feed
    // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
    // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

    tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
    tty.c_cc[VMIN] = 0;

    // Set in/out baud rate to be 9600
    cfsetispeed(&tty, brate);
    cfsetospeed(&tty, brate);

    // Save tty settings, also checking for error
    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) 
    {
        LOG_ERROR("Error {} from tcsetattr: {}", errno, strerror(errno));
        rc = -1;
        return;
    }

    // initialise read_buffor with '\0'
    memset(&read_buffer, '\0', sizeof(read_buffer));
}

void Serial::Close()
{
    close(serial_port);
}

void Serial::Write(std::string data)
{
    int writeResult = write(serial_port, data.c_str(), sizeof(data.c_str()));

    // Check status
    if (writeResult == -1)
    {
        rc = -1;
        LOG_ERROR("Can not write to Serial: {}", device_name);
    }
}

std::string Serial::Read()
{
    // Read from file
    // We provide the underlying raw array from the readBuffer_ vector to this C api.
    // This will work because we do not delete/resize the vector while this method
    // is called
    std::string data;
    size_t n = read(serial_port, &read_buffer[0], SERIAL_READ_BUFFOR_SIZE);

    if (n < 0) 
    {
        LOG_ERROR("Can not read from Serial: {}", device_name);
    }
    else if(n == 0) 
    {
        data = "";
    }
    else if(n > 0) 
    {
        data = std::string(&read_buffer[0], n);
    }
    return data;
}

void Serial::SetTimeout(int timeout)
{
    tty.c_cc[VTIME] = (cc_t)(timeout / 100);
    tty.c_cc[VMIN] = 0;
}

int32_t Serial::IsAvaliable()
{   
    int32_t ret = 0;
    ioctl(serial_port, FIONREAD, &ret);
    return ret;
}

