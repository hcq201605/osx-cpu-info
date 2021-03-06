#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "osx-cpu-info.c"

int cpuTemperatureFlag       = 0;
int batteryTemperatureFlag   = 0;
int batteryHealthFlag        = 0;
int batteryCycleCountFlag    = 0;
int batteryTimeRemainingFlag = 0;
int batteryChargeFlag        = 0;
int fanSpeedFlag             = 0;
char *fanSpeedValue          = NULL;
int fanNumber                = 0;
char *infoString;
float bat_temp;

void usage()
{
  printf("Usage: osx-cpu [-c] [-b] [-h] [-y] [-t] [-p] [-f[n=0]]\n");
  printf("\t\t-c [C]PU\n");
  printf("\t\t-b [B]attery temperature\n");
  printf("\t\t-h Battery [H]ealth\n");
  printf("\t\t-y Battery c[Y]cle count\n");
  printf("\t\t-t Battery [T]ime remaining\n");
  printf("\t\t-p Battery charge [P]ercentage\n");
  printf("\t\t-f [F]an speed (default fan = 0)\n");
}

void debug()
{
  printf("%d\n", cpuTemperatureFlag);
  printf("%d\n", batteryTemperatureFlag);
  printf("%d\n", batteryHealthFlag);
  printf("%d\n", batteryCycleCountFlag);
  printf("%d\n", batteryTimeRemainingFlag);
  printf("%d\n", batteryChargeFlag);
  printf("%d\n", fanSpeedFlag);
}

int main(int argc, char *argv[])
{
  int c;
  infoString = ((char*) malloc(255*sizeof(char)));
  SMCOpen();

  if(argc == 1)
  {
    usage();
    return(0);
  }

  while ((c = getopt (argc, argv, "cbhytpf:")) != -1)
    switch (c)
    {
      case 'c':
        cpuTemperatureFlag = 1;
        strncat(infoString, getCPUTemperature(), strlen(getCPUTemperature()));
        break;
      case 'b':
        batteryTemperatureFlag = 1;
        strncat(infoString, getBatteryTemperature(), strlen(getBatteryTemperature()));
        break;
      case 'h':
        batteryHealthFlag = 1;
        strncat(infoString, getBatteryHealth(), strlen(getBatteryHealth()));
        break;
      case 'y':
        strncat(infoString, getBatteryCyclesCount(), strlen(getBatteryCyclesCount()));
        batteryCycleCountFlag = 1;
        break;
      case 't':
        strncat(infoString, getBatteryCyclesCount(), strlen(getBatteryTimeRemaining()));
        batteryTimeRemainingFlag = 1;
        break;
      case 'p':
        strncat(infoString, getBatteryChargePercentage(), strlen(getBatteryChargePercentage()));
        batteryChargeFlag = 1;
        break;
      case 'f':
        fanNumber = atoi(optarg);
        if(fanNumber >= 0 && fanNumber < SMCGetFanNumber(SMC_KEY_FAN_NUM)){
          strncat(infoString, getFanSpeed(fanNumber), strlen(getFanSpeed(fanNumber)));
        } else {
          printf("\nError: Fan %d not found\n", fanNumber);
          exit(1);
        }
        fanSpeedFlag = 1;
        break;
      case '?':
        usage();
        return 1;
      default:
        usage();
    }
  SMCClose();
  printf("%s", infoString);

  return 0;
}
