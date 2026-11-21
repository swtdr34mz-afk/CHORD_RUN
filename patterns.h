/* SPDX-License-Identifier: GPL-3.0-only
 * Copyright (C) 2026 Obsimo (OSR Records) */
/* OSR-1: drum patterns, bass sounds and motifs, the screen's cast. Written by hand (data.h is generated). */
#pragma once

/* 16 steps: X accent, x soft, . rest; hi-hat row: o open, 2 / 3 / 4 a roll of that many hits in the step.
 * Ten current styles, each with its own kit (CHORD RUN). */
#define NDRUM 10
static const char *const DRUM_NAME[NDRUM] = {"HOUSE", "POP", "TRAP", "PHONK", "DRILL", "AFROBEAT", "REGGAETON", "LO-FI",
                                             "DREAMCORE", "ANGELCORE"};
static const struct { const char *k, *h, *s, *r; } DRUM_DEF[NDRUM] = {
    /* kick                hi-hat              snare / clap        perc */
    {"X...X...X...X...", "..o...o...o...o.", "....X.......X...", "...x......x...x."},   /* house: four on the floor, open hats, clap */
    {"X.......X.x.....", "x.x.x.x.x.x.x.x.", "....X.......X...", "................"},   /* pop */
    {"X......X..X.....", "x.x.x.x3x.x.x.4.", "........X.......", "................"},   /* trap: half time, 808, rolls */
    {"X.....X...X..X..", "x.x.x.x.x.x.x.x.", "....X.......X...", "x.xx..x.x.xx..x."},   /* phonk: cowbell */
    {"X..X......X..X..", "x.x3x.x.x.x3x.x.", "........X.....x.", "................"},   /* drill */
    {"X......X..X.....", "x.xxx.xxx.xxx.xx", "...x..x....x..x.", "X..X..X...X.X..."},   /* afrobeat: shaker, clave */
    {"X...X...X...X...", "x.x.x.x.x.x.x.x.", "...X..X....X..X.", "................"},   /* reggaeton: dembow */
    {"X......X..X.....", "x.x.x.x.x.x.x.x.", "....X.......X...", "................"},   /* lo-fi: soft, dusty */
    {"X.........X.....", "..o.......o.....", "........X.......", "......x.......x."},   /* dreamcore: slow, washed, a chime */
    {"X.X.......XX....", "x.x.x.x.x.x.x.x.", "....X..x.x..X..x", ".......x.......x"},   /* angelcore: a soft break, airy */
};

/* each pattern's kit: kick (base Hz, sweep Hz, pitch drop s, body s, drive, hold s, click), hi-hat (tone, closed s, open s,
 * metal), perc (Hz, Hz, decay s, click), snare (body Hz, body 0..1, decay s, bright 0..1, clap 0 / 1), space (reverb) */
static const kit_t DRUM_KIT[NDRUM] = {
    /* house: a punchy 909-ish kick, bright metal hats, a wide clap */
    {52.0f, 180.0f, 0.025f, 0.18f, 2.0f, 0.020f, 0.50f, 0.70f, 0.030f, 0.22f, 0.70f, 1700.0f, 500.0f, 0.010f, 0.30f, 900.0f, 0.00f, 0.20f, 0.45f, 1.0f, 0.18f},
    /* pop: a tight kick with a click, a snare with body */
    {55.0f, 160.0f, 0.020f, 0.12f, 1.8f, 0.015f, 0.60f, 0.60f, 0.025f, 0.12f, 0.40f, 1700.0f, 500.0f, 0.010f, 0.30f, 190.0f, 0.50f, 0.16f, 0.60f, 0.0f, 0.15f},
    /* trap: a long 808, crisp hats (rolls), a clap */
    {45.0f, 90.0f, 0.040f, 0.70f, 1.6f, 0.120f, 0.35f, 0.85f, 0.018f, 0.09f, 0.80f, 1700.0f, 500.0f, 0.010f, 0.30f, 900.0f, 0.00f, 0.20f, 0.55f, 1.0f, 0.12f},
    /* phonk: a distorted 808, the 808 cowbell, a hard clap */
    {44.0f, 120.0f, 0.030f, 0.50f, 4.0f, 0.080f, 0.50f, 0.70f, 0.020f, 0.08f, 0.60f, 540.0f, 800.0f, 0.120f, 0.00f, 200.0f, 0.30f, 0.14f, 0.70f, 1.0f, 0.10f},
    /* drill: a deep sliding-style 808, fast metal hats, a crisp snare */
    {42.0f, 70.0f, 0.050f, 0.60f, 1.8f, 0.100f, 0.30f, 0.90f, 0.015f, 0.07f, 0.90f, 1700.0f, 500.0f, 0.010f, 0.30f, 220.0f, 0.40f, 0.14f, 0.80f, 0.0f, 0.10f},
    /* afrobeat: a round kick, a shaker (noise hats), the clave, a rimshot */
    {56.0f, 120.0f, 0.020f, 0.14f, 1.4f, 0.015f, 0.40f, 0.60f, 0.030f, 0.10f, 0.20f, 2500.0f, 1250.0f, 0.030f, 0.10f, 330.0f, 0.60f, 0.07f, 0.70f, 0.0f, 0.12f},
    /* reggaeton: a thick kick, a snappy dembow snare */
    {54.0f, 150.0f, 0.020f, 0.16f, 2.2f, 0.020f, 0.50f, 0.60f, 0.020f, 0.08f, 0.50f, 1700.0f, 500.0f, 0.010f, 0.30f, 200.0f, 0.45f, 0.13f, 0.65f, 0.0f, 0.12f},
    /* lo-fi: soft and dusty, dark hats, a warm snare */
    {50.0f, 80.0f, 0.030f, 0.20f, 1.2f, 0.010f, 0.10f, 0.25f, 0.030f, 0.14f, 0.15f, 1700.0f, 500.0f, 0.010f, 0.30f, 180.0f, 0.55f, 0.18f, 0.25f, 0.0f, 0.30f},
    /* dreamcore: a deep soft kick, washed open hats, a big snare far away, a chime */
    {46.0f, 70.0f, 0.035f, 0.35f, 1.1f, 0.020f, 0.05f, 0.35f, 0.040f, 0.35f, 0.30f, 1760.0f, 2640.0f, 0.300f, 0.00f, 160.0f, 0.50f, 0.40f, 0.30f, 0.0f, 0.65f},
    /* angelcore: a light break, airy hats, a chime */
    {54.0f, 140.0f, 0.020f, 0.14f, 1.6f, 0.010f, 0.40f, 0.85f, 0.020f, 0.14f, 0.60f, 2093.0f, 3136.0f, 0.350f, 0.00f, 210.0f, 0.45f, 0.20f, 0.65f, 0.0f, 0.45f},
};

static const char *const BASS_NAME[NBASS_SOUND] = {"OFF", "SUB", "ROUND", "ACID", "REESE", "PLUCK", "FM"};
static const char *const MOTIF_NAME[NMOTIF] = {"FOLLOW", "HOLD", "OFFBEAT", "OCTAVES", "HALVES", "GALLOP",
                                               "SYNCOPE", "ROLLING"};

#define NCREATURE 8
static const char *const CREATURE_NAME[NCREATURE] = {"PUFFER", "SUNFISH", "JELLY", "STARFISH", "OCTOPUS", "SEAHORSE",
                                                     "MANTA", "CRAB"};
#define NEYE 6
static const char *const EYE_NAME[NEYE] = {"ROUND", "HEART", "STAR", "LASER", "SPIRAL", "CROSS"};
#define NDECOR 8
static const char *const DECOR_NAME[NDECOR] = {"BLUE", "MOIRE", "EMBERS", "ACID", "ABYSS", "CORAL", "AURORA",
                                               "GREEN CRT"};
/* the eyes' light, per background */
static const float DECOR_EYE[NDECOR][3] = {{0.85f, 0.95f, 1.0f}, {1.0f, 0.35f, 0.75f}, {1.0f, 0.8f, 0.3f},
                                           {0.4f, 1.0f, 0.95f}, {0.45f, 0.9f, 1.0f}, {1.0f, 0.95f, 0.7f},
                                           {0.7f, 1.0f, 0.6f}, {0.5f, 1.0f, 0.45f}};
#define NACC 6
static const char *const ACC_NAME[NACC] = {"NONE", "PARTY HAT", "CROWN", "HEADPHONES", "BOW", "ANTENNA"};
static const char *const ONOFF_NAME[2] = {"OFF", "ON"};
