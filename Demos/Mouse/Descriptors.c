/*
             MyUSB Library
     Copyright (C) Dean Camera, 2007.
              
  dean [at] fourwalledcubicle [dot] com
      www.fourwalledcubicle.com

 Released under the GPL Licence, Version 3
*/

#include "Descriptors.h"

USB_Descriptor_Device_t DeviceDescriptor PROGMEM =
{
	Header:                 {Size: sizeof(USB_Descriptor_Device_t), Type: DTYPE_Device},
		
	USBSpecification:       0x0200,
	Class:                  0x00,
	SubClass:               0x00,	
	Protocol:               0x00,
				
	Endpoint0Size:          ENDPOINT_CONTROLEP_SIZE,
		
	VendorID:               0x0000,
	ProductID:              USB_PRODUCT_ID('M', 'U'),
	ReleaseNumber:          0x0000,
		
	ManafacturerStrIndex:   0x01,
	ProductStrIndex:        0x02,
	SerialNumStrIndex:      0x03,
		
	NumberOfConfigurations: CONFIGURATIONS
};
	
USB_Descriptor_Configuration_t ConfigurationDescriptor PROGMEM =
{
	Config:
		{
			Header:                 {Size: sizeof(USB_Descriptor_Configuration_Header_t), Type: DTYPE_Configuration},

			TotalConfigurationSize: (  sizeof(USB_Descriptor_Configuration_Header_t)
			                         + sizeof(USB_Descriptor_Interface_t)
									 + sizeof(USB_Descriptor_HID_t) 
	                                 + sizeof(USB_Descriptor_Endpoint_t)
			                        ),
			TotalInterfaces:        1,
				
			ConfigurationNumber:    1,
			ConfigurationStrIndex:  NO_DESCRIPTOR_STRING,
				
			ConfigAttributes:       CONFIG_ATTRIBUTES,
			
			MaxPowerConsumption:    USB_CONFIG_POWER_MA(100)
		},
		
	Interface:
		{
			Header:                 {Size: sizeof(USB_Descriptor_Interface_t), Type: DTYPE_Interface},

			InterfaceNumber:        MOUSE_INTERFACE_NUMBER,
			AlternateSetting:       MOUSE_INTERFACE_ALTERNATE,
			
			TotalEndpoints:         MOUSE_INTERFACE_ENDPOINTS,
				
			Class:                  MOUSE_INTERFACE_CLASS,
			SubClass:               MOUSE_INTERFACE_SUBCLASS,
			Protocol:               MOUSE_INTERFACE_PROTOCOL,
				
			InterfaceStrIndex:      NO_DESCRIPTOR_STRING
		},

	MouseHID:
		{
			Header:                 {Size: sizeof(USB_Descriptor_HID_t),
									 Type: DTYPE_HID},
									 
			HIDSpec:                0x1001,
			CountryCode:            0x00,
			TotalHIDDescriptors:    0x01,
			HIDReportType:          0x22,
			HIDReportLength:        sizeof(USB_HID_Report_Mouse_t)
		},

	MouseEndpoint:
		{
			Header:                 {Size: sizeof(USB_Descriptor_Endpoint_t),
									 Type: DTYPE_Endpoint},
										 
			EndpointAddress:        (ENDPOINT_DESCRIPTOR_DIR_IN | MOUSE_EPNUM),
			Attributes:             0x03,
			EndpointSize:           MOUSE_EPSIZE,
			PollingIntervalMS:      0x02
		}	
};

USB_Descriptor_Language_t LanguageString PROGMEM =
{
	Header:                 {Size: sizeof(USB_Descriptor_Language_t), Type: DTYPE_String},
		
	LanguageID:             LANGUAGE_ID_ENG
};

USB_Descriptor_String_t ManafacturerString PROGMEM =
{
	Header:                 {Size: USB_STRING_LEN(11), Type: DTYPE_String},
		
	UnicodeString:          {'D','e','a','n',' ','C','a','m','e','r','a'}
};

USB_Descriptor_String_t ProductString PROGMEM =
{
	Header:                 {Size: USB_STRING_LEN(16), Type: DTYPE_String},
		
	UnicodeString:          {'M','y','U','S','B',' ','M','o','u','s','e',' ','D','e','m','o'}
};

USB_Descriptor_String_t VersionString PROGMEM =
{
	Header:                 {Size: USB_STRING_LEN(5), Type: DTYPE_String},
		
	UnicodeString:          {'0','.','1','.','0'}
};

bool USB_GetDescriptor(const uint8_t Type, const uint8_t Index,
                       void** DescriptorAddr, uint16_t* Size)
{
	switch (Type)
	{
		case DTYPE_Device:
			*DescriptorAddr = (void*)&DeviceDescriptor;
			*Size           = sizeof(USB_Descriptor_Device_t);
			return true;
		case DTYPE_Configuration:
			*DescriptorAddr = (void*)&ConfigurationDescriptor;
			*Size           = sizeof(USB_Descriptor_Configuration_t);
			return true;
		case DTYPE_String:
			switch (Index)
			{
				case 0x00:
					*DescriptorAddr = (void*)&LanguageString;
					*Size           = sizeof(USB_Descriptor_Language_t);
					return true;
				case 0x01:
					*DescriptorAddr = (void*)&ManafacturerString;
					*Size           = pgm_read_byte(&ManafacturerString.Header.Size);
					return true;
				case 0x02:
					*DescriptorAddr = (void*)&ProductString;
					*Size           = pgm_read_byte(&ProductString.Header.Size);
					return true;
				case 0x03:
					*DescriptorAddr = (void*)&VersionString;
					*Size           = pgm_read_byte(&VersionString.Header.Size);
					return true;
			}
			
			break;
		case DTYPE_HID:
			*DescriptorAddr = (void*)&ConfigurationDescriptor.MouseHID;
			*Size           = sizeof(USB_Descriptor_HID_t);

			return true;
		case DTYPE_Report:
			*DescriptorAddr = (void*)&MouseReport;
			*Size           = sizeof(USB_HID_Report_Mouse_t);

			return true;
	}
		
	return false;
}