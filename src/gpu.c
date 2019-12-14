#include <stdio.h>
#include <stdlib.h>
#include "gpu.h"

void writeScanlineToFrameBuffer(GpuState*);
void drawFrame(GpuState*);

GpuState initializeGpu() {
    GpuState gpuState = {
        .mode=2,
        .modeClock=0,
        .line=0
    };

    return gpuState;
}

enum GpuMode {
    accessingOam = 2,
    accessingVram = 3,
    horizontalBlank = 0,
    verticalBlank = 1
};

void stepGpu(GpuState* gpuState, int elapsedClockCycles) {
    gpuState->modeClock += elapsedClockCycles;

    printf("GPU: mode=%d, modeClock=%d, line=%d\n", gpuState->mode, gpuState->modeClock, gpuState->line);

    switch (gpuState->mode) {
        case accessingOam: {
            if (gpuState->modeClock >= 80) {
                gpuState->modeClock = 0;
                gpuState->mode = accessingVram;
            }
            break;
        }
        case accessingVram: {
            if (gpuState->modeClock >= 172) {
                gpuState->modeClock = 0;
                gpuState->mode = horizontalBlank;
                writeScanlineToFrameBuffer(gpuState);
            }
            break;
        }
        case horizontalBlank: {
            if (gpuState->modeClock >= 204) {
                gpuState->modeClock = 0;
                gpuState->line++;

                if (gpuState->line == 143) {
                    gpuState->mode = verticalBlank;
                    drawFrame(gpuState);
                }
                else {
                    gpuState->mode = accessingOam;
                }
            }
            break;
        }
        case verticalBlank: {
            if (gpuState->modeClock >= 456) {
                gpuState->modeClock = 0;
                gpuState->line++;

                if (gpuState->line > 153) {
                    gpuState->mode = accessingOam;
                    gpuState->line = 0;
                }
            }
            break;
        }
    }
}

void writeScanlineToFrameBuffer(GpuState* gpuState) {
    // code for writing scaneline to framebuffer
}

void drawFrame(GpuState* gpuState) {
    // code for drawing frame
}