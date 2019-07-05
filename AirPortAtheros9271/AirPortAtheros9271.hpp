#ifndef AIR_PORT_ATHEROS_9271_H
#define AIR_PORT_ATHEROS_9271_H

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#pragma clang diagnostic ignored "-Wdocumentation"
#include <IOKit/network/IOEthernetController.h>
#include <IOKit/IOService.h>
#pragma clang diagnostic pop

enum DeviceState
{
    kAthUnknown,
    kAthInitialize,
    kAthFirmwareWrite,
    kAthFirmwareWritten,
    kAthResetComplete,
    kAthUpdateComplete,
    kAthUpdateNotNeeded,
    kAthUpdateAborted,
};

class AirPort_Atheros9271 : public IOEthernetController
{
    OSDeclareDefaultStructors(AirPort_Atheros9271);
    
protected:
    typedef IOService super;
    
    IOUSBHostDevice *fDevice;
    IOUSBHostInterface *fInterface;
    
    IOUSBHostPipe *fBulkPipe;
    IOUSBHostPipe *fControlPipe;
    IOUSBHostPipe *fInterruptPipe;
    
    bool setConfiguration(int configurationIndex);
    bool resetDevice();
    bool findInterface();
    bool findPipe(IOUSBHostPipe *pipe, UInt8 type, UInt8 direction);
    bool performUpgrade();
    bool continuousRead();
    
    void setDevice(IOService *provider);
    void setInterface(IOService *interface);
    void setPipe(IOUSBHostPipe *pipe, OSObject *p);
    void printDeviceInfo();
    void pushDeviceFirmware();
    void readCompletion(void *target, void *parameter, IOReturn status, UInt32 bufferSizeRemaining);
    
    int getDeviceStatus();
    
    IOReturn getDeviceConfiguration(UInt8 *configNumber);
    IOReturn hciCommand(void *command, UInt16 length);
    IOReturn readFromPipe(IOUSBHostPipe *pipe, IOMemoryDescriptor *buffer, UInt32 length, IOUSBHostCompletion *completion, UInt32 *bytesRead);
    IOReturn writeToPipe(IOUSBHostPipe *pipe, IOMemoryDescriptor *buffer, UInt32 length, IOUSBHostCompletion *completion);
    IOReturn bulkWrite(const void *data, uint16_t length);
    
private:
    UInt16 fVendorId;
    UInt16 fProductId;
    UInt16 fDeviceRev;
    
public:
    // IOKit overrides
    virtual bool init(OSDictionary *props = 0) override;
    virtual void free(void) override;
    
    virtual IOService *probe(IOService *provider, SInt32 *score) override;
    
    virtual bool attach(IOService *provider) override;
    virtual void detach(IOService *provider) override;
    
    virtual bool start(IOService *provider) override;
    virtual void stop(IOService *provider) override;
    
    virtual IOReturn message(UInt32 type, IOService *provider, void *argument) override;

    virtual bool willTerminate(IOService *provider, IOOptionBits options) override;
    virtual bool terminate(IOOptionBits options = 0) override;
    virtual bool finalize(IOOptionBits options) override;
    
    // IOEtherenetController overrides
    virtual IOReturn getHardwareAddress(IOEthernetAddress *addr) override;
};

#endif
