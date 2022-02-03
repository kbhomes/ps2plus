#include "irx_builtins.h"
#include "libps2plman.h"

#include <ps2plus_common.h>

#include <debug.h>
#include <iopcontrol.h>
#include <iopheap.h>
#include <kernel.h>
#include <loadfile.h>
#include <sbv_patches.h>
#include <sifcmd.h>
#include <sifrpc.h>
#include <stdio.h>
#include <string.h>
#include <tamtypes.h>
#include <unistd.h>

static void loadModules(void) {
  int ret;

  printf("Loading sio2man module (builtin) - (%x,%d) ", (unsigned int)sio2man_irx, size_sio2man_irx);
  SifExecModuleBuffer(sio2man_irx, size_sio2man_irx, 0, NULL, &ret);
  printf("[%d] returned\n", ret);

  printf("Loading ps2plman module (builtin) - (%x,%d) ", (unsigned int)ps2plman_irx, size_ps2plman_irx);
  SifExecModuleBuffer(ps2plman_irx, size_ps2plman_irx, 0, NULL, &ret);
  printf("[%d] returned\n", ret);
}

int main(int argc, char *argv[]) {
  int ret;

  //Reboot IOP
  SifInitRpc(0);
  while(!SifIopReset("", 0)){};
  while(!SifIopSync()){};
    
  //Initialize SIF services
  SifInitRpc(0);
  SifLoadFileInit();
  SifInitIopHeap();
  sbv_patch_enable_lmb();

  // Load modules for the program
  loadModules();
  ps2plman_init();

  uint16_t firmware_version;
  if ((ret = ps2plman_get_firmware_version(&firmware_version)) != PS2PLMAN_RET_OK) {
    printf("Error: %d\n", ret);
  } else {
    printf("Firmware version: 0x%04x\n", firmware_version);
  }

  char microcontroller_version[33];
  size_t microcontroller_version_size = sizeof(microcontroller_version) - 1;
  if ((ret = ps2plman_get_microcontroller_version(microcontroller_version, NULL, microcontroller_version_size)) != PS2PLMAN_RET_OK) {
    printf("Error: %d\n", ret);
  } else {
    printf("Microcontroller version: %s\n", microcontroller_version);
  }

  uint16_t configuration_version;
  if ((ret = ps2plman_get_configuration_version(&configuration_version)) != PS2PLMAN_RET_OK) {
    printf("Error: %d\n", ret);
  } else {
    printf("Configuration version: 0x%04x\n", configuration_version);
  }

  SleepThread();
  return 0;
}
