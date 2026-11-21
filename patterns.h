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

/* each pattern's kit: kick (base Hz, sweep Hz, pitch drop s, body s, drive), hi-hat (tone, closed s, open s),
 * perc (Hz, Hz, decay s, click), snare (body Hz, body 0..1, decay s, bright 0..1, clap 0 / 1), space (reverb) */
static const kit_t DRUM_KIT[NDRUM] = {
    {55.0f, 110.0f, 0.016f, 0.140f, 1.8f, 0.75f, 0.020f, 0.16f, 1700.0f, 500.0f, 0.010f, 0.30f, 900.0f, 0.00f, 0.16f, 0.65f, 1.0f, 0.15f},   /* house */
    {52.0f, 100.0f, 0.020f, 0.120f, 1.6f, 0.60f, 0.018f, 0.10f, 1700.0f, 500.0f, 0.010f, 0.30f, 190.0f, 0.45f, 0.14f, 0.50f, 0.0f, 0.12f},   /* pop */
    {44.0f, 62.0f, 0.030f, 0.550f, 1.5f, 0.80f, 0.012f, 0.05f, 1700.0f, 500.0f, 0.010f, 0.30f, 900.0f, 0.00f, 0.18f, 0.70f, 1.0f, 0.10f},    /* trap: 808 */
    {48.0f, 90.0f, 0.025f, 0.350f, 3.0f, 0.70f, 0.014f, 0.07f, 560.0f, 845.0f, 0.090f, 0.00f, 200.0f, 0.30f, 0.12f, 0.80f, 1.0f, 0.08f},    /* phonk: cowbell, a hard 808 */
    {44.0f, 70.0f, 0.030f, 0.450f, 1.6f, 0.75f, 0.012f, 0.05f, 1700.0f, 500.0f, 0.010f, 0.30f, 230.0f, 0.35f, 0.12f, 0.75f, 0.0f, 0.10f},   /* drill */
    {55.0f, 90.0f, 0.020f, 0.100f, 1.4f, 0.55f, 0.020f, 0.12f, 2300.0f, 1150.0f, 0.030f, 0.10f, 330.0f, 0.60f, 0.08f, 0.60f, 0.0f, 0.10f},  /* afrobeat: clave, a rimshot */
    {54.0f, 95.0f, 0.018f, 0.110f, 1.7f, 0.60f, 0.016f, 0.08f, 1700.0f, 500.0f, 0.010f, 0.30f, 210.0f, 0.40f, 0.11f, 0.60f, 0.0f, 0.10f},   /* reggaeton */
    {50.0f, 70.0f, 0.020f, 0.160f, 1.1f, 0.30f, 0.020f, 0.10f, 1700.0f, 500.0f, 0.010f, 0.30f, 180.0f, 0.55f, 0.16f, 0.25f, 0.0f, 0.25f},   /* lo-fi */
    {46.0f, 55.0f, 0.030f, 0.300f, 1.0f, 0.35f, 0.030f, 0.25f, 1760.0f, 2640.0f, 0.250f, 0.00f, 160.0f, 0.50f, 0.35f, 0.30f, 0.0f, 0.60f}, /* dreamcore: a chime */
    {52.0f, 80.0f, 0.020f, 0.120f, 1.2f, 0.80f, 0.015f, 0.12f, 2093.0f, 3136.0f, 0.300f, 0.00f, 210.0f, 0.40f, 0.18f, 0.60f, 0.0f, 0.45f}, /* angelcore: a chime */
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
