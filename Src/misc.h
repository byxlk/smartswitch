#ifndef	__MISC_H
#define	__MISC_H

#define LOGD PrintString1

void init_Watch_Dog(void);
void System_PowerDown();
bit POF_Boot_Delay(void);
void setSystemSleepFlag(bit sleep);
void Reboot_System(void);
unsigned char setMontorRunningStatus(unsigned char runStat);
void PrintSystemInfoToSerial(void);

#endif