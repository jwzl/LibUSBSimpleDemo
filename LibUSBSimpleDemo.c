/*
* Author: Qing
* Date: 2017/09/21
* Description: This is a simple libusb bulk transfer demo.
*
* Reference: https://github.com/tytouf/libusb-cdc-example
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <errno.h>
#include <libusb-1.0/libusb.h>
#include <unistd.h>

#define TEST_OUT_ENDPOINT  	(0x02)
#define TEST_IN_ENDPOINT  	(0x82)
#define USB_DEVICE_VID		(0x154f)	
#define USB_DEVICE_PID		(0x154f)
#define USB_DEVICE_INTERFACE_INDEX	(0)

static struct libusb_device_handle *devh = NULL;

int do_test_libusb(libusb_device_handle *devh){
	int rc = 0;
	//TODO:####

	return rc;
}

int main(int argc, char *argv[])
{
	int rc;

	/* 1. Init the libusb library. */
	rc = libusb_init(NULL);
	if (rc < 0) {
		fprintf(stderr, "Error initializing libusb: %s\n", libusb_error_name(rc));
		exit(1);
	}

	/* 2. Open the usb device. */
	devh = libusb_open_device_with_vid_pid(NULL, USB_DEVICE_VID, USB_DEVICE_PID);
	if (!devh) {
		fprintf(stderr, "Error finding USB device\n");
		goto out;
	}

	/* 3. bind the interface. */
	rc = libusb_claim_interface(devh, USB_DEVICE_INTERFACE_INDEX);
	if (rc < 0) {
		fprintf(stderr, "Error claiming interface: %s\n", libusb_error_name(rc));
		goto out;
	}

	/* Clear the halt state. */
	/* NOTE: you must reset the device! */
	libusb_reset_device(devh);
	usleep(10000);  // this is maskable
	libusb_clear_halt(devh,TEST_OUT_ENDPOINT);
    libusb_clear_halt(devh,TEST_IN_ENDPOINT);
	usleep(10000);   // this is maskable

	/*4. do usb communication !*/
	char buf[128] = {0};
	char buf2[128] = {0};
	buf[0] = 0x10;
	buf[1] = 0x04;
	buf[2] = 2;	
	int BytesWritten = 0 ;
	rc = libusb_bulk_transfer(devh,TEST_OUT_ENDPOINT, (unsigned char *)buf, 3, &BytesWritten,0);
	if(rc!= 0){
		fprintf(stderr, "Error transfer: %s\n", libusb_error_name(rc));
		goto out;
	}

	rc =libusb_bulk_transfer(devh,TEST_IN_ENDPOINT,(unsigned char *)buf2,16,&BytesWritten,0);	
	if(rc!= 0){
    	fprintf(stderr, "Error transfer: %s\n", libusb_error_name(rc));
		goto out;
	}
    printf("Bytes:%d :%x %x\n",BytesWritten,buf2[0],buf2[1]);	
#if  0	
	buf[0] = 0x10;
	buf[1] = 0x04;
	buf[2] = 2;	
	BytesWritten = 0 ;
	rc = libusb_bulk_transfer(devh,TEST_OUT_ENDPOINT, (unsigned char *)buf, 3, &BytesWritten,10000);
	if(rc!= 0){
		fprintf(stderr, "Error transfer: %s\n", libusb_error_name(rc));
		goto out;
	}
	
	rc =libusb_bulk_transfer(devh,TEST_IN_ENDPOINT,(unsigned char *)buf2,16,&BytesWritten,1000);	
	if(rc!= 0){
    	fprintf(stderr, "Error transfer: %s\n", libusb_error_name(rc));
		goto out;
	}
    printf("Bytes:%d :%x %x\n",BytesWritten,buf2[0],buf2[1]);
#endif
	/*5. Release the interface. */
	libusb_release_interface(devh, USB_DEVICE_INTERFACE_INDEX);
out:
	if (devh)
		libusb_close(devh);
	libusb_exit(NULL);
	return rc;
}
