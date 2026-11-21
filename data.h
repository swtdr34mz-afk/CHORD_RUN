/* SPDX-License-Identifier: GPL-3.0-only
 * Copyright (C) 2026 Obsimo (OSR Records) */
/* CHORD RUN data (from OSR-1's, once generated from Blop's browser prototype, now written by hand):
 * the names translated to English, 2-STEP moved first in the rhythms and the sounds (families, SOUNDS)
 * replaced by 13 sounds after DRIFT's factory presets (Leo Kuroshita, Hügelton Instruments, MIT; see CREDITS.md).
 * CHORD RUN: 8 more sounds (plucks, keys, synths), 8 chord rhythms, the song sets with their own sounds. */
#pragma once
static const char *const FAMILY_NAME[5] = {"AMBIENT", "SOFT KEYS", "PLUCKS", "KEYS", "SYNTH"};
#define NFAMILY 5
#define NSOUND 24
static const uint8_t FAMILY_OF[NSOUND] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 3, 3, 4, 4, 4, 4, 4, 3};   /* the family of each sound */
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
    "NIGHT SIGNAL",
    "NYLON PLUCK",
    "MALLET",
    "KALIMBA",
    "E-PIANO",
    "GRAND KEYS",
    "80S BRASS",
    "SUPERSAW",
    "POCKET LEAD",
    "OB BRASS PAD",
    "OB BRASS",
    "GRAND PIANO"
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
    {ENGINE_SUB, 0, 0, 8.0f, 0.0f, 0.6494f, 1.0f, 1.0f, 0.4f, 0.2f, 0.0f, 0.6f, 0.996f, 0.001f, 0.2772f, 0.0f, 0.2153f, 3.4286f, 0.2517f, 0.1276f, 0.0f, 0.3f, 0.0f, 0.45f, 1.3f, 146.0f},   /* DUST PLUCK: louder (it was the quietest) */
    {ENGINE_SUB, 2, 2, 5.0f, 0.0f, 0.4598f, 1.0f, 1.0f, 0.4f, 0.2f, 0.0f, 0.6f, 0.996f, 0.0013f, 0.3446f, 0.2756f, 0.4446f, 0.6209f, 0.3128f, 0.1984f, 0.0f, 0.3f, 0.0f, 0.45f, 1.4f, 2740.0f},   /* MOON KEYS */
    {ENGINE_SUB, 3, 3, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.4f, 0.2f, 0.0f, 0.6f, 0.996f, 0.001f, 0.1668f, 0.0f, 0.4135f, 0.0f, 0.1515f, 0.0638f, 0.0f, 0.3f, 0.0f, 0.45f, 1.7f, 16000.0f},   /* SINE DROPS: louder */
    {ENGINE_SUB, 0, 0, 8.0f, 0.0f, 0.6494f, 1.0f, 1.0f, 0.4f, 0.2f, 0.0f, 0.6f, 0.996f, 0.0021f, 0.2074f, 0.5906f, 0.2877f, 0.7312f, 0.1883f, 0.1701f, 0.0f, 0.3f, 0.0f, 0.45f, 0.8f, 1628.0f},   /* NIGHT SIGNAL */
    /* the song sounds (CHORD RUN): a guitar, mallets, keys and synths, so the songs sound apart */
    {ENGINE_PLUCK, 0, 0, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.4f, 0.2f, 0.0f, 0.45f, 0.9965f, 0.002f, 3.0f, 0.9f, 0.35f, 0.8f, 0.3f, 0.25f, 0.0f, 0.3f, 0.0f, 0.45f, 0.9f, 1800.0f},   /* NYLON PLUCK */
    {ENGINE_FM, 3, 3, 0.0f, 0.0f, 0.0f, 4.0f, 2.2f, 0.07f, 0.0f, 0.0f, 0.6f, 0.996f, 0.001f, 1.0f, 0.0f, 0.3f, 0.0f, 0.3f, 0.1f, 0.0f, 0.3f, 0.0f, 0.45f, 0.9f, 6000.0f},   /* MALLET */
    {ENGINE_FM, 3, 3, 0.0f, 0.0f, 0.0f, 7.0f, 1.6f, 0.08f, 0.0f, 0.0f, 0.6f, 0.996f, 0.001f, 2.2f, 0.0f, 0.5f, 0.0f, 0.3f, 0.2f, 0.0f, 0.3f, 0.0f, 0.45f, 0.9f, 8000.0f},   /* KALIMBA */
    {ENGINE_FM, 3, 3, 0.0f, 0.0f, 0.0f, 1.0f, 1.6f, 0.45f, 0.35f, 0.08f, 0.6f, 0.996f, 0.002f, 4.0f, 0.25f, 0.45f, 0.0f, 0.3f, 0.55f, 0.0f, 0.3f, 0.0f, 0.45f, 0.9f, 5000.0f},   /* E-PIANO */
    {ENGINE_FM, 3, 3, 0.0f, 0.0f, 0.0f, 1.0f, 3.0f, 0.35f, 0.2f, 0.35f, 0.6f, 0.996f, 0.001f, 5.0f, 0.0f, 0.5f, 0.0f, 0.3f, 0.15f, 0.0f, 0.3f, 0.0f, 0.45f, 0.85f, 5000.0f},   /* GRAND KEYS */
    {ENGINE_SUB, 0, 0, 14.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.4f, 0.2f, 0.0f, 0.6f, 0.996f, 0.03f, 0.6f, 0.75f, 0.35f, 1.6f, 0.35f, 0.45f, 0.0f, 0.3f, 0.0f, 0.45f, 0.8f, 900.0f},   /* 80S BRASS */
    {ENGINE_SUB, 0, 0, 28.0f, 12.0f, 0.6f, 1.0f, 1.0f, 0.4f, 0.2f, 0.0f, 0.6f, 0.996f, 0.006f, 0.5f, 0.85f, 0.45f, 0.4f, 0.3f, 0.9f, 0.0f, 0.3f, 0.0f, 0.45f, 0.75f, 2600.0f},   /* SUPERSAW */
    {ENGINE_SUB, 1, 1, 6.0f, 12.0f, 0.35f, 1.0f, 1.0f, 0.4f, 0.2f, 0.0f, 0.6f, 0.996f, 0.004f, 0.3f, 0.8f, 0.2f, 0.6f, 0.2f, 0.2f, 0.0f, 0.3f, 0.0f, 0.45f, 0.7f, 2200.0f},   /* POCKET LEAD */
    /* an Oberheim-ish brass: warm detuned saws, a slow swell and lush chorus (a pad), and its short, punchy lead */
    {ENGINE_SUB, 0, 0, 9.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.4f, 0.2f, 0.0f, 0.6f, 0.996f, 0.09f, 0.8f, 0.8f, 0.7f, 1.0f, 0.45f, 0.7f, 2.0f, 0.25f, 0.0f, 0.45f, 0.8f, 1000.0f},   /* OB BRASS PAD */
    {ENGINE_SUB, 0, 1, 7.0f, 0.0f, 0.7f, 1.0f, 1.0f, 0.4f, 0.2f, 0.0f, 0.6f, 0.996f, 0.012f, 0.35f, 0.35f, 0.14f, 1.5f, 0.16f, 0.35f, 0.0f, 0.3f, 0.0f, 0.45f, 0.85f, 1250.0f},   /* OB BRASS */
    /* a sampled grand (the resource region: Salamander Grand Piano V3, Alexander Holm, CC-BY 3.0; tools/make_piano.py):
     * the samples' own decay (sustain 1), the filter open, a little brighter on a hit */
    {ENGINE_SAMPLE, 3, 3, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.4f, 0.2f, 0.0f, 0.6f, 0.996f, 0.001f, 10.0f, 1.0f, 0.35f, 0.8f, 0.6f, 0.05f, 0.0f, 0.3f, 0.0f, 0.45f, 1.0f, 6000.0f},   /* GRAND PIANO */
};
#define NRHYTHM 8
static const char *const RHYTHM_NAME[NRHYTHM] = {"HELD", "QUARTERS", "EIGHTHS", "OFFBEAT", "PUMP", "SYNCOPATED", "TRESILLO", "HALF-TIME"};
/* 16 steps: X accent, x soft, - tie, . rest ("" = held) */
static const struct { const char *s; float gate, swing, atk; } RHYTHM_DEF[NRHYTHM] = {
    {"", 0.0f, 0.0f, 0.0f},
    {"X---x---X---x---", 0.85f, 0.0f, 0.0f},
    {"X-x-X-x-X-x-X-x-", 0.6f, 0.0f, 0.0f},
    {"..X...X...X...X.", 0.55f, 0.0f, 0.0f},
    {"X---X---X---X---", 0.95f, 0.0f, 0.09f},
    {"X..x..x...x..x..", 0.4f, 0.0f, 0.0f},
    {"X--x--x-X--x--x-", 0.7f, 0.0f, 0.0f},
    {"X-------x-----x-", 0.9f, 0.0f, 0.0f},
};
#define NGRID 17
/* chord SETS: the chords of a song, one per BLACK key from the left (up to 8 of them); the white keys play a melody in the
 * scale of the chord that sounds. A chord is its semitones above the set's key + a quality (M maj, V dominant-major, m, D dom7,
 * d dim, s sus4, S sus2, a aug); key 0 = C .. 11 = B is the song's own key, rich the richness it starts with.
 * Then the song's own sound (CHORD RUN), set when the song is chosen: the chords' sound, the melody's (0 = as the chords, else
 * the sound + 1), the bass's sound and riff, the chord rhythm (taken only when a rhythm plays), the drums' style, the tempo,
 * and lock: 1 = the melody keeps the song's own scale on every chord (it does not follow the chords). */
static const struct {
    const char *name, *by;
    uint8_t key, rich;
    const char *white;
    uint8_t snd, msnd, bsnd, bmot, rhy, dpat, bpm, lock;
} GRID_DEF[NGRID] = {
    {"LET IT BE", "THE BEATLES", 0, 0, "0M 7M 9m 5M", 23, 12, 2, 0, 1, 1, 72, 0},
    {"TAKE ON ME", "A HA", 9, 0, "0M 9m 5M 7M 2m", 18, 21, 5, 3, 2, 1, 84, 0},
    {"AFRICA", "TOTO", 9, 0, "9m 5M 0M 7M", 21, 23, 2, 0, 1, 11, 93, 0},
    {"BABY ONE MORE TIME", "BRITNEY SPEARS", 0, 0, "0m 7M 3M 5m 7s", 23, 11, 6, 6, 5, 1, 93, 0},
    {"ROLLING IN THE DEEP", "ADELE", 0, 0, "0m 10M 8M 7m", 13, 18, 1, 0, 2, 1, 105, 0},
    {"HELLO", "ADELE", 5, 0, "0m 3M 10M 8M", 23, 12, 1, 1, 1, 1, 79, 0},
    {"SHAPE OF YOU", "ED SHEERAN", 1, 0, "0m 5m 8M 10M", 14, 15, 1, 6, 6, 6, 96, 0},
    {"BLINDING LIGHTS", "THE WEEKND", 5, 0, "0m 7m 10M 5M", 19, 19, 5, 7, 2, 1, 86, 1},
    {"SOMEONE LIKE YOU", "ADELE", 9, 0, "0M 7M 9m 5M", 23, 12, 1, 0, 2, 7, 70, 0},
    {"CREEP", "RADIOHEAD", 7, 0, "0M 4M 5M 5m", 13, 3, 2, 0, 2, 1, 92, 0},
    {"FALLING BASS", "SOMEONE LIKE YOU STYLE", 0, 0, "0M 7M 9m 5M", 3, 11, 2, 0, 7, 7, 80, 0},
    {"SHALLOW", "LADY GAGA", 7, 0, "2m 7M 0M 9m 5M", 13, 18, 1, 1, 2, 1, 96, 0},
    {"ALWAYS REMEMBER US", "LADY GAGA", 0, 0, "5M 0M 9m 7M", 23, 17, 2, 0, 1, 1, 70, 0},
    {"BETTER NOW", "POST MALONE", 10, 0, "2m 7M 0M 5M", 13, 13, 1, 0, 5, 10, 73, 0},
    {"SAY MY NAME", "DESTINYS CHILD", 0, 0, "0m 8M 5m 10M", 16, 11, 6, 6, 5, 10, 70, 0},
    {"THE BUSINESS", "TIESTO", 8, 0, "0m 10M 8M", 19, 21, 3, 2, 3, 0, 120, 0},
    {"PERFECT", "ED SHEERAN", 8, 0, "0M 9m 5M 7M", 13, 12, 2, 0, 6, 7, 95, 0},
};
