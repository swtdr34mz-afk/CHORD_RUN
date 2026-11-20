/* SPDX-License-Identifier: GPL-3.0-only
 * Copyright (C) 2026 Obsimo (OSR Records) */
/* OSR-1 data: generated from the browser prototype (gen_data.mjs), then edited by hand (keep this if you regenerate):
 * the names translated to English, 2-STEP moved first in the rhythms and the sounds (families, SOUNDS)
 * replaced by 13 sounds after DRIFT's factory presets (Leo Kuroshita, Hügelton Instruments, MIT; see CREDITS.md).
 * Do not regenerate with gen_data.mjs: it would bring the old sounds back and drop NSOUND, FAMILY_OF and the cut column. */
#pragma once
static const char *const FAMILY_NAME[2] = {"AMBIENT", "SOFT KEYS"};
#define NFAMILY 2
#define NSOUND 13
static const uint8_t FAMILY_OF[NSOUND] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1};   /* the family of each sound */
static const char *const SOUND_NAME[NSOUND] = {
    "PAPER STRINGS",
    "STILL WATER",
    "SOFT HORIZON",
    "VELVET TAPE",
    "AIR AND SALT",
    "LOW CLOUD",
    "AIRFIELD",
    "SLOW BLOOM",
    "GLASS TIDE",
    "DUST PLUCK",
    "MOON KEYS",
    "SINE DROPS",
    "NIGHT SIGNAL"
};
/* the last number of a sound is its own cutoff in Hz (the CUTOFF knob at 50 %); the knob moves it up and down */
static const sound_t SOUNDS[NSOUND] = {
    {ENGINE_SUB, 0, 0, 20.0f, 0.0f, 0.8955f, 1.0f, 1.0f, 0.4f, 0.2f, 0.0f, 0.6f, 0.996f, 0.1386f, 0.3446f, 0.8661f, 0.8539f, 0.0574f, 0.3128f, 0.3969f, 3.0f, 0.3f, 0.0f, 0.45f, 0.8f, 1119.0f},   /* PAPER STRINGS */
    {ENGINE_SUB, 0, 0, 14.0f, 0.0f, 1.0159f, 1.0f, 1.0f, 0.4f, 0.2f, 0.0f, 0.6f, 0.996f, 0.4423f, 0.4952f, 0.8661f, 1.7636f, 0.0765f, 0.4496f, 0.3189f, 3.0f, 0.3f, 0.0f, 0.45f, 0.8f, 501.0f},   /* STILL WATER */
    {ENGINE_SUB, 1, 1, 8.0f, 0.0f, 0.6494f, 1.0f, 1.0f, 0.4f, 0.2f, 0.0f, 0.6f, 0.996f, 0.2303f, 0.4952f, 0.9055f, 1.2272f, 0.081f, 0.4496f, 0.3685f, 3.0f, 0.3f, 0.0f, 0.45f, 0.65f, 1343.0f},   /* SOFT HORIZON */
    {ENGINE_SUB, 0, 0, 11.0f, 0.0f, 0.7887f, 1.0f, 1.0f, 0.4f, 0.2f, 0.0f, 0.6f, 0.996f, 0.3825f, 0.5725f, 0.8661f, 1.8962f, 0.0765f, 0.5197f, 0.1843f, 3.0f, 0.3f, 0.0f, 0.45f, 0.8f, 1001.0f},   /* VELVET TAPE */
    {ENGINE_SUB, 0, 0, 4.0f, 0.0f, 0.6933f, 1.0f, 1.0f, 0.4f, 0.2f, 0.0f, 0.6f, 0.996f, 0.149f, 1.2712f, 0.6929f, 2.5344f, 0.3071f, 1.1541f, 0.1276f, 3.0f, 0.3f, 0.0f, 0.45f, 0.8f, 1623.0f},   /* AIR AND SALT */
    {ENGINE_SUB, 0, 1, 4.0f, -12.0f, 0.6494f, 1.0f, 1.0f, 0.4f, 0.2f, 0.0f, 0.6f, 0.996f, 0.0776f, 1.0227f, 0.7874f, 1.2272f, 0.1215f, 0.9285f, 0.2126f, 3.0f, 0.3f, 0.0f, 0.45f, 0.8f, 231.0f},   /* LOW CLOUD */
    {ENGINE_SUB, 1, 1, 16.0f, 0.0f, 0.5301f, 1.0f, 1.0f, 0.4f, 0.2f, 0.0f, 0.6f, 0.996f, 1.4113f, 1.0227f, 0.8268f, 3.6421f, 0.198f, 0.9285f, 0.3543f, 3.0f, 0.3f, 0.0504f, 0.45f, 0.65f, 461.0f},   /* AIRFIELD */
    {ENGINE_SUB, 0, 0, 23.0f, 0.0f, 1.0159f, 1.0f, 1.0f, 0.4f, 0.2f, 0.0f, 0.6f, 0.996f, 2.9145f, 1.0227f, 0.9291f, 4.5273f, 0.0911f, 0.9285f, 0.4252f, 3.0f, 0.3f, 0.0126f, 0.45f, 0.8f, 898.0f},   /* SLOW BLOOM */
    {ENGINE_SUB, 3, 3, 7.0f, 0.0f, 1.0159f, 1.0f, 1.0f, 0.4f, 0.2f, 0.0f, 0.6f, 0.996f, 0.0012f, 0.4952f, 0.1575f, 0.8539f, 0.0f, 0.4496f, 0.1559f, 0.0f, 0.3f, 0.0f, 0.45f, 1.3f, 8840.0f},   /* GLASS TIDE */
    {ENGINE_SUB, 0, 0, 8.0f, 0.0f, 0.6494f, 1.0f, 1.0f, 0.4f, 0.2f, 0.0f, 0.6f, 0.996f, 0.001f, 0.2772f, 0.0f, 0.2153f, 3.4286f, 0.2517f, 0.1276f, 0.0f, 0.3f, 0.0f, 0.45f, 0.8f, 146.0f},   /* DUST PLUCK */
    {ENGINE_SUB, 2, 2, 5.0f, 0.0f, 0.4598f, 1.0f, 1.0f, 0.4f, 0.2f, 0.0f, 0.6f, 0.996f, 0.0013f, 0.3446f, 0.2756f, 0.4446f, 0.6209f, 0.3128f, 0.1984f, 0.0f, 0.3f, 0.0f, 0.45f, 1.4f, 2740.0f},   /* MOON KEYS */
    {ENGINE_SUB, 3, 3, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.4f, 0.2f, 0.0f, 0.6f, 0.996f, 0.001f, 0.1668f, 0.0f, 0.4135f, 0.0f, 0.1515f, 0.0638f, 0.0f, 0.3f, 0.0f, 0.45f, 1.3f, 16000.0f},   /* SINE DROPS */
    {ENGINE_SUB, 0, 0, 8.0f, 0.0f, 0.6494f, 1.0f, 1.0f, 0.4f, 0.2f, 0.0f, 0.6f, 0.996f, 0.0021f, 0.2074f, 0.5906f, 0.2877f, 0.7312f, 0.1883f, 0.1701f, 0.0f, 0.3f, 0.0f, 0.45f, 0.8f, 1628.0f},   /* NIGHT SIGNAL */
};
#define NRHYTHM 16
static const char *const RHYTHM_NAME[NRHYTHM] = {"HELD", "2-STEP", "QUARTERS", "EIGHTHS", "SIXTEENTHS", "OFFBEAT", "PUMP", "TRESILLO", "TRANCE GATE", "GARAGE STAB", "SYNCOPATED", "SHUFFLE", "BOSSA", "STUTTER", "HALF-TIME", "FLOATING"};
/* 16 steps: X accent, x soft, - tie, . rest ("" = held) */
static const struct { const char *s; float gate, swing, atk; } RHYTHM_DEF[NRHYTHM] = {
    {"", 0.0f, 0.0f, 0.0f},
    {"X..x...x..X..x..", 0.45f, 0.12f, 0.0f},
    {"X---x---X---x---", 0.85f, 0.0f, 0.0f},
    {"X-x-X-x-X-x-X-x-", 0.6f, 0.0f, 0.0f},
    {"XxxxXxxxXxxxXxxx", 0.3f, 0.0f, 0.0f},
    {"..X...X...X...X.", 0.55f, 0.0f, 0.0f},
    {"X---X---X---X---", 0.95f, 0.0f, 0.09f},
    {"X--x--x-X--x--x-", 0.7f, 0.0f, 0.0f},
    {"X.XX.XX.X.XX.XX.", 0.55f, 0.0f, 0.0f},
    {"..X..x....X..x.x", 0.35f, 0.15f, 0.0f},
    {"X..x..x...x..x..", 0.4f, 0.0f, 0.0f},
    {"X.xxX.xxX.xxX.x.", 0.45f, 0.2f, 0.0f},
    {"X..x..x...x.x...", 0.55f, 0.0f, 0.0f},
    {"XxXx....X...xxxx", 0.4f, 0.0f, 0.0f},
    {"X-------x-----x-", 0.9f, 0.0f, 0.0f},
    {"X-----x-----x---", 0.8f, 0.0f, 0.0f},
};
#define NGRID 38
/* chords: semitones above the tonic + quality (M maj, V dominant-major, m, D dom7, d dim, s sus4, S sus2, a aug); 8 white keys then 5 black */
static const struct { const char *name; uint8_t rich; const char *white, *black; } GRID_DEF[NGRID] = {
    {"BRIGHT POP", 0, "0M 2m 4m 5M 7V 9m 7s 5S", "10M 5m 8M 4D 2D"},
    {"BALLAD", 1, "0M 9m 5M 7V 4m 2m 7s 0S", "10M 8M 4D 5m 3M"},
    {"PSYCH INDIE", 1, "0M 7m 5M 5m 9m 4m 2m 10M", "8M 3M 2V 6d 1M"},
    {"EUPHORIC MINOR", 1, "0m 8M 3M 10V 5m 7m 10s 0S", "7V 2d 1M 5M 8m"},
    {"DREAMY PSYCH", 1, "0M 4m 5M 5m 9m 2D 7V 10M", "8M 3M 1M 6d 11m"},
    {"WARM DORIAN", 1, "0m 5V 3M 10M 2m 7m 9d 0s", "8M 7M 1M 5D 11M"},
    {"MIXOLYDIAN", 0, "0V 10M 5M 7m 2m 9m 4d 0s", "3M 8M 5m 7M 2D"},
    {"ANDALUSIAN", 0, "0m 10V 8M 7V 5m 3M 2d 7s", "1M 7D 0M 6d 11d"},
    {"MELANCHOLY", 1, "0m 8M 3M 10V 5m 7m 10s 0s", "7V 1M 5V 8m 4M"},
    {"HOUSE PIANO", 1, "9m 5M 0M 7V 2m 4m 7s 0S", "10M 4D 8M 3M 5m"},
    {"DEEP HOUSE", 2, "0m 3M 10m 5m 8M 7m 2m 10V", "1M 6M 11M 4M 9M"},
    {"FLOATING LYDIAN", 1, "0M 2M 7M 4m 11m 9m 6d 0S", "5M 10M 8M 3M 1M"},
    {"DARK", 1, "0m 1M 3M 5m 7d 8M 10m 0s", "6M 11M 4M 9M 2M"},
    {"ENGLISH SPLEEN", 1, "0m 8M 3M 4M 9m 5M 10M 7s", "1M 6m 11M 2M 7V"},
    {"DISCO", 1, "2m 7D 0M 9m 5M 4D 0S 7s", "10M 3M 8M 1D 6d"},
    {"STRANGE SKY", 1, "0M 4M 9m 5m 0S 7s 2m 8M", "3M 10M 6M 1M 11m"},
    {"ANTHEM", 0, "0M 7V 9m 4m 5M 2m 7s 2V", "10M 8M 5m 3M 4M"},
    {"NIGHT", 2, "0m 3M 5m 7m 8M 10V 7s 0S", "11M 1M 6M 9M 4M"},
    {"INDIE REVERIE", 1, "0M 4V 9m 2V 5M 5m 7s 10M", "3M 8M 6d 11m 1M"},
    {"CHROMATIC", 1, "0M 4M 8M 3M 9M 5M 1M 7M", "10M 6M 11M 2M 0m"},
    {"MAJOR-MINOR", 1, "0M 0m 5M 5m 7V 3M 8M 10M", "1M 9m 2m 4m 6d"},
    {"SUNSHINE", 0, "0M 5M 9m 7V 2m 0S 5S 7s", "10M 3M 8M 4D 6d"},
    {"BOSSA", 2, "2m 7D 0M 9D 5M 4m 9m 7s", "3d 8D 10D 11d 0m"},
    {"MELODIC TRAP", 0, "0m 8M 10V 3M 5m 7m 2d 0S", "1M 7M 11d 6M 9d"},
    {"TRANCE EUPHORIA", 0, "0m 8M 3M 10V 5m 0s 8S 10s", "7V 2d 1M 6M 11M"},
    {"FUNK", 1, "0D 5D 7D 10D 3D 2m 0s 9m", "1D 6D 8D 11D 4D"},
    {"BLUES", 1, "0D 5D 7D 9m 2m 4m 10M 0s", "3M 8M 6d 1D 11d"},
    {"CINEMA", 1, "0m 8M 5m 3M 10V 7V 1M 0s", "4M 6m 9M 11d 2d"},
    {"AMBIENT", 3, "0M 2m 4m 5M 7s 9m 0S 5S", "10M 3M 8M 6m 1M"},
    {"SHOEGAZE", 1, "0M 5M 9m 4V 2V 7V 10M 0S", "3M 8M 1M 6M 11m"},
    {"80S DREAM", 1, "0m 10V 8M 10s 5m 3M 7m 0S", "1M 6M 11M 2d 4M"},
    {"ROMANTIC", 1, "0M 4m 9m 2m 5M 7V 9D 4D", "8M 10M 3M 6d 1M"},
    {"FLOATING MINOR", 2, "0m 3M 7m 10V 5m 2m 8M 0S", "1M 6M 4M 11M 9M"},
    {"RETRO GAME", 0, "0M 7V 9m 5M 2m 4m 10M 8M", "3M 1M 6d 11m 0m"},
    {"TENSION", 1, "0m 1M 6d 11d 3M 8M 7D 0s", "4M 9M 2d 5m 10m"},
    {"SUNSET", 2, "0M 5M 7m 10M 2m 9m 4m 0S", "3M 8M 1M 6d 11M"},
    {"UK GARAGE", 2, "2m 0M 4m 9m 5M 7D 7s 0S", "1M 3M 6d 8M 10D"},
    {"90S RAVE", 0, "0m 3M 10V 8M 5m 7V 0s 10s", "1M 6M 11M 2M 4M"},
};
