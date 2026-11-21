/* SPDX-License-Identifier: GPL-3.0-only
 * Copyright (C) 2026 Obsimo (OSR Records) */
/* CHORD RUN by swtdr34mz, based on BLOP by Obsimo: song chords on the black keys, a melody on the white ones, a runner
 * that runs while you play, for the M-VAVE FM-1 (on fm1-base).
 * This header is the app's half of the contract with the platform (device/unity.c, host/plat_host.c). */
#pragma once
#include <stdint.h>
#include "plat.h"

#define APP_NAME "CHORD RUN"
#define APP_SPLASH_RGB RGB(255, 214, 110)
#define APP_SPLASH                      /* app_splash: the intro's first picture, not the platform's white page */
#define APP_TICK                        /* app_tick: the MIDI in, each clock timed to the ms */
#define APP_BLOCK 256u                   /* frames per audio block: 5.8 ms at 44.1 kHz */

/* the store: the app's settings, the panel calibration (the platform's) and the player's progress (metres run, missions:
 * apart from the settings, so a new settings layout never takes it away), and their flash offsets */
enum { OBJ_SETTINGS, OBJ_PANEL, OBJ_PROGRESS, OBJ_NOBJ };
#define APP_STORE_MAP {0xFC000u, 0xDC000u, 0xDE000u}

void app_init(void);
void app_render(int32_t *lr, uint32_t n, uint32_t master_q12);
void app_panic(void);
void app_ui_init(void);
void app_frame(void);
void app_input_only(void);
void app_say(const char *a, const char *b);
void app_splash(void);
void app_tick(void);

#define APP_HOST_EXPECT
int app_expect(const char *what, int *got);
