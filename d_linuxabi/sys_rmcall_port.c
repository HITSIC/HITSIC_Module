#include "sys_rmcall_port.h"

#if defined(CMODULE_USE_RMCALL) && (CMODULE_USE_RMCALL > 0)

#include <sys_rmcall.h>

#define SYSLOG_TAG "RMCALL"
#define SYSLOG_LVL RMCALL_SYSLOG_LVL
#include <inc_syslog.h>

extern rmcall_t rmcall_mcu;

int ttyPortId = -1;

typedef struct _rmcall_xferObj
{
    void *data;
    uint32_t size;
    pthread_t pid;
    pthread_mutex_t mutex;
    pthread_cond_t newTask;
}rmcall_xferObj_t;

rmcall_xferObj_t rmcall_txObj, rmcall_rxObj;

#ifdef __cplusplus
extern "C" {
#endif

void RMCALL_HOST_ObjCleanup(void *arg)
{
    rmcall_xferObj_t *obj = (rmcall_xferObj_t*) arg;

    obj->data = NULL;
    obj->size = 0U;

    pthread_mutex_unlock(&obj->mutex);
}

void *RMCALL_HOST_TxThread(void *arg)
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL); // default: PTHREAD_CANCEL_DEFFERED

    while(1)
    {
        pthread_cleanup_push(RMCALL_HOST_ObjCleanup, (void*)&rmcall_txObj);
        pthread_mutex_lock(&rmcall_txObj.mutex);
        while(NULL == rmcall_txObj.data)
        {
            SYSLOG_V("Tx wait");
            pthread_cond_wait(&rmcall_txObj.newTask, &rmcall_txObj.mutex);
        }

        // got task, do uart xfer
        SYSLOG_V("Tx send");
        write(ttyPortId, (char*)(rmcall_txObj.data), rmcall_txObj.size);
        rmcall_txObj.data = NULL;

        pthread_mutex_unlock(&rmcall_txObj.mutex);
        pthread_cleanup_pop(0);
        //sleep(1);
        RMCALL_TxIsr(&rmcall_mcu);
    }
}

void *RMCALL_HOST_RxThread(void *arg)
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL); // default: PTHREAD_CANCEL_DEFFERED

    while(1)
    {
        pthread_cleanup_push(RMCALL_HOST_ObjCleanup, (void*)&rmcall_rxObj);
        pthread_mutex_lock(&rmcall_rxObj.mutex);
        while(NULL == rmcall_rxObj.data)
        {
            SYSLOG_V("Rx wait");
            pthread_cond_wait(&rmcall_rxObj.newTask, &rmcall_rxObj.mutex);
        }

        // got task, do uart xfer
        SYSLOG_V("Rx recv");
        char *recvPtr =(char*)rmcall_rxObj.data;
        uint32_t recvCnt = 0U, remainCnt = rmcall_rxObj.size;
        do{
            recvCnt = read(ttyPortId, recvPtr, remainCnt);
            recvPtr += recvCnt;
            remainCnt -=recvCnt;
            SYSLOG_V("Rx recv %d", recvCnt);
        }while(0U != remainCnt);
        rmcall_rxObj.data = NULL;

        pthread_mutex_unlock(&rmcall_rxObj.mutex);
        pthread_cleanup_pop(0);
        //sleep(1);
        RMCALL_RxIsr(&rmcall_mcu);
    }
}

mstatus_t RMCALL_PreInitHook(rmcall_t *_inst)
{
    extern char* param_ttyPath;
    SYSLOG_I("Open serial port \"%s\"", param_ttyPath);

    ttyPortId = open(param_ttyPath, O_RDWR);

    // Check for errors
    
    if (ttyPortId < 0) {
        SYSLOG_E("Error %i from open: %s", errno, strerror(errno));
        assert(0);
    }

    // Create new termios struct, we call it 'tty' for convention
    // No need for "= {0}" at the end as we'll immediately write the existing
    // config to this struct
    struct termios tty;

    // Read in existing settings, and handle any error
    // NOTE: This is important! POSIX states that the struct passed to tcsetattr()
    // must have been initialized with a call to tcgetattr() overwise behaviour
    // is undefined
    int retVal = tcgetattr(ttyPortId, &tty);

    if(retVal != 0) 
    {
        SYSLOG_E("Error %i from tcgetattr: %s", errno, strerror(errno));
        assert(0);
    }


    tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
    // tty.c_cflag |= PARENB;  // Set parity bit, enabling parity

    tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
    // tty.c_cflag |= CSTOPB;  // Set stop field, two stop bits used in communication

    tty.c_cflag &= ~CSIZE; // Clear all the size bits, then use one of the statements below
    // tty.c_cflag |= CS5; // 5 bits per byte
    // tty.c_cflag |= CS6; // 6 bits per byte
    // tty.c_cflag |= CS7; // 7 bits per byte
    tty.c_cflag |= CS8; // 8 bits per byte (most common)

    tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
    // tty.c_cflag |= CRTSCTS;  // Enable RTS/CTS hardware flow control

    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

    tty.c_lflag &= ~ICANON; // Disable Canonical mode
    tty.c_lflag &= ~ECHO; // Disable echo
    tty.c_lflag &= ~ECHOE; // Disable erasure
    tty.c_lflag &= ~ECHONL; // Disable new-line echo
    tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes
    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
    // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT IN LINUX)
    // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT IN LINUX)

    tty.c_cc[VTIME] = 0;    // Wait for up to 1s (VTIME x 100 ms), returning as soon as any data is received.
    tty.c_cc[VMIN] = 1;     // block the operation if data in buffer < VMIN.

    // Set in/out baud rate to be 115200
    // Set in/out baud rate to be 115200
    cfsetispeed(&tty, B115200);
    cfsetospeed(&tty, B115200);

    // Save tty settings, also checking for error
    if (tcsetattr(ttyPortId, TCSANOW, &tty) != 0) 
    {
        SYSLOG_E("Error %i from tcsetattr: %s", errno, strerror(errno));
        assert(0);
    }

    SYSLOG_I("Port opened.\n");

    rmcall_txObj.data = NULL;
    rmcall_txObj.size = 0U;
    pthread_mutex_init(&rmcall_txObj.mutex, NULL);
    pthread_cond_init(&rmcall_txObj.newTask, NULL);
    pthread_create(&rmcall_txObj.pid, NULL, RMCALL_HOST_TxThread, NULL);

    rmcall_rxObj.data = NULL;
    rmcall_rxObj.size = 0U;
    pthread_mutex_init(&rmcall_rxObj.mutex, NULL);
    pthread_cond_init(&rmcall_rxObj.newTask, NULL);
    pthread_create(&rmcall_rxObj.pid, NULL, RMCALL_HOST_RxThread, NULL);

    return mStatus_Success;
}

mstatus_t RMCALL_AftDeInitHook(rmcall_t *_inst)
{
    pthread_cancel(rmcall_txObj.pid);
    while(ESRCH != pthread_kill(rmcall_txObj.pid, 0));
    pthread_mutex_destroy(&rmcall_txObj.mutex);
    pthread_cond_destroy(&rmcall_txObj.newTask);

    pthread_cancel(rmcall_rxObj.pid);
    while(ESRCH != pthread_kill(rmcall_rxObj.pid, 0));
    pthread_mutex_destroy(&rmcall_rxObj.mutex);
    pthread_cond_destroy(&rmcall_rxObj.newTask);

    close(ttyPortId);
    SYSLOG_I("Port closed.");
}

mstatus_t RMCALL_HOST_Tx(void *_data, uint32_t _dataSize)
{
    if(0 == pthread_mutex_lock(&rmcall_txObj.mutex))
    {
        rmcall_txObj.data = _data;
        rmcall_txObj.size = _dataSize;
        pthread_cond_signal(&rmcall_txObj.newTask);
        pthread_mutex_unlock(&rmcall_txObj.mutex);
        return mStatus_Success;
    }
    else
    {
        return mStatus_Fail;
    }
}
mstatus_t RMCALL_HOST_Rx(void *_data, uint32_t _dataSize)
{
    if(0 == pthread_mutex_lock(&rmcall_rxObj.mutex))
    {
        rmcall_rxObj.data = _data;
        rmcall_rxObj.size = _dataSize;
        pthread_cond_signal(&rmcall_rxObj.newTask);
        pthread_mutex_unlock(&rmcall_rxObj.mutex);
        return mStatus_Success;
    }
    else
    {
        return mStatus_Fail;
    }
}
void RMCALL_HOST_TxAbort(void)
{
    pthread_cancel(rmcall_txObj.pid);
    while(ESRCH != pthread_kill(rmcall_txObj.pid, 0));
    rmcall_txObj.data = NULL;
    rmcall_txObj.size = 0U;
    pthread_mutex_init(&rmcall_txObj.mutex, NULL);
    pthread_cond_init(&rmcall_txObj.newTask, NULL);
    pthread_create(&rmcall_txObj.pid, NULL, RMCALL_HOST_TxThread, NULL);
}
void RMCALL_HOST_RxAbort(void)
{
    pthread_cancel(rmcall_rxObj.pid);
    while(ESRCH != pthread_kill(rmcall_rxObj.pid, 0));
    rmcall_rxObj.data = NULL;
    rmcall_rxObj.size = 0U;
    pthread_mutex_init(&rmcall_rxObj.mutex, NULL);
    pthread_cond_init(&rmcall_rxObj.newTask, NULL);
    pthread_create(&rmcall_rxObj.pid, NULL, RMCALL_HOST_RxThread, NULL);
}

#ifdef __cplusplus
}
#endif

#endif // CMODULE_USE_RMCALL
