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
#define NGRID 19
/* chord SETS: the chords of a song, one per BLACK key from the left (up to 8 of them); the white keys play a melody in the
 * scale of the chord that sounds. A chord is its semitones above the set's key + a quality (M maj, V dominant-major, m, D dom7,
 * d dim, s sus4, S sus2, a aug); key 0 = C .. 11 = B is the song's own key, rich the richness it starts with. */
static const struct { const char *name, *by; uint8_t key, rich; const char *white; } GRID_DEF[NGRID] = {
    {"LET IT BE", "THE BEATLES", 0, 0, "0M 7M 9m 5M"},
    {"STAIRWAY TO HEAVEN", "LED ZEPPELIN", 9, 0, "0m 3M 5M 8M 10M 7M"},
    {"TAKE ON ME", "A HA", 9, 0, "0M 9m 5M 7M 2m"},
    {"AFRICA", "TOTO", 9, 0, "9m 5M 0M 7M"},
    {"BABY ONE MORE TIME", "BRITNEY SPEARS", 0, 0, "0m 7M 3M 5m 7s"},
    {"ROLLING IN THE DEEP", "ADELE", 0, 0, "0m 10M 8M 7m"},
    {"HELLO", "ADELE", 5, 0, "0m 3M 10M 5M"},
    {"SHAPE OF YOU", "ED SHEERAN", 1, 0, "0m 5m 8M 10M"},
    {"BLINDING LIGHTS", "THE WEEKND", 5, 0, "0m 7m 10M 5M"},
    {"SOMEONE LIKE YOU", "ADELE", 9, 0, "0M 7M 9m 5M"},
    {"CREEP", "RADIOHEAD", 7, 0, "0M 4M 5M 5m"},
    {"FALLING BASS", "SOMEONE LIKE YOU STYLE", 0, 0, "0M 7M 9m 5M"},
    {"SHALLOW", "LADY GAGA", 7, 0, "2m 7M 0M 9m 5M"},
    {"ALWAYS REMEMBER US", "LADY GAGA", 0, 0, "5M 0M 9m 7M"},
    {"BETTER NOW", "POST MALONE", 10, 0, "2m 7M 0M 5M"},
    {"SAY MY NAME", "DESTINYS CHILD", 0, 0, "0m 8M 5m 10M"},
    {"THE BUSINESS", "TIESTO", 8, 0, "0m 10M 8M"},
    {"WENN DAS LIEBE IST", "NINA CHUBA", 5, 0, "0m 10M 8M 5m 7M 5M"},
    {"PERFECT", "ED SHEERAN", 8, 0, "0M 9m 5M 7M"},
};
