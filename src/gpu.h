#ifndef GPU_H
#define GPU_H

typedef struct GpuState {
    int mode;
    int modeClock;
    int line;
} GpuState;

GpuState initializeGpu();
void stepGpu(GpuState*, int);

#endif