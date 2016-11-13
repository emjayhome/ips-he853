#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include "hidapi.h"

#ifdef _WIN32
	#include <windows.h>
#else
	#include <unistd.h>
#endif

int main(int argc, char* argv[])
{
	hid_device *handle;
  int res;

#ifdef WIN32
	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);
#endif

  if(argc != 4) {
      printf("Wrong number of arguments\n");
      return -1;
  }

  // First argument is executable name only

  // Argument 2 expects device number (System and Home Code)
  // Written as 1 (On = top) or 0 (Off = bottom)
  if(strlen(argv[1]) != 10) {
      printf("Argument 1 invalid length - expected 10\n");
      return -1;
  }
  unsigned char inputs[] = {0x00, 0x00, 0x00};
  if(argv[1][0] == '0') {
    inputs[0] += 64; 
  }
  if(argv[1][1] == '0') {
    inputs[0] += 16; 
  }
  if(argv[1][2] == '0') {
    inputs[0] += 4; 
  }
  if(argv[1][3] == '0') {
    inputs[0] += 1; 
  }
  if(argv[1][4] == '0') {
    inputs[1] += 64; 
  }
  if(argv[1][5] == '0') {
    inputs[1] += 16; 
  }
  if(argv[1][6] == '0') {
    inputs[1] += 4; 
  }
  if(argv[1][7] == '0') {
    inputs[1] += 1; 
  }
  if(argv[1][8] == '0') {
    inputs[2] += 64; 
  }
  if(argv[1][9] == '0') {
    inputs[2] += 16; 
  }

  // Argument 3 expects on (1) or off (0)
  if(strlen(argv[2]) != 1) {
      printf("Argument 2 invalid length - expected 1\n");
      return -1;
  }
  if(argv[2][0] == '0') {
    inputs[2] += 4; 
  } else {
    inputs[2] += 5;
  }
	
  // Argument 4 is number of retries
  // 0 < x < 10
  int repeat = atoi(argv[3]);
  if((repeat < 1) || (repeat > 9)) {
      printf("Argument 3 invalid valid - expected 0 < x < 10\n");
      return -1;
  }

	if (hid_init()) {
    printf("Failed to initialize HID\n");
		return -1;
  }

	// Open the device using the VID, PID,
	// and optionally the Serial number.
	////handle = hid_open(0x4d8, 0x3f, L"12345");
	handle = hid_open(0x4d9, 0x1357, NULL);
	if (!handle) {
		printf("Unable to open HE853\n");
 		return -1;
	}

  bool error = false;
  for (int i=0; i<repeat; i++) {

    // WR 00  01 00 20 03 CA 00 00 00 - constant
    unsigned char buf01[] = {0x00, 0x01, 0x00, 0x20, 0x03, 0xCA, 0x00, 0x00, 0x00};
	  res = hid_write(handle, buf01, 9);
	  if (res < 0) {
		  printf("Unable to write block 01\n");
      error = true;
    }

    // WR 00  02 00 20 60 60 20 18 12 - constant
    unsigned char buf02[] = {0x00, 0x02, 0x00, 0x20, 0x60, 0x60, 0x20, 0x18, 0x12};
	  res = hid_write(handle, buf02, 9);
	  if (res < 0) {
		  printf("Unable to write block 02\n");
      error = true;
    }

    // WR 00  03 xx xx xy 00 00 00 00 - 00 03 {x 5 nibbles device ID} {y 1 nibble on/off}
    unsigned char buf03[] = {0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    buf03[2] = inputs[0];
    buf03[3] = inputs[1];
    buf03[4] = inputs[2];
	  res = hid_write(handle, buf03, 9);
	  if (res < 0) {
		  printf("Unable to write block 03\n");
      error = true;
    }

    //WR 00  04 00 00 00 00 00 00 00 - constant
    unsigned char buf04[] = {0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	  res = hid_write(handle, buf04, 9);
	  if (res < 0) {
		  printf("Unable to write block 04\n");
      error = true;
    }

    // WR 00  05 00 00 00 00 00 00 00  - constant
    unsigned char buf05[] = {0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	  res = hid_write(handle, buf05, 9);
	  if (res < 0) {
		  printf("Unable to write block 05\n");
      error = true;
    }

    if(error) {
      break;
    }

  }

	hid_close(handle);

	hid_exit();

  if(error) {
    return -1;
  } else {
	  return 0;
  }
}
