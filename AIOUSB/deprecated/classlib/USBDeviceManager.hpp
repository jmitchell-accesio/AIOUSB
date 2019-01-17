/**
 * @file   USBDeviceManager.hpp
 * @author $Format: %an <%ae>$
 * @date   $Format: %ad$
 * @brief class USBDeviceManager, OperationFailedException, IllegalArgumentException declarations
 */

#if ! defined( USBDeviceManager_hpp )
#define USBDeviceManager_hpp

#include "CppCommon.h"
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <aiousb.h>
#include <USBDeviceBase.hpp>


namespace AIOUSB {

/**
 * @brief Class USBDeviceManager manages all the USB devices on the bus. It scans the bus and builds
 * a list of all the devices found. It also initializes and terminates use of the underlying
 * AIOUSB module.
 */
class USBDeviceManager {


public:
	/** The version number of this Java class library. */
	static const std::string VERSION_NUMBER;

	/** The version date of this Java class library. */
	static const std::string VERSION_DATE;

	static const int MIN_PRODUCT_ID				= 0;
	static const int MAX_PRODUCT_ID				= 0xffff;



protected:
	USBDeviceArray deviceList;					// contains all the ACCES devices found on the bus
	static const unsigned long OPEN_PATTERN = 0x786938f5;	// unique, random value
	unsigned long openStatus;					// if openStatus == OPEN_PATTERN, module is initialized
	static const std::string MESSAGE_NOT_OPEN;



protected:
	void emptyDeviceList();



public:
	USBDeviceManager();
	virtual ~USBDeviceManager();

	/*
	 * properties
	 */

	virtual std::ostream &print( std::ostream &out );
	USBDeviceManager &printDevices();

	/**
	 * Gets the version number of the underlying AIOUSB module.
	 * @return The AIOUSB module version number as a string with the form, "1.78".
	 */

	std::string getAIOUSBVersion() const {
		return AIOUSB_GetVersion();
	}	// getAIOUSBVersion()

	/**
	 * Gets the version date of the underlying AIOUSB module.
	 * @return The AIOUSB module version date as a string with the form, "15 November 2009".
	 */

	std::string getAIOUSBVersionDate() const {
		return AIOUSB_GetVersionDate();
	}	// getAIOUSBVersionDate()

	/*
	 * utilities
	 */

	static std::string productIDToName( int productID );
	static StringArray productIDToName( const IntArray &productID );
	static int productNameToID( const std::string &productName );
	static IntArray productNameToID( const StringArray &productName );

	/*
	 * operations
	 */

	/**
	 * Prints the properties of all the devices found on the bus to the standard output device.
	 * This function is similar to <i>printDevices()</i> but is implemented
	 * by the underlying AIOUSB module and produces different output than <i>printDevices()</i>. Mainly
	 * useful for diagnostic purposes.
	 */

	void listDevices() const {
		AIOUSB_ListDevices();
	}	// listDevices()

	/**
	 * Gets the string representation of an AIOUSB result code, useful mainly for debugging purposes.
	 * This method is also used to convert an AIOUSB result code to a string when an
	 * OperationFailedException is thrown in response to an AIOUSB failure.
	 * Although this method is <i>static</i>, an instance of USBDeviceManager must be created
	 * and be "open" for use before this method can be used. This stipulation is imposed because the
	 * underlying library must be initialized in order for result code lookups to succeed, and that
	 * initialization occurs only when an instance of USBDeviceManager is created and its
	 * <i>open()</i> method is called.
	 * @param result an AIOUSB result code.
	 * @return The string representation of <i>result</i>.
	 */

	static std::string getResultCodeAsString( int result ) {
          return AIOUSB_GetResultCodeAsString( result );
	}	// getResultCodeAsString()

	/**
	 * Tells if the USB device manager has been "opened" for use <i>(see open())</i>.
	 * @return <i>True</i> indicates that the device manager is open and ready to be used; <i>false</i>
	 * indicates that it is not open.
	 */

	bool isOpen() const {
		return openStatus == OPEN_PATTERN;
	}	// isOpen()

	USBDeviceManager &open();
	USBDeviceManager &close();
	USBDeviceManager &scanForDevices();
	USBDeviceArray getDeviceByProductID( int productID ) const;
	USBDeviceArray getDeviceByProductID( int minProductID, int maxProductID ) const;
	USBDeviceArray getDeviceByProductID( const IntArray &productIDs ) const;
	USBDeviceArray getDeviceBySerialNumber( __uint64_t serialNumber ) const;



};	// class USBDeviceManager



/**
 * Class OperationFailedException is thrown whenever an operation attempted on a device fails. The
 * message is either generated by this Java class library or consists of the string representation
 * of an error code returned by the AIOUSB module.
 */

class OperationFailedException : public std::runtime_error {
public:

	/**
	 * Constructs the exception from an AIOUSB module error code.
	 * @param result AIOUSB module result code.
	 */

	OperationFailedException( int result )
		: std::runtime_error( USBDeviceManager::getResultCodeAsString( result ) ) {
	}	// OperationFailedException()

	/**
	 * Constructs the exception from a simple string message.
	 * @param message The text of the message.
	 */

	OperationFailedException( const std::string &message )
		: std::runtime_error( message ) {
	}	// OperationFailedException()

};	// class OperationFailedException





/**
 * Class IllegalArgumentException is thrown whenever an invalid argument is passed to a method.
 */

class IllegalArgumentException : public std::invalid_argument {
public:

	/**
	 * Constructs the exception from a simple string message.
	 * @param message The text of the message.
	 */

	IllegalArgumentException( const std::string &message )
		: std::invalid_argument( message ) {
	}	// IllegalArgumentException()

};	// class IllegalArgumentException



}	// namespace AIOUSB

#endif

/* end of file */