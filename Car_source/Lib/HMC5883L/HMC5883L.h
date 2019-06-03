#ifndef HMC5883L_H_
#define HMC5883L_H_

#define HMC5883L_writeAddres  0x3C
#define HMC5883L_readAddres  0x3D

void HMC5883L_Init();
void HMC5883L_Read(float *);

#endif

