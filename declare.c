float tmp[_SIZE];
#pragma acc declare device_resident(tmp)
float beta[2] = {42.0f, 84.0f};
#pragma acc declare device_resident(beta)
