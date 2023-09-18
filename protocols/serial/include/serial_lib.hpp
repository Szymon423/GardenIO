#include <stdio.h>
#include <string>

// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

enum acces
{
    O_RDONLY = O_RDONLY,
    O_WRONLY = O_WRONLY,
    O_RDWR = O_RDWR
}

enum parity
{
    none,
    even,
    odd
}

enum data_bits
{
    CS5 = CS5,
    CS6 = CS6,
    CS7 = CS7,
    CS8 = CS8
}

enum stop_bits
{
    one,
    two
}

class Serial
{
private:
    int rc;
    parity par;
    stop_bits sbits;
    data_bits dbits;
    struct termios tty; 
    std::string device_name;

public:
    Serial(std::string device_name, int baud_rate, parity par, stop_bits sbits);
    ~Serial();
    void open();
}