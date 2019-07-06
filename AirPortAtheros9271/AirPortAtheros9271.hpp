#ifndef AIR_PORT_ATHEROS_9271_H
#define AIR_PORT_ATHEROS_9271_H

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#pragma clang diagnostic ignored "-Wdocumentation"
#include <IOKit/network/IOEthernetController.h>
#include <IOKit/network/IOEthernetInterface.h>
#include <IOKit/network/IOOutputQueue.h>
#include <IOKit/IOService.h>
#pragma clang diagnostic pop

#define AR9271_FIRMWARE 0x501000
#define AR9271_FIRMWARE_TEXT 0x903000

#define FIRMWARE_DOWNLOAD 0x30
#define FIRMWARE_DOWNLOAD_COMP 0x31

class AirPort_Atheros9271 : public IOEthernetController
{
    OSDeclareDefaultStructors(AirPort_Atheros9271);
    
protected:
    typedef IOEthernetController super;
    
    IOUSBHostDevice *fDevice;
    IOUSBHostInterface *fInterface;
    
    IOUSBHostPipe *fBulkPipe;
    IOUSBHostPipe *fInterruptPipe;
    
    IOEthernetInterface *fNetworkInterface;
    IONetworkStats *fNetworkStats;
    
    IOLock *fCompletionLock = NULL;
    
    bool setConfiguration(int configurationIndex);
    bool resetDevice();
    bool findInterface();
    bool findPipe(IOUSBHostPipe *pipe, UInt8 type, UInt8 direction);
    bool performUpgrade();
    bool createMediumTables();
    bool createNetworkInterface();

    
    void setDevice(IOService *provider);
    void setInterface(IOService *interface);
    void setPipe(IOUSBHostPipe *pipe, OSObject *p);
    void printDeviceInfo();
    void pushDeviceFirmware();
    
    int getDeviceStatus();
    
    IOReturn pipeCommand(UInt8 requestType, UInt8 command, UInt16 address, IOMemoryDescriptor *buffer);
    IOReturn pipeCommand(UInt8 requestType, UInt8 command, UInt16 address, void *buffer, UInt16 length);
    IOReturn getDeviceConfiguration(UInt8 *configNumber);
    IOReturn readFromPipe(IOUSBHostPipe *pipe, IOMemoryDescriptor *buffer, IOByteCount length, IOUSBHostCompletion *completion, IOByteCount *bytesRead);
    IOReturn writeToPipe(IOUSBHostPipe *pipe, IOMemoryDescriptor *buffer, IOByteCount length, IOUSBHostCompletion *completion);
    IOReturn bulkWrite(const void *data, UInt16 length);
    
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
    
    virtual bool willTerminate(IOService *provider, IOOptionBits options) override;
    virtual bool terminate(IOOptionBits options = 0) override;
    virtual bool finalize(IOOptionBits options) override;
    
    virtual IOReturn message(UInt32 type, IOService *provider, void *argument) override;
    
    // IOEtherenetController overrides
    virtual IOReturn getHardwareAddress(IOEthernetAddress *addr) override;
    virtual IOReturn getMaxPacketSize(UInt32 *maxSize) const override;
    virtual IOReturn getPacketFilters(const OSSymbol *group, UInt32 *filters) const override;
    
    virtual IONetworkInterface *createInterface() override;
    virtual bool configureInterface(IONetworkInterface *iface) override;
    
    virtual IOReturn enable(IONetworkInterface *iface) override;
    virtual IOReturn disable(IONetworkInterface *iface) override;
    virtual IOReturn selectMedium(const IONetworkMedium *medium) override;
    virtual IOReturn setMulticastMode(bool active) override;
    virtual IOReturn setMulticastList(IOEthernetAddress *addrs, UInt32 count) override;
    virtual IOReturn setPromiscuousMode(bool active) override;
};

#endif
