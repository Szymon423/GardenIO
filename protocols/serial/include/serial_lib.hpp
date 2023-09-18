#include <stdio.h>
#include <string>

// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

#define SERIAL_READ_BUFFOR_SIZE 1024


enum Parity
{
    NONE,
    EVEN,
    ODD
};

enum DataBits
{
    FIVE,
    SIX,
    SEVEN,
    EIGHT
};

enum StopBits
{
    ONE,
    TWO
};

enum BaudRate
{
    BR300 = B300,
    BR1200 = B1200,
    BR9600 = B9600,
    BR19200 = B19200,
    BR57600 = B57600,
    BR115200 = B115200
};

class Serial
{
private:
    BaudRate brate;
    Parity par;
    StopBits sbits;
    DataBits dbits;
    struct termios tty; 
    std::string device_name;
    int serial_port;
    char read_buffer[SERIAL_READ_BUFFOR_SIZE];
    int rc;

public:
    Serial(std::string device_name, BaudRate brate, DataBits dbits, Parity par, StopBits sbits);
    ~Serial();
    void Open();
    void Close();
    void SetTimeout(int timeout);
    int32_t IsAvaliable();
    void Write(std::string data);
    std::string Read();
};