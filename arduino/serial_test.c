#include <errno.h>
#include <fcntl.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>


int set_interface_attribs(int fd, int speed)
{
    struct termios tty;

    if (tcgetattr(fd, &tty) < 0) {
        printf("!!FATAL ERROR!! : %s\n", strerror(errno));
        return -1;
    }

    cfsetospeed(&tty, (speed_t)speed);
    cfsetispeed(&tty, (speed_t)speed);

    tty.c_cflag |= (CLOCAL | CREAD);    /* ignore modem controls */
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;         /* 8-bit characters */
    tty.c_cflag &= ~PARENB;     /* no parity bit */
    tty.c_cflag &= ~CSTOPB;     /* only need 1 stop bit */
    tty.c_cflag &= ~CRTSCTS;    /* no hardware flowcontrol */

    /* setup for non-canonical mode */
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
    tty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    tty.c_oflag &= ~OPOST;

    /* fetch bytes as they become available */
    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 1;

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        printf("!!FATAL ERROR!! : %s\n", strerror(errno));
        return -1;
    }
    return 0;
}


void main(int argc, char* argv[]) {
    
    char* msg = (char*)malloc(5);
    strcpy(msg, argv[1]);
    if (!msg) {
        printf("no arg found. please enter 1,2 or 3\n");
        return;
    } else {
        msg[1] = '\n';
        printf("choice: %s\n", msg);

    }
    size_t msg_len = strlen(msg);
    int serCom = open("/dev/ttyACM0",O_RDWR | O_NONBLOCK);
    set_interface_attribs(serCom, B9600);
    sleep(2);
    int n = write(serCom, msg, msg_len);
    if (n!=msg_len) {
        printf("!!WARNING!! : Unable to write everything!!\n");
    }
    close(serCom);

}