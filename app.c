/* SPDX-License-Identifier: GPL-3.0-only
 * Copyright (C) 2026 Obsimo (OSR Records)
 * Copyright (C) 2026 swtdr34mz (CHORD RUN: the song sets, the melody, the runner, missions, the loop) */
/* CHORD RUN (based on BLOP by Obsimo): input, pages, the song sets, the melody, the runner and its missions, the loop,
 * the screen, settings. Part of the unity build (device/unity.c, host/plat_host.c), after the platform: gfx.c and announce.c
 * are in scope. The sound is dsp/engine.c.
 *
 * Controls
 *   black keys      the song's chords (left to right, up to 8). With a rhythm the chord keeps playing (latch): the same
 *                   key again stops it; without one (HELD) it plays while the key is down
 *   white keys      the melody, in the scale of the chord that played last (its own sound, MELODY page)
 *   SELECT song (its own sounds, tempo and drums come with it)   ALGORITHM chord rhythm   PRESET the chords' sound
 *   K1..K4          the page's four settings, always shown at the bottom of the screen
 *   top row: page buttons (the same button again: back to the main page)
 *                   main  cutoff, glide, note length, drums (level; 0 = no drums)
 *                   FX    delay, tremolo, chorus, reverb
 *                   SCL   chords: key, voicing, richness, strum
 *                   ENV   groove: BPM, swing, density, sidechain on/off
 *                   LFO   bass: sound, level, riff, filter
 *                   EDIT  melody: cutoff, level, space (reverb, then echoes), preset
 *                   GLO   drums: style (each with its own kit), kick, hats, snare
 *   bottom row      HOME  back to the main page (held: the title and the player's progress)
 *                   SAVE  the music heard from outside the room
 *                   SEQ   the next character (the unlocked ones)   PLAY  hide the info
 *                   REC   the loop: ready, the first chord records 4 bars, then it plays round; REC again: close /
 *                         pause / play; held 1 s: gone
 *   OCT- / OCT+ octave.   HOME + REC held 3 s: back to the first-start settings (a countdown; the progress stays).
 *   Settings save themselves a few seconds after the last change, the progress after a few quiet seconds. */
#include "fastmath.h"
#include "engine.h"
#include "data.h"
#include "patterns.h"

/* ------------------------------------------------------------------ settings --- */
#define SET_MAGIC 0x4F535237u            /* "OSR7" (v7, CHORD RUN: 21 sounds, 8 rhythms, 10 drum styles, the song's own sound) */
typedef struct {
    uint32_t magic;
    uint8_t grid, key, rich, voicing;
    int8_t octave;
    uint8_t sound, rhythm, page;
    uint8_t cutoff, glide, gate, drums;         /* main */
    uint8_t delay, trem, chorus, reverb;        /* FX */
    uint8_t strum;                              /* chords (with key, voicing, richness) */
    uint8_t bpm, swing, dens, side;             /* groove (density: 50 = as written; side: 0 off, 1 on) */
    uint8_t dpat, dkick, dhat, drim;            /* drums */
    uint8_t bsound, blevel, bmotif, bcut;       /* bass */
    uint8_t rsv_decor[4];                       /* (Blop's aquarium decor: no longer used, kept so the layout stays) */
    uint8_t mcut, mlev, mrev, msound;           /* melody: cutoff, level, space (reverb, then echoes), preset (0 = as the chords) */
    uint8_t rsv[3];
} settings_t;
static settings_t set;
static uint8_t set_dirty;
static uint32_t dirty_at;             /* when the settings last changed (auto-save)*/

static void song_style(void);
static void settings_defaults(void)
{
    memset(&set, 0, sizeof set);
    set.magic = SET_MAGIC;
    set.cutoff = 50; set.glide = 17; set.gate = 50; set.drums = 0;      /* first start: no drums */
    set.delay = 0; set.trem = 0; set.chorus = 0; set.reverb = 22;   /* no FX, a light reverb */
    set.strum = 0;
    set.bpm = 118; set.swing = 0; set.dens = 50; set.side = 1;
    set.dpat = 0; set.dkick = 75;   /* the drums start as a plain kick */ set.dhat = 30; set.drim = 25;
    set.bsound = BASS_ROND; set.blevel = 80; set.bmotif = MOTIF_SUIT; set.bcut = 45;
    set.mcut = 50; set.mlev = 80; set.mrev = 30; set.msound = 0;
    song_style();
}

/* a song's own sound (data.h GRID_DEF): chords, melody, bass, tempo, drums; its chord rhythm only when a rhythm plays */
static void song_style(void)
{
    set.key = GRID_DEF[set.grid].key;
    set.rich = GRID_DEF[set.grid].rich;
    set.sound = GRID_DEF[set.grid].snd;
    set.msound = GRID_DEF[set.grid].msnd;
    set.bsound = GRID_DEF[set.grid].bsnd;
    set.bmotif = GRID_DEF[set.grid].bmot;
    set.dpat = GRID_DEF[set.grid].dpat;
    set.bpm = GRID_DEF[set.grid].bpm;
    if (set.rhythm) set.rhythm = GRID_DEF[set.grid].rhy;
}

static float glide_s(void) { return set.glide < 1u ? 0.0f : (float)set.glide * 0.015f; }
/* the knob, 70 Hz .. 12 kHz; the engine reads it relative to the sound's own cutoff (50 % = as the sound is made) */
static float cutoff_hz(void) { return 70.0f * fm_powf(12000.0f / 70.0f, (float)set.cutoff * 0.01f); }
static float reso_q(void) { return 1.0f; }

/* ------------------------------------------------------------------ chords --- */
typedef struct { uint8_t semi; char q; } chord_def_t;
static chord_def_t grid_w[NGRID][8];
static uint8_t grid_n[NGRID];             /* the chords in a set (4..8) */
static const char *const NOTE[12] = {"C", "C#", "D", "Eb", "E", "F", "F#", "G", "Ab", "A", "Bb", "B"};
static const char *const RICH_NAME[4] = {"TRIAD", "7TH", "9TH", "LUSH"};
static const char *const VOICING_NAME[3] = {"CLOSE", "OPEN", "WIDE"};
static const uint8_t WHITE_IDX[8] = {0, 2, 4, 5, 7, 9, 11, 12}, BLACK_IDX[5] = {1, 3, 6, 8, 10};

static int parse_chords(const char *s, chord_def_t *out, int max)
{
    int n = 0;
    while (*s && n < max) {
        int v = 0;
        while (*s == ' ') s++;
        while (*s >= '0' && *s <= '9') v = v * 10 + (*s++ - '0');
        out[n].semi = (uint8_t)v;
        out[n].q = *s ? *s++ : 'M';
        n++;
    }
    return n;
}

/* intervals for a quality at a richness; returns the count */
static int quality_ivs(char q, int rich, int8_t *iv)
{
    static const int8_t M[4][6] = {{0,4,7}, {0,4,7,11}, {0,4,7,11,14}, {0,4,7,11,14,21}};
    static const int8_t V[4][6] = {{0,4,7}, {0,4,7,10}, {0,4,7,10,14}, {0,4,7,10,14,21}};
    static const int8_t m[4][6] = {{0,3,7}, {0,3,7,10}, {0,3,7,10,14}, {0,3,7,10,14,17}};
    static const int8_t D[4][6] = {{0,4,7,10}, {0,4,7,10}, {0,4,7,10,14}, {0,4,7,10,14,21}};
    static const int8_t d[4][6] = {{0,3,6}, {0,3,6,10}, {0,3,6,10,14}, {0,3,6,10,14,17}};
    static const int8_t s4[4][6] = {{0,5,7}, {0,5,7,10}, {0,5,7,10,14}, {0,5,7,10,14,21}};
    static const int8_t s2[4][6] = {{0,2,7}, {0,2,7,10}, {0,2,7,10}, {0,2,7,10,21}};
    static const int8_t a[4][6] = {{0,4,8}, {0,4,8,11}, {0,4,8,11,14}, {0,4,8,11,14}};
    static const uint8_t NM[4] = {3, 4, 5, 6}, ND[4] = {4, 4, 5, 6}, NS2[4] = {3, 4, 4, 5}, NA[4] = {3, 4, 5, 5};
    const int8_t *src;
    int n, i;
    rich = clamp(rich, 0, 3);
    switch (q) {
    case 'V': src = V[rich]; n = NM[rich]; break;
    case 'm': src = m[rich]; n = NM[rich]; break;
    case 'D': src = D[rich]; n = ND[rich]; break;
    case 'd': src = d[rich]; n = NM[rich]; break;
    case 's': src = s4[rich]; n = NM[rich]; break;
    case 'S': src = s2[rich]; n = NS2[rich]; break;
    case 'a': src = a[rich]; n = NA[rich]; break;
    default: src = M[rich]; n = NM[rich]; break;
    }
    for (i = 0; i < n; i++) iv[i] = src[i];
    return n;
}

typedef struct { int root; int8_t iv[6]; int n; int bass_off; int oct; int shift; } chord_t;

#define KEY_C4 7                         /* key index of C4 (key 0 is F3) */
/* The keys: the BLACK keys play the set's chords (the first 8, left to right), the WHITE keys play the melody. */
static int8_t kb_black[32], kb_white[32];                      /* key -> chord slot (-1) / white-key number (-1) */
static uint8_t kb_ready;
static int last_slot = -1;                                      /* the chord the melody follows (the last one played) */
static uint32_t ld_held;                                        /* the white keys held */
static int ld_cur = -1;                                         /* the white key the melody voice plays (-1: none) */

static void kb_init(void)
{
    int k, i, nb = 0, nw = 0;
    for (k = 0; k < 32; k++) { kb_black[k] = -1; kb_white[k] = -1; }
    for (k = 0; k < NKEYS && k < 32; k++) {
        int pos = (((k - KEY_C4) % 12) + 12) % 12, blk = 0;
        for (i = 0; i < 5; i++) if (BLACK_IDX[i] == pos) blk = 1;
        if (blk) { kb_black[k] = (int8_t)(nb < 8 ? nb : -1); nb++; }
        else kb_white[k] = (int8_t)nw++;
    }
    kb_ready = 1;
}

/* a key -> the set's chord (0..7) it plays; -1 when the key is no chord key or the set has no such chord */
static int key_slot(int key, int *var, int *oct)
{
    int s;
    if (!kb_ready) kb_init();
    *var = 0;
    *oct = 0;
    if (key < 0 || key >= 32) return -1;
    s = kb_black[key];
    return s >= 0 && s < (int)grid_n[set.grid] ? s : -1;
}

/* The melody: white key number w -> a MIDI pitch. Each white key has its own note in the SONG's scale (from the song's tonic,
 * leftmost key; the same for every chord), and that note follows the chord that played last: the chord's scale is the mode of
 * that chord (by its quality) that fits the song's key best; a note that is not in it moves one semitone, to a chord tone
 * if one is there, else down. So a key keeps its note when the chord changes, or moves a semitone at most. */
static const int8_t MODES[7][7] = {{0,2,4,5,7,9,11}, {0,2,3,5,7,9,10}, {0,1,3,5,7,8,10}, {0,2,4,6,7,9,11},
                                   {0,2,4,5,7,9,10}, {0,2,3,5,7,8,10}, {0,1,3,5,6,8,10}};   /* ionian dorian phrygian lydian mixolydian aeolian locrian */
static float mel_pitch(int w)
{
    int n = grid_n[set.grid], slot = last_slot, root, minor, cand[3], nc, i, j, best = 0, bs = -1, base, deg, p, pc, nt;
    const chord_def_t *d;
    uint16_t song = 0, sc = 0, ct = 0;
    int8_t iv[6];
    if (slot < 0 || slot >= n) slot = 0;
    d = &grid_w[set.grid][slot];
    root = (set.key + d->semi) % 12;
    minor = (grid_w[set.grid][0].semi == 0 && grid_w[set.grid][0].q == 'm')            /* a minor song: it starts or ends on its minor tonic */
         || (grid_w[set.grid][n - 1].semi == 0 && grid_w[set.grid][n - 1].q == 'm');
    for (i = 0; i < 7; i++) song |= (uint16_t)(1u << ((set.key + MODES[minor ? 5 : 0][i]) % 12));
    switch (d->q) {
    case 'm': cand[0] = 1; cand[1] = 5; cand[2] = 2; nc = 3; break;
    case 'D': cand[0] = 4; nc = 1; break;
    case 'd': cand[0] = 6; nc = 1; break;
    default: cand[0] = 0; cand[1] = 3; cand[2] = 4; nc = 3; break;
    }
    for (j = 0; j < nc; j++) {
        int s2 = 0;
        for (i = 0; i < 7; i++) s2 += (song >> ((root + MODES[cand[j]][i]) % 12)) & 1;
        if (s2 > bs) { bs = s2; best = cand[j]; }
    }
    for (i = 0; i < 7; i++) sc |= (uint16_t)(1u << ((root + MODES[best][i]) % 12));
    nt = quality_ivs(d->q, 0, iv);                                       /* the chord's own notes (its triad) */
    for (i = 0; i < nt; i++) ct |= (uint16_t)(1u << ((root + iv[i]) % 12));
    base = 60 + set.key - (set.key >= 6 ? 12 : 0) + set.octave * 12;   /* the leftmost white key: the song's tonic */
    deg = w < 0 ? 0 : w;
    p = base + MODES[minor ? 5 : 0][deg % 7] + 12 * (deg / 7);
    pc = ((p % 12) + 12) % 12;
    if (!((sc >> pc) & 1u)) {
        int dn = (pc + 11) % 12, up = (pc + 1) % 12, in_dn = (sc >> dn) & 1u, in_up = (sc >> up) & 1u;
        if (in_dn && in_up) p += (((ct >> up) & 1u) && !((ct >> dn) & 1u)) ? 1 : -1;
        else p += in_up ? 1 : -1;
    }
    while (p > 108) p -= 12;
    while (p < 36) p += 12;
    return (float)p;
}

static chord_t slot_chord(int slot, int var, int oct)
{
    chord_t c;
    const chord_def_t *d;
    int rich = set.rich, i;
    c.root = 0; c.n = 0; c.bass_off = 0; c.oct = oct; c.shift = set.octave * 12;
    if (slot < 0 || slot > 7) return c;
    d = &grid_w[set.grid][slot];
    c.root = (set.key + d->semi) % 12;
    c.n = quality_ivs(d->q, rich, c.iv);
    if (var) {                                                  /* the black key: one step richer (one poorer at LUSH) */
        int8_t v[6];
        int nv = quality_ivs(d->q, rich < 3 ? rich + 1 : 2, v);
        if (nv == c.n && rich < 2) nv = quality_ivs(d->q, rich + 2, v);   /* a 7th chord's next step looks the same: go one more */
        for (i = 0; i < nv; i++) c.iv[i] = v[i];
        c.n = nv;
    }
    return c;
}

static chord_t build_chord(int key)
{
    int var, oct, slot = key_slot(key, &var, &oct);
    return slot_chord(slot, var, oct);
}

static int has(const chord_t *c, int pc)
{
    int i;
    for (i = 0; i < c->n; i++) if (c->iv[i] % 12 == pc) return 1;
    return 0;
}

static void text_cat(char *d, const char *s)
{
    while (*d) d++;
    while (*s) *d++ = *s++;
    *d = 0;
}

static void chord_name(const chord_t *c, char *b)
{
    int m = has(c, 3) && !has(c, 4), M = has(c, 4);
    int sus4 = !m && !M && has(c, 5), sus2 = !m && !M && !has(c, 5) && has(c, 2);
    int dim = m && has(c, 6) && !has(c, 7), aug = M && has(c, 8) && !has(c, 7);
    int maj7 = has(c, 11), min7 = has(c, 10), six = has(c, 9), add9 = has(c, 2) && !sus2;
    int eleven = has(c, 5) && (m || M);
    b[0] = 0;
    text_cat(b, NOTE[c->root]);
    if (dim) { text_cat(b, min7 ? "m7b5" : "dim"); return; }
    if (m) text_cat(b, "m");
    if (aug) text_cat(b, "+");
    if (maj7 || min7) {
        if (maj7) text_cat(b, "maj");
        text_cat(b, six ? "13" : eleven ? "11" : add9 ? "9" : "7");
    } else if (six && add9) text_cat(b, "6/9");
    else if (six) text_cat(b, "6");
    else if (add9) text_cat(b, "add9");
    if (sus4) text_cat(b, "sus4");
    if (sus2) text_cat(b, "sus2");
}

/* the 4-note voicing closest to the centre: the same key always sounds the same */
static void voice_chord(const chord_t *c, float *out)
{
    int pcs[6], np = 0, i, r, base, best_set = 0;
    int center = 64 + c->oct * 7 + c->shift;
    float best = 1e9f;
    for (i = 0; i < c->n; i++) {
        int p = (c->root + c->iv[i]) % 12, k, dup = 0;
        for (k = 0; k < np; k++) if (pcs[k] == p) dup = 1;
        if (!dup) pcs[np++] = p;
    }
    if (np > 4) {                        /* drop the fifth, then the root (the bass plays it) */
        int k, j = 0;
        for (k = 0; k < np; k++) if (pcs[k] != (c->root + 7) % 12) pcs[j++] = pcs[k];
        np = j;
    }
    if (np > 4) {
        int k, j = 0;
        for (k = 0; k < np; k++) if (pcs[k] != c->root) pcs[j++] = pcs[k];
        np = j;
    }
    if (np > 4) np = 4;
    for (r = 0; r < np; r++) {
        for (base = 50 + c->shift; base <= 72 + c->shift; base++) {
            int notes[4], k, cur = base, cnt = 0;
            float mean = 0.0f, cost;
            for (k = 0; k < np; k++) {
                int p = pcs[(r + k) % np];
                while (((cur % 12) + 12) % 12 != p) cur++;
                notes[cnt++] = cur++;
            }
            if (cnt == 3) notes[cnt++] = notes[0] + 12;
            if (notes[0] < 50 + c->shift || notes[cnt - 1] > 84 + c->shift) continue;
            for (k = 0; k < 4; k++) mean += (float)notes[k];
            mean *= 0.25f;
            cost = fm_fabsf(mean - (float)center);
            if (cost < best) {
                best = cost;
                for (k = 0; k < 4; k++) out[k] = (float)notes[k];
                best_set = 1;
            }
        }
    }
    if (!best_set) for (i = 0; i < 4; i++) out[i] = 60.0f + (float)i * 4.0f;
}

static void spread(float *n, int mode)   /* SERRE as is, OUVERT drop 2, LARGE drop 2 + top up an octave */
{
    int i, j;
    if (mode >= 1) n[2] -= 12.0f;
    if (mode >= 2) n[3] += 12.0f;
    for (i = 1; i < 4; i++)              /* sort */
        for (j = i; j > 0 && n[j - 1] > n[j]; j--) { float t = n[j]; n[j] = n[j - 1]; n[j - 1] = t; }
}

/* ------------------------------------------------------------------ engine links --- */
/* the melody voice (white keys): its preset (0 = the chords' sound), cutoff (50 = as the sound is made), level, and SPACE,
 * one knob for a few effects: up to 40 % a reverb that grows (a light tail), then echoes (the ping-pong delay, in time) join
 * in and the reverb grows on to big and airy */
static void send_lead(void)
{
    float hz = 70.0f * fm_powf(12000.0f / 70.0f, (float)set.mcut * 0.01f), sp = (float)set.mrev * 0.01f;
    float rev = fm_minf(1.0f, sp * 1.35f), del = sp <= 0.4f ? 0.0f : (sp - 0.4f) * (0.5f / 0.6f);
    eng_lead_sound(set.msound ? &SOUNDS[set.msound - 1] : &SOUNDS[set.sound]);
    eng_lead_params(hz, (float)set.mlev * 0.0125f, rev, del);
}
static void send_sound(void) { eng_sound(&SOUNDS[set.sound]); send_lead(); }
static void send_rhythm(void)
{
    rhythm_t r;
    int i, def = set.rhythm;
    const char *s = RHYTHM_DEF[def].s;
    memset(&r, 0, sizeof r);
    r.hold = s[0] == 0;
    r.gate = RHYTHM_DEF[def].gate;
    r.swing = RHYTHM_DEF[def].swing;
    r.atk = RHYTHM_DEF[def].atk;
    if (!r.hold)
        for (i = 0; i < 16; i++) {
            if (s[i] == 'X' || s[i] == 'x') {
                int len = 1;
                while (i + len < 16 && s[i + len] == '-') len++;
                r.v[i] = s[i] == 'X' ? 1.0f : 0.62f;
                r.len[i] = (uint8_t)len;
            }
        }
    eng_rhythm(&r);
}
/* ------------------------------------------------------------------ MIDI clock in (USB) --- */
/* Ableton (or any host) sending MIDI clock to Blop: its tempo replaces BPM, and its beats place the rhythm's steps.
 * The 1 kHz tick reads the MIDI in (each clock's time to the ms); the main loop takes the tempo and the beats from it.
 * Start (FA) puts the next clock on beat 1, Continue (FB) and Song Position (F2) where the host is, Stop (FC) lets
 * Blop run on alone at the last tempo, as when the clock goes quiet. */
#define CK_N 49u                                  /* 48 clocks: two beats, for the tempo */
static volatile struct {
    uint32_t t[CK_N], n;                          /* the clocks' times (ms), how many so far */
    int32_t pos;                                  /* clocks since beat 1 (24 a beat), counted while the host plays */
    uint8_t run;                                  /* the host's transport plays (Start / Continue .. Stop) */
    uint32_t beat_ms, beat_seq;                   /* the last beat's time, and a count of beats */
    int32_t beat_pos;
    uint32_t last_ms;
} ck;
static float ck_bpm;                              /* the tempo it gives (0: no clock, BPM leads) */
static uint32_t ck_seen_seq;
static uint8_t ck_placed;                         /* the engine's steps follow the beats */

void app_tick(void)
{
    uint32_t pkt, now;
    int port;
    while (plat_midi_in(&pkt, &port)) {
        uint32_t st = pkt >> 8 & 0xFFu;
        if (port != PORT_USB) continue;
        now = plat_ms();
        switch (st) {
        case 0xF8:
            ck.t[ck.n % CK_N] = now; ck.n++; ck.last_ms = now;
            if (ck.run) {
                ck.pos++;
                if (ck.pos % 24 == 0) { ck.beat_ms = now; ck.beat_pos = ck.pos; ck.beat_seq++; }
            }
            break;
        case 0xFA: ck.run = 1; ck.pos = -1; break;
        case 0xFB: ck.run = 1; break;
        case 0xFC: ck.run = 0; break;
        case 0xF2: ck.pos = (int32_t)((pkt >> 16 & 0x7Fu) | (pkt >> 24 & 0x7Fu) << 7) * 6 - 1; break;
        default: break;
        }
    }
}

static void send_params(void)
{
    eng_params(ck_bpm > 0.0f ? ck_bpm : (float)set.bpm, cutoff_hz(), reso_q(), set.side ? 0.75f : 0.0f, set.drums > 0);
}

static void clock_follow(uint32_t now)
{
    uint32_t f, n, t0, t1, seq, bms;
    int32_t bpos;
    uint8_t run;
    float bpm = 0.0f;
    f = plat_lock();
    n = ck.n; t1 = ck.t[(n + CK_N - 1u) % CK_N]; t0 = ck.t[n % CK_N];
    seq = ck.beat_seq; bms = ck.beat_ms; bpos = ck.beat_pos; run = ck.run;
    plat_unlock(f);
    if (n >= CK_N && now - t1 < 300u && t1 - t0 > 0u) bpm = 120000.0f / (float)(t1 - t0);   /* 48 clocks = 2 beats */
    if (bpm > 0.0f && (bpm < 30.0f || bpm > 300.0f)) bpm = 0.0f;
    if (bpm > 0.0f) {
        float b = ck_bpm > 0.0f ? ck_bpm + 0.25f * (bpm - ck_bpm) : bpm;
        b = (float)(int32_t)(b * 10.0f + 0.5f) * 0.1f;               /* to a tenth */
        if (ck_bpm <= 0.0f) app_say("MIDI SYNC", "TEMPO FROM USB");
        if (b != ck_bpm) { ck_bpm = b; send_params(); }
        if (seq != ck_seen_seq) {
            ck_seen_seq = seq;
            if (run) { eng_sync((int)((bpos / 6) & 15), bms); ck_placed = 1; }
        }
        if (!run && ck_placed) { eng_sync(-1, 0); ck_placed = 0; }   /* the host stopped: free again, its tempo kept */
    } else if (ck_bpm > 0.0f) {
        ck_bpm = 0.0f;
        eng_sync(-1, 0);
        ck_placed = 0;
        send_params();
        app_say("MIDI SYNC", "OFF");
    }
}
static void send_fx(void)
{
    eng_fx((float)set.reverb * 0.01f, (float)set.delay * 0.01f, (float)set.trem * 0.01f, (float)set.chorus * 0.01f);
}
static void send_groove(void)
{
    eng_groove((float)set.swing * 0.01f, ((float)set.dens - 50.0f) * 0.02f, (float)set.gate * 0.01f,
               (float)set.strum * 0.01f);
}
static float drum_v(char c) { return c == 'X' ? 1.0f : (c == 'x' || c == 'o' || (c >= '2' && c <= '4')) ? 0.6f : 0.0f; }
static void send_drums(void)
{
    drums_t d;
    int i;
    for (i = 0; i < 16; i++) {
        char hc = DRUM_DEF[set.dpat].h[i];
        d.k[i] = drum_v(DRUM_DEF[set.dpat].k[i]);
        d.h[i] = drum_v(hc);
        d.s[i] = drum_v(DRUM_DEF[set.dpat].s[i]);
        d.r[i] = drum_v(DRUM_DEF[set.dpat].r[i]);
        d.open[i] = hc == 'o';
        d.roll[i] = (uint8_t)(hc >= '2' && hc <= '4' ? hc - '0' : 0);
    }
    d.kit = DRUM_KIT[set.dpat];
    {   /* the main page's DRUMS is the kit's master level */
        float m = (float)set.drums * 0.01f;
        m = m * (1.6f - 0.6f * m);
        eng_drums(&d, (float)set.dkick * 0.01f * m, (float)set.dhat * 0.01f * m, (float)set.drim * 0.01f * m);
    }
}
static void send_bass(void) { eng_bass(set.bsound, set.bmotif, (float)set.blevel * 0.01f, (float)set.bcut * 0.01f); }

/* ------------------------------------------------------------------ playing --- */
static int cur_idx = -1, held_idx = -1, active;
static char chord_txt[16] = APP_NAME;
static uint32_t hop_at;                                         /* the runner hops (a chord, a melody note) */
static void play(int idx)
{
    chord_t c = build_chord(idx);
    float notes[4], bass;
    uint32_t now = plat_ms();
    if (c.n == 0) return;                                       /* a silent key */
    voice_chord(&c, notes);
    spread(notes, set.voicing);
    bass = (float)(36 + (c.root + c.bass_off) % 12 + clamp(c.shift, -12, 12));
    chord_name(&c, chord_txt);
    cur_idx = idx;
    hop_at = now;
    { int v, o; last_slot = key_slot(idx, &v, &o); }
    active = 1;
    eng_chord(notes, bass, glide_s());
}

static void stop(void)
{
    active = 0;
    cur_idx = -1;
    eng_stop();
}

static void rechord(void)
{
    int v, o;
    if (!active || cur_idx < 0) return;
    if (key_slot(cur_idx, &v, &o) < 0) stop();                  /* the new set has no chord on this key */
    else play(cur_idx);
}

/* ------------------------------------------------------------------ pages --- */
enum { PG_MAIN, PG_FX, PG_ACC, PG_GROOVE, PG_BASS, PG_MELODY, PG_DRUMS, NPAGE };
static const char *const PAGE_NAME[NPAGE] = {"MAIN", "FX", "CHORDS", "GROOVE", "BASS", "MELODY", "DRUMS"};
static const uint8_t PAGE_BTN[NPAGE] = {0xFF, B_FX, B_SEL, B_ENV, B_LFO, B_EDIT, B_GLO};
static const uint8_t PAGE_RGB[NPAGE][3] = {{200, 255, 60}, {120, 200, 255}, {255, 190, 90}, {255, 120, 90},
                                          {110, 240, 190}, {210, 170, 255}, {255, 150, 120}};

enum { KD_PCT, KD_OFF, KD_GLIDE, KD_BPM, KD_BIP, KD_LIST };
enum { G_NONE, G_PARAMS, G_FX, G_GROOVE, G_DRUMS, G_BASS, G_CHORD, G_DRUMLVL, G_MELODY };
typedef struct {
    const char *lab;
    uint8_t *v;
    uint8_t lo, hi, kind, grp;
    const char *const *names;
} kdef_t;
static const char *msound_name[NSOUND + 1];    /* "AS CHORDS", then the sounds (app_init) */
static const kdef_t KD[NPAGE][4] = {
    {{"CUTOFF", &set.cutoff, 0, 100, KD_PCT, G_PARAMS, 0}, {"GLIDE", &set.glide, 0, 100, KD_GLIDE, G_NONE, 0},
     {"LENGTH", &set.gate, 0, 100, KD_PCT, G_GROOVE, 0}, {"DRUMS", &set.drums, 0, 100, KD_OFF, G_DRUMLVL, 0}},
    {{"DELAY", &set.delay, 0, 100, KD_OFF, G_FX, 0}, {"TREMOLO", &set.trem, 0, 100, KD_OFF, G_FX, 0},
     {"CHORUS", &set.chorus, 0, 100, KD_OFF, G_FX, 0}, {"REVERB", &set.reverb, 0, 100, KD_PCT, G_FX, 0}},
    {{"KEY", &set.key, 0, 11, KD_LIST, G_CHORD, NOTE}, {"VOICING", &set.voicing, 0, 2, KD_LIST, G_CHORD, VOICING_NAME},
     {"RICHNESS", &set.rich, 0, 3, KD_LIST, G_CHORD, RICH_NAME}, {"STRUM", &set.strum, 0, 100, KD_OFF, G_GROOVE, 0}},
    {{"BPM", &set.bpm, 70, 160, KD_BPM, G_PARAMS, 0}, {"SWING", &set.swing, 0, 100, KD_OFF, G_GROOVE, 0},
     {"DENSITY", &set.dens, 0, 100, KD_BIP, G_GROOVE, 0}, {"SIDECHAIN", &set.side, 0, 1, KD_LIST, G_PARAMS, ONOFF_NAME}},
    {{"SOUND", &set.bsound, 0, NBASS_SOUND - 1, KD_LIST, G_BASS, BASS_NAME}, {"LEVEL", &set.blevel, 0, 100, KD_PCT, G_BASS, 0},
     {"RIFF", &set.bmotif, 0, NMOTIF - 1, KD_LIST, G_BASS, MOTIF_NAME}, {"FILTER", &set.bcut, 0, 100, KD_PCT, G_BASS, 0}},
    {{"CUTOFF", &set.mcut, 0, 100, KD_PCT, G_MELODY, 0}, {"LEVEL", &set.mlev, 0, 100, KD_PCT, G_MELODY, 0},
     {"SPACE", &set.mrev, 0, 100, KD_OFF, G_MELODY, 0}, {"PRESET", &set.msound, 0, NSOUND, KD_LIST, G_MELODY, msound_name}},
    {{"STYLE", &set.dpat, 0, NDRUM - 1, KD_LIST, G_DRUMS, DRUM_NAME}, {"KICK", &set.dkick, 0, 100, KD_OFF, G_DRUMS, 0},
     {"HATS", &set.dhat, 0, 100, KD_OFF, G_DRUMS, 0}, {"SNARE", &set.drim, 0, 100, KD_OFF, G_DRUMS, 0}},
};

static int settings_ok(void)
{
    int p, k;
    if (set.magic != SET_MAGIC || set.grid >= NGRID || set.octave < -2 || set.octave > 2 || set.sound >= NSOUND
        || set.rhythm >= NRHYTHM || set.page >= NPAGE || set.rsv[0] >= 10)
        return 0;
    for (p = 0; p < NPAGE; p++)
        for (k = 0; k < 4; k++)
            if (*KD[p][k].v < KD[p][k].lo || *KD[p][k].v > KD[p][k].hi) return 0;
    return 1;
}

/* ------------------------------------------------------------------ platform contract --- */
static void game_init(void);
void app_init(void)
{
    int g;
    msound_name[0] = "AS CHORDS";
    for (g = 0; g < NSOUND; g++) msound_name[g + 1] = SOUND_NAME[g];
    for (g = 0; g < NGRID; g++) {
        grid_n[g] = (uint8_t)parse_chords(GRID_DEF[g].white, grid_w[g], 8);
    }
    if (plat_store_load(OBJ_SETTINGS, &set, sizeof set) != (int)sizeof set || !settings_ok())
        settings_defaults();
    game_init();
    eng_init();
    send_sound();
    send_rhythm();
    send_groove();
    send_drums();
    send_bass();
    send_fx();
    send_params();
}

void app_render(int32_t *lr, uint32_t n, uint32_t master_q12)
{
    eng_render(lr, n, (float)master_q12 * (1.0f / 4096.0f) * 8300000.0f);
}

void app_panic(void) { eng_panic(); }

/* ------------------------------------------------------------------ UI --- */
static struct {
    char msg[2][24];
    uint32_t msg_until;
    uint32_t keys, btn;
    uint32_t frame_at, home_at;
    uint8_t help, help_drawn, home_long;
    uint32_t reset_at;                   /* HOME + REC held since (0: not) */
    uint8_t reset_shown, reset_done;
    uint8_t bowl, clean;                 /* the music heard from outside the room (SAVE), no info on the screen (PLAY) */
} ui;
static void fun(int k);                  /* the bottom row (screen part) */

static void int_text(int32_t v, char *b)
{
    char t[12];
    int n = 0;
    uint32_t u = (uint32_t)(v < 0 ? -v : v);
    do t[n++] = (char)('0' + u % 10u); while ((u /= 10u) && n < 11);
    if (v < 0) *b++ = '-';
    while (n) *b++ = t[--n];
    *b = 0;
}

void app_say(const char *a, const char *b)
{
    uint32_t i;
    char t[48];
    for (i = 0; i < 23u && a && a[i]; i++) ui.msg[0][i] = a[i];
    ui.msg[0][i] = 0;
    for (i = 0; i < 23u && b && b[i]; i++) ui.msg[1][i] = b[i];
    ui.msg[1][i] = 0;
    ui.msg_until = plat_ms() + 1300u;
    if (ann_on()) {
        t[0] = 0;
        text_cat(t, ui.msg[0]);
        text_cat(t, " ");
        text_cat(t, ui.msg[1]);
        ann_say(t, 0, ANN_NOW);
    }
}

static void dirty(void) { set_dirty = 1; dirty_at = plat_ms(); }

static void wrap_u8(uint8_t *v, int32_t d, int n) { *v = (uint8_t)(((int32_t)*v + d % n + n) % n); }

/* a quick turn moves further: x1 slow, x2, x4 when the detents come close together */
static uint32_t enc_at[NE];
static int32_t accel(int k, int32_t e)
{
    uint32_t now = plat_ms(), dt = now - enc_at[k];
    enc_at[k] = now;
    if (dt < 40u) return e * 4;
    if (dt < 90u) return e * 2;
    return e;
}

/* a knob's value as text, and as 0..1 for its bar */
static float knob_text(const kdef_t *d, char *b)
{
    int v = *d->v;
    float f = (float)(v - d->lo) / (float)(d->hi - d->lo);
    switch (d->kind) {
    case KD_LIST: b[0] = 0; text_cat(b, d->names[v]); break;
    case KD_BPM: if (ck_bpm > 0.0f) { int_text((int32_t)(ck_bpm + 0.5f), b); text_cat(b, " SYNC"); } else int_text(v, b); break;
    case KD_GLIDE:
        if (v < 1) { b[0] = 0; text_cat(b, "OFF"); }
        else { int_text(v * 15, b); text_cat(b, " MS"); }
        break;
    case KD_BIP:
        if (v == 50) { b[0] = 0; text_cat(b, "0"); }
        else { if (v > 50) { b[0] = '+'; b[1] = 0; } else b[0] = 0; int_text((v - 50) * 2, b + (v > 50)); text_cat(b, "%"); }
        break;
    case KD_OFF:
        if (v == 0) { b[0] = 0; text_cat(b, "OFF"); break; }
        /* fall through */
    default: int_text(v, b); text_cat(b, "%"); break;
    }
    return f;
}

static uint8_t prev_drums;
static void apply_group(int g)
{
    switch (g) {
    case G_PARAMS: send_params(); break;
    case G_FX: send_fx(); break;
    case G_GROOVE: send_groove(); break;
    case G_DRUMS: send_drums(); break;
    case G_BASS: send_bass(); break;
    case G_MELODY: send_lead(); break;
    case G_CHORD: rechord(); break;
    case G_DRUMLVL:
        if ((*KD[PG_MAIN][3].v > 0) != (prev_drums > 0)) send_params();
        send_drums();
        break;
    default: break;
    }
}

static void knob(int k, int32_t e)
{
    const kdef_t *d = &KD[set.page][k];
    prev_drums = set.drums;
    int32_t step = d->kind == KD_LIST ? e : d->kind == KD_BPM ? accel(EN_K1 + k, e) : accel(EN_K1 + k, e) * 3;
    int32_t n = clamp((int32_t)*d->v + step, d->lo, d->hi);
    if (n == *d->v) return;
    *d->v = (uint8_t)n;
    apply_group(d->grp);
    dirty();
}

static void encoder(int k, int32_t e)
{
    switch (k) {
    case EN_SELECT:
        wrap_u8(&set.grid, e, NGRID);
        song_style();                                           /* the song's own sound, tempo and drums */
        app_say("SET", GRID_DEF[set.grid].name);
        send_sound(); send_bass(); send_drums(); send_params();
        if (set.rhythm) send_rhythm();
        rechord();
        break;
    case EN_ALGO:
        wrap_u8(&set.rhythm, e, NRHYTHM);
        app_say("RHYTHM", set.rhythm ? RHYTHM_NAME[set.rhythm] : "OFF");
        send_rhythm();
        if (!set.rhythm && held_idx < 0 && active) stop();   /* no rhythm, no latch */
        break;
    case EN_PRESET:
        wrap_u8(&set.sound, e, NSOUND);
        app_say(FAMILY_NAME[FAMILY_OF[set.sound]], SOUND_NAME[set.sound]);
        send_sound();
        break;
    case EN_K1: case EN_K2: case EN_K3: case EN_K4: knob(k - EN_K1, e); return;
    default: return;
    }
    dirty();
}

/* ------------------------------------------------------------------ the run (CHORD RUN) --- */
/* While you play (a chord sounds, a melody note, the loop) the little runner runs: its metres add up (kept in the flash, apart
 * from the settings), a run ends after 3 s of quiet (the best one is kept), new characters unlock at so many metres, and
 * there is always a mission (one after the other, a bit harder each round) that gives metres on top. */
static const char *const CHR_NAME_B[10] = {"CUBE", "DIAMOND", "SPIKE", "ORB", "HEXAGON", "CROSS", "STAR", "ARROW", "RING", "HOURGLASS"};
static const uint32_t UNLOCK_M[10] = {0u, 0u, 250u, 700u, 1400u, 2400u, 3600u, 5200u, 7200u, 10000u};
#define PROG_MAGIC 0x31524E43u                  /* "CNR1" */
typedef struct {
    uint32_t magic;
    uint32_t metres, best;                      /* metres run in all, the longest run */
    uint32_t chords, notes;                     /* chords and melody notes played in all */
    uint32_t missions;                          /* missions done: the next one is the one after */
    uint32_t songs;                             /* a bit per song played */
    uint32_t rsv[4];
} prog_t;
static prog_t prog;
static uint8_t prog_dirty;

enum { MS_CHORDS, MS_NOTES, MS_RUN, MS_SONGS, MS_ALL, MS_DRUMS, MS_LOOP, MS_TOTAL, NMS };
static const struct { const char *a, *b; uint16_t base, step, cap; } MISSION[NMS] = {
    {"PLAY ", " CHORDS", 24, 12, 96},
    {"PLAY ", " NOTES", 40, 20, 160},
    {"RUN ", " M AT ONCE", 120, 60, 600},
    {"PLAY ", " SONGS", 3, 1, 8},
    {"ALL CHORDS OF A SONG", "", 0, 0, 0},
    {"JAM ", " S WITH DRUMS", 45, 15, 180},
    {"RECORD A LOOP (REC)", "", 1, 0, 1},
    {"RUN ", " M", 400, 200, 2000},
};
static const uint8_t MS_ORDER[NMS] = {MS_CHORDS, MS_NOTES, MS_RUN, MS_SONGS, MS_ALL, MS_DRUMS, MS_LOOP, MS_TOTAL};

static struct {
    float speed, run_m, carry, scroll;          /* m/s now, this run's metres, a part metre not yet counted, the ground's scroll (px) */
    uint32_t busy_at, note_at, party_at;        /* last played, last melody note, a celebration */
    uint8_t running;                            /* a run is under way */
    uint8_t mtype, mgrid, mslots;               /* the mission, and for EVERY CHORD: the song, the chords played in it */
    uint16_t mtarget;
    float mprog;
    uint32_t mseen;                             /* songs played in this mission (a bit each) */
} run;
static struct { char a[24], b[24]; uint32_t at, until; } toast;

static void show_toast(const char *a, const char *b)
{
    toast.a[0] = 0; text_cat(toast.a, a);
    toast.b[0] = 0; text_cat(toast.b, b);
    toast.at = plat_ms(); toast.until = toast.at + 2600u;
    run.party_at = toast.at;
    app_say(a, b);
}

static int chars_unlocked(void)
{
    int i, n = 0;
    for (i = 0; i < 10; i++) if (prog.metres >= UNLOCK_M[i]) n = i + 1;
    return n;
}

static int grid_full_mask(void) { return (1 << grid_n[set.grid]) - 1; }
static int popcount8(uint32_t v) { int n = 0; while (v) { n += (int)(v & 1u); v >>= 1; } return n; }

static void mission_start(void)
{
    uint32_t k = prog.missions;
    int round = (int)(k / NMS), t;
    run.mtype = MS_ORDER[k % NMS];
    t = MISSION[run.mtype].base + MISSION[run.mtype].step * round;
    if (t > MISSION[run.mtype].cap && MISSION[run.mtype].cap) t = MISSION[run.mtype].cap;
    run.mtarget = (uint16_t)t;
    run.mprog = 0.0f;
    run.mseen = 0;
    run.mgrid = set.grid; run.mslots = 0;
}

/* the mission as text: "PLAY 24 CHORDS", and its progress "5/24" */
static void mission_text(char *b, char *p)
{
    char n[12];
    b[0] = 0; p[0] = 0;
    text_cat(b, MISSION[run.mtype].a);
    if (MISSION[run.mtype].b[0]) { int_text(run.mtarget, n); text_cat(b, n); text_cat(b, MISSION[run.mtype].b); }
    if (run.mtype == MS_ALL) {
        int_text(popcount8(run.mslots), p); text_cat(p, "/"); int_text(grid_n[set.grid], n); text_cat(p, n);
    } else if (run.mtype != MS_LOOP) {
        int_text((int32_t)run.mprog, p); text_cat(p, "/"); int_text(run.mtarget, n); text_cat(p, n);
    }
}

static void add_metres(uint32_t m)
{
    int before = chars_unlocked(), after;
    prog.metres += m;
    prog_dirty = 1;
    after = chars_unlocked();
    if (after > before) {
        char t[24];
        t[0] = 0; text_cat(t, CHR_NAME_B[after - 1]); text_cat(t, " - PRESS SEQ");
        show_toast("NEW CHARACTER", t);
        eng_blip(BLIP_TOC);
    }
}

static void mission_check(void)
{
    int done = run.mtype == MS_ALL ? run.mslots != 0 && run.mslots == grid_full_mask() : run.mprog >= (float)run.mtarget;
    if (done) {
        uint32_t bonus = 150u + 50u * (prog.missions / NMS);
        char t[16], n[12];
        prog.missions++;
        prog_dirty = 1;
        int_text((int32_t)bonus, n);
        t[0] = 0; text_cat(t, "+"); text_cat(t, n); text_cat(t, " M");
        show_toast("MISSION DONE", t);
        eng_blip(BLIP_PLOP);
        mission_start();
        add_metres(bonus);
    }
}

/* the player played a chord (key) or stopped it (-1), a melody note */
static void game_chord(int key)
{
    int v, o, slot;
    uint32_t now = plat_ms();
    run.busy_at = now;
    if (key < 0) return;
    prog.chords++;
    prog.songs |= 1u << set.grid;
    if (run.mtype == MS_CHORDS) run.mprog += 1.0f;
    if (run.mtype == MS_SONGS && !(run.mseen & (1u << set.grid))) { run.mseen |= 1u << set.grid; run.mprog += 1.0f; }
    if (run.mtype == MS_ALL) {
        if (run.mgrid != set.grid) { run.mgrid = set.grid; run.mslots = 0; }
        slot = key_slot(key, &v, &o);
        if (slot >= 0) run.mslots |= (uint8_t)(1u << slot);
    }
    mission_check();
}
static void game_note(void)
{
    run.note_at = run.busy_at = plat_ms();
    prog.notes++;
    if (run.mtype == MS_NOTES) { run.mprog += 1.0f; mission_check(); }
}

/* ------------------------------------------------------------------ the loop (REC) --- */
/* REC: ready; the first chord starts the recording, 4 bars (of the tempo), then it plays round and round (the chord changes,
 * as played), and the melody is yours to play over it. REC again: while recording, the loop closes at the bar; while playing,
 * a pause; paused, it plays again from its start. REC held 1 s: the loop is gone. */
enum { LP_OFF, LP_READY, LP_REC, LP_PLAY, LP_PAUSE };
#define LP_MAX 96
#define LP_BARS 4
static struct {
    uint8_t st, firing, held_done;
    uint16_t n, len;                            /* events, the loop's length in steps (16 a bar) */
    struct { uint16_t step; int8_t key; } ev[LP_MAX];
    uint32_t t0, rec_down;                      /* the recording's / the round's start (ms), REC pressed at */
    int32_t last;                               /* the last step played this round */
} lp;

static float step_ms(void) { return 15000.0f / (ck_bpm > 0.0f ? ck_bpm : (float)set.bpm); }

static void lp_note(int key)
{
    uint32_t now = plat_ms();
    if (lp.st == LP_READY && key >= 0) {
        lp.st = LP_REC; lp.t0 = now; lp.n = 0; lp.len = LP_BARS * 16;
        app_say("RECORDING", "4 BARS");
    }
    if (lp.st == LP_REC && lp.n < LP_MAX) {
        int32_t st = (int32_t)((float)(now - lp.t0) / step_ms() + 0.5f);
        if (key < 0 && lp.n && lp.ev[lp.n - 1].key >= 0 && st <= (int32_t)lp.ev[lp.n - 1].step)
            st = lp.ev[lp.n - 1].step + 1;      /* a short tap: it still sounds a step */
        if (st < (int32_t)lp.len) { lp.ev[lp.n].step = (uint16_t)st; lp.ev[lp.n].key = (int8_t)key; lp.n++; }
    }
}

static void lp_fire(int i)
{
    lp.firing = 1;
    if (lp.ev[i].key >= 0) { held_idx = -1; play(lp.ev[i].key); }
    else if (active) stop();
    lp.firing = 0;
}

static void lp_close(uint32_t now)
{
    float sm = step_ms();
    uint32_t bars = (uint32_t)((float)(now - lp.t0) / (sm * 16.0f)) + 1u;
    if (bars > LP_BARS) bars = LP_BARS;
    lp.len = (uint16_t)(bars * 16u);
    lp.t0 += (uint32_t)((float)lp.len * sm);    /* the next round starts where the recording's bars end */
    lp.last = -1;
    lp.st = LP_PLAY;
    app_say("LOOP", "PLAYING - PLAY ON TOP");
    if (run.mtype == MS_LOOP) { run.mprog = 1.0f; mission_check(); }
}

static void lp_tick(uint32_t now)
{
    float sm = step_ms();
    int32_t cur;
    int i;
    if (lp.st == LP_REC && (float)(now - lp.t0) >= (float)lp.len * sm) lp_close(lp.t0 + (uint32_t)((float)lp.len * sm) - 1u);
    if (lp.st != LP_PLAY || (int32_t)(now - lp.t0) < 0) return;
    cur = (int32_t)((float)(now - lp.t0) / sm);
    while (cur >= (int32_t)lp.len) {             /* the round ends: its last events, then the next round */
        for (i = 0; i < lp.n; i++) if ((int32_t)lp.ev[i].step > lp.last) lp_fire(i);
        lp.t0 += (uint32_t)((float)lp.len * sm);
        cur -= lp.len;
        lp.last = -1;
    }
    for (i = 0; i < lp.n; i++)
        if ((int32_t)lp.ev[i].step > lp.last && (int32_t)lp.ev[i].step <= cur) lp_fire(i);
    lp.last = cur;
    run.busy_at = now;
}

/* REC: a tap (on release) moves the loop on; held 1 s, it is gone */
static void lp_button(int down, uint32_t now)
{
    if (down) { lp.rec_down = now; lp.held_done = 0; return; }
    if (lp.held_done) return;
    switch (lp.st) {
    case LP_OFF: lp.st = LP_READY; app_say("LOOP", "PLAY A CHORD TO RECORD"); break;
    case LP_READY: lp.st = LP_OFF; app_say("LOOP", "OFF"); break;
    case LP_REC: lp_close(now); break;
    case LP_PLAY: lp.st = LP_PAUSE; lp.firing = 1; if (active) stop(); lp.firing = 0; app_say("LOOP", "PAUSED"); break;
    default: lp.st = LP_PLAY; lp.t0 = now; lp.last = -1; app_say("LOOP", "PLAYING"); break;
    }
}
static void lp_hold(uint32_t now)
{
    if (lp.held_done || lp.st == LP_OFF || now - lp.rec_down < 1000u) return;
    lp.held_done = 1;
    if (lp.st == LP_PLAY && active) { lp.firing = 1; stop(); lp.firing = 0; }
    lp.st = LP_OFF; lp.n = 0;
    app_say("LOOP", "CLEARED");
}

static void game_init(void)
{
    if (plat_store_load(OBJ_PROGRESS, &prog, sizeof prog) != (int)sizeof prog || prog.magic != PROG_MAGIC) {
        memset(&prog, 0, sizeof prog);
        prog.magic = PROG_MAGIC;
    }
    if (set.rsv[0] >= chars_unlocked()) set.rsv[0] = 0;
    mission_start();
}

/* the player's own chord (a key down / a stop): the loop records it, the run counts it */
static void user_chord(int key)
{
    lp_note(key);
    game_chord(key);
}

/* once a frame: the runner's speed and metres, the end of a run, the timed missions */
static int obst_near(int wo, int cx);
static void game_step(uint32_t now, float dt)
{
    int busy = active || now - run.note_at < 1200u || lp.st == LP_PLAY || lp.st == LP_REC;
    float bpm = ck_bpm > 0.0f ? ck_bpm : (float)set.bpm, vt = busy ? 5.0f * bpm / 120.0f : 0.0f, dm;
    if (busy) run.busy_at = now;
    run.speed += (vt - run.speed) * fm_minf(1.0f, dt * (busy ? 3.0f : 1.4f));
    if (!busy && run.speed < 0.6f) run.speed = obst_near((int)run.scroll, 146) ? 0.6f : 0.0f;   /* it stops on free ground */
    dm = run.speed * dt;
    run.scroll += dm * 12.0f;                   /* 12 px a metre */
    if (run.scroll > 30000.0f) run.scroll -= 30000.0f;   /* (a multiple of the obstacles' 300 px) */
    if (busy && !run.running) { run.running = 1; run.run_m = 0.0f; }
    if (run.running) {
        run.run_m += dm;
        if (run.mtype == MS_RUN && run.run_m > run.mprog) { run.mprog = run.run_m; mission_check(); }
        if (!busy && now - run.busy_at > 3000u) {       /* the run ends */
            run.running = 0;
            if ((uint32_t)run.run_m > prog.best) {
                char t[16];
                if (run.run_m >= 50.0f) { int_text((int32_t)run.run_m, t); text_cat(t, " M"); show_toast("NEW BEST RUN", t); }
                prog.best = (uint32_t)run.run_m; prog_dirty = 1;
            }
        }
    }
    if (run.mtype == MS_TOTAL && dm > 0.0f) { run.mprog += dm; mission_check(); }
    if (run.mtype == MS_DRUMS && active && set.drums && set.rhythm) { run.mprog += dt; mission_check(); }
    run.carry += dm;
    if (run.carry >= 1.0f) { uint32_t w = (uint32_t)run.carry; run.carry -= (float)w; add_metres(w); }
}

static void button(int k)
{
    char t[8];
    int p;
    for (p = 1; p < NPAGE; p++)
        if (PAGE_BTN[p] == k) {          /* a page; the same button again: back to the main page */
            set.page = (uint8_t)(set.page == p ? PG_MAIN : p);
            dirty();
            return;
        }
    switch (k) {
    case B_HOME: ui.home_at = plat_ms(); ui.home_long = 0; return;    /* acts on release (held = help) */
    case B_SAVE:
        ui.bowl ^= 1u;
        eng_muffle(ui.bowl ? 1.0f : 0.0f);
        return;
    case B_REC:
        if ((ui.btn >> B_HOME) & 1u) return;      /* HOME + REC: the reset (input), not the loop */
        lp_button(1, plat_ms());                  /* the loop: it acts on release (held: cleared) */
        return;
    case B_SEQ: {                                /* the character that runs: the next one unlocked */
        int n = chars_unlocked();
        set.rsv[0] = (uint8_t)((set.rsv[0] + 1u) % (uint32_t)(n > 0 ? n : 1));
        if (n < 10) {
            char t[24], m[12];
            t[0] = 0; text_cat(t, CHR_NAME_B[set.rsv[0]]); text_cat(t, "  NEXT AT ");
            int_text((int32_t)UNLOCK_M[n], m); text_cat(t, m); text_cat(t, " M");
            app_say("CHARACTER", t);
        } else app_say("CHARACTER", CHR_NAME_B[set.rsv[0]]);
        hop_at = plat_ms();
        break;
    }
    case B_ARP: case B_PLAY: fun(k); return;
    case B_OCTDN:
    case B_OCTUP:
        set.octave = (int8_t)clamp(set.octave + (k == B_OCTUP ? 1 : -1), -2, 2);
        t[0] = set.octave < 0 ? '-' : '+'; t[1] = (char)('0' + (set.octave < 0 ? -set.octave : set.octave)); t[2] = 0;
        app_say("OCTAVE", t);
        rechord();
        break;
    default: return;
    }
    dirty();
}

/* the keys that play a chord in the current set (a bit per key) */
static uint32_t valid_keys(void)
{
    uint32_t m = 0, k;
    int v, o;
    for (k = 0; k < NKEYS; k++)
        if (key_slot((int)k, &v, &o) >= 0) m |= 1u << k;
    return m;
}

static void input(void)
{
    uint32_t keys = plat_keys(), btn = plat_buttons(), k, lit_keys = 0, lit_btn = 0, now = plat_ms();
    int32_t e;
    clock_follow(now);
    if (!kb_ready) kb_init();
    for (k = 0; k < NKEYS; k++) {                               /* the white keys: the melody voice (the last key pressed sounds) */
        uint32_t bit = 1u << k;
        if (kb_white[k] < 0) continue;
        if ((keys & bit) && !(ui.keys & bit)) {
            ld_held |= bit; ld_cur = (int)k; hop_at = now; eng_lead(mel_pitch(kb_white[k]));
            game_note();
        } else if (!(keys & bit) && (ui.keys & bit)) {
            ld_held &= ~bit;
            if (ld_cur == (int)k) {
                int j;
                ld_cur = -1;
                for (j = NKEYS - 1; j >= 0; j--) if (ld_held & (1u << j)) { ld_cur = j; break; }
                if (ld_cur >= 0) eng_lead(mel_pitch(kb_white[ld_cur])); else eng_lead(-1.0f);
            }
        }
    }
    for (k = 0; k < NKEYS; k++) {
        uint32_t bit = 1u << k;
        int kv, ko;
        if (key_slot((int)k, &kv, &ko) < 0) continue;            /* a silent key */
        if ((keys & bit) && !(ui.keys & bit)) {
            /* with a rhythm the chord keeps playing: the same key again stops it */
            if (set.rhythm && active && held_idx < 0 && cur_idx == (int)k) { stop(); user_chord(-1); }
            else { held_idx = (int)k; play((int)k); user_chord((int)k); }
        } else if (!(keys & bit) && (ui.keys & bit) && held_idx == (int)k) {
            held_idx = -1;
            if (!set.rhythm) { stop(); user_chord(-1); }
        }
    }
    for (k = 0; k < NB; k++) {
        uint32_t bit = 1u << k;
        if ((btn & bit) && !(ui.btn & bit)) button((int)k);
    }
    /* the loop: REC let go (a tap), REC held (cleared), its steps */
    if (!((btn >> B_REC) & 1u) && ((ui.btn >> B_REC) & 1u) && !((btn >> B_HOME) & 1u) && !ui.reset_at) lp_button(0, now);
    if (((btn >> B_REC) & 1u) && !((btn >> B_HOME) & 1u)) lp_hold(now);
    lp_tick(now);
    /* HOME + REC held 3 s: back to the settings of the first start (a countdown; let go before: nothing) */
    if (((btn >> B_HOME) & 1u) && ((btn >> B_REC) & 1u)) {
        uint32_t held;
        char t[4];
        if (!ui.reset_at) { ui.reset_at = now | 1u; ui.reset_shown = 0; ui.reset_done = 0; }
        lp.held_done = 1;                         /* (REC let go after this: no loop tap) */
        ui.home_long = 1; ui.help = 0;
        held = now - ui.reset_at;
        if (!ui.reset_done && held >= 3000u) {
            uint8_t page = PG_MAIN;
            ui.reset_done = 1;
            if (active) stop();
            settings_defaults();
            set.page = page;
            ui.bowl = ui.clean = 0;
            eng_muffle(0.0f);
            send_sound(); send_rhythm(); send_groove(); send_drums(); send_bass(); send_fx(); send_params();
            dirty();
            app_say("RESET", "FIRST SETTINGS BACK");
        } else if (!ui.reset_done && ui.reset_shown != 1u + held / 1000u) {
            ui.reset_shown = (uint8_t)(1u + held / 1000u);
            t[0] = (char)('0' + 4 - ui.reset_shown); t[1] = 0;
            app_say("RESET IN", t);
        }
    } else if (ui.reset_at) {
        if (!ui.reset_done) app_say("RESET", "CANCELLED");
        ui.reset_at = 0;
    }
    /* HOME: a short press, back to the main page; held, the help screen while held */
    if ((btn >> B_HOME) & 1u) {
        if (!ui.home_long && now - ui.home_at > 600u) { ui.home_long = 1; ui.help = 1; ui.help_drawn = 0; }
        if (ui.reset_at) ui.help = 0;
    } else if ((ui.btn >> B_HOME) & 1u) {
        if (ui.home_long) { ui.help = 0; ui.help_drawn = 0; }
        else if (set.page != PG_MAIN) { set.page = PG_MAIN; dirty(); }
    }
    for (k = 0; k < NE; k++)
        if ((e = plat_enc((int)k)) != 0)
            encoder((int)k, e);
    ui.keys = keys;
    ui.btn = btn;
    /* auto-save: 4 s after the last change, and never while a chord plays (a flash write stops the sound) */
    if (set_dirty && !active && lp.st != LP_PLAY && plat_ms() - dirty_at > 4000u) {
        if (plat_store_save(OBJ_SETTINGS, &set, sizeof set) == 0) set_dirty = 0;
        else dirty_at = plat_ms();
    }
    /* the progress too: after 4 s of quiet (no chord, no melody, no loop) */
    if (prog_dirty && !active && !ld_held && lp.st != LP_PLAY && lp.st != LP_REC && now - run.busy_at > 4000u) {
        prog.magic = PROG_MAGIC;
        if (plat_store_save(OBJ_PROGRESS, &prog, sizeof prog) == 0) prog_dirty = 0;
        else run.busy_at = now;
    }
    /* lights: the keys glow, the one playing is lit; the page's button, the fun ones that are on, the pressed one */
    lit_keys = valid_keys();
    for (k = 0; k < NKEYS; k++) if (kb_white[k] >= 0 && ((keys >> k) & 1u)) lit_keys |= 1u << k;   /* a melody key lights while pressed */                                    /* the keys that have a chord glow; a latched chord blinks */
    if (cur_idx >= 0 && held_idx < 0 && (now / 300u) % 2u) lit_keys &= ~(1u << (uint32_t)cur_idx);
    if (set.page != PG_MAIN) lit_btn |= 1u << PAGE_BTN[set.page];
    if (ui.bowl) lit_btn |= 1u << B_SAVE;
    if (lp.st == LP_PLAY || ((lp.st == LP_REC || lp.st == LP_READY) && (now / (lp.st == LP_REC ? 200u : 500u)) % 2u))
        lit_btn |= 1u << B_REC;
    if (ui.clean) lit_btn |= LED_PLAY_GREEN;
    /* OCT- / OCT+: lit on the side the octave went, blinking at two octaves */
    if (set.octave) {
        uint32_t ob = 1u << (set.octave < 0 ? B_OCTDN : B_OCTUP);
        if (set.octave == 1 || set.octave == -1 || (now / 300u) % 2u) lit_btn |= ob;
    }
    lit_btn |= btn & ((1u << 14) - 1u);  /* a button lights fully while it is pressed */
    plat_lights(lit_btn, lit_keys, 0, (1u << NKEYS) - 1u, 1);
    /* the 12 buttons and OCT- / OCT+ always softly on (one LED colour each: the active one fully lit, the others mid) */
    plat_lights_mid(((1u << 14) - 1u) & ~lit_btn, 0);
}

void app_input_only(void) { input(); }

/* ------------------------------------------------------------------ screen (240 x 240) --- */
/* The 2027 look (below) is drawn in two halves (240 x 120) into the canvas and sent, ~25 times a second. At the start, and while
 * HOME is held: the intro (the title, the credits, the player's progress). */
static inline float clamp01(float x) { return x < 0.0f ? 0.0f : x > 1.0f ? 1.0f : x; }

static struct {
    float title, title_ms;                       /* the intro: shown (0..1), its time (ms) */
    uint32_t start_at;
    uint8_t started;
} sc;

static void scene_init(void) { memset(&sc, 0, sizeof sc); }

/* ---- the bottom row: PLAY hides the info */
static void fun(int k)
{
    if (k == B_PLAY) ui.clean ^= 1u;
}

/* ---- once a frame: the intro at the start (3 s, then it fades), and while HOME is held */
static void scene_step(uint32_t now, float dt)
{
    uint32_t since;
    float tt;
    if (!sc.started) { sc.started = 1; sc.start_at = now; }     /* the intro counts from the first frame */
    since = now - sc.start_at;
    tt = ui.help ? 1.0f : since < 3000u ? 1.0f : since < 3500u ? 1.0f - (float)(since - 3000u) / 500.0f : 0.0f;
    sc.title_ms = ui.help ? (float)(now - ui.home_at - 600u) : (float)since;
    sc.title += (tt - sc.title) * fm_minf(1.0f, dt * 8.0f);
    if (tt >= 1.0f && since < 3000u) sc.title = 1.0f;
}

/* ---- the info bands, crisp over the picture */
static uint16_t page_col(void) { const uint8_t *c = PAGE_RGB[set.page]; return RGB(c[0], c[1], c[2]); }

static void draw_bottom_text(void)       /* in the second canvas: y 0 = screen row 120 */
{
    int k;
    int32_t y0 = 189 - 120, tw = text_w(&FONT_T, PAGE_NAME[set.page]) + 10;
    uint16_t pc = page_col();
    char t[32];
    cv_rect(0, y0 - 3, 240, 60, RGB(5, 9, 11));
    cv_rect(0, y0 - 3, 240, 1, RGB(70, 96, 104));
    cv_rect(6, y0 + 1, tw, 13, pc);
    cv_text(11, y0 + 1, &FONT_T, PAGE_NAME[set.page], RGB(0, 0, 0));
    t[0] = 0;
    text_cat(t, set.rhythm ? RHYTHM_NAME[set.rhythm] : "RHYTHM OFF");
    if (set.drums) text_cat(t, " + DRUMS");
    cv_text(232 - text_w(&FONT_T, t), y0 + 1, &FONT_T, t, RGB(170, 166, 180));
    for (k = 0; k < 4; k++) {
        char v[24];
        const kdef_t *d = &KD[set.page][k];
        int32_t x = 6 + k * 58;
        float f = knob_text(d, v);
        const felucca_font_t *vf = text_w(&FONT_S, v) <= 56 ? &FONT_S : &FONT_T;
        cv_text(x, y0 + 15, &FONT_T, d->lab, RGB(190, 186, 200));
        cv_text(x, y0 + 28 + (vf == &FONT_T ? 2 : 0), vf, v, RGB(250, 248, 252));
        cv_rect(x, y0 + 47, 52, 2, RGB(30, 44, 50));
        cv_rect(x, y0 + 47, (int32_t)(52.0f * f + 0.5f), 2, pc);
    }
}


/* ===================================================================================================== 2027 look ===
 * A picture behind everything: a deep petrol-to-ember sky with drifting dust, pulsing rings of an orbiting sphere, a dot-matrix
 * floor (intro) and a little runner: a geometric character that hops over spikes and blocks, and when a chord is played. All drawn from
 * rectangles; (x, y) in screen rows 0..239, g_oy = the screen row of the canvas' first row (0 or 120). */
static int g_oy;
#define NSR(x, y, w, h, c) cv_rect((x), (y) - g_oy, (w), (h), (c))
#define NPS(x, y, c) cv_rect((x), (y) - g_oy, 1, 1, (c))
#define HY 184                                                    /* the horizon of the main screen (just above the info panel) */
#define ACC1 RGB(200, 255, 60)                                  /* acid lime */
#define ACC2 RGB(255, 110, 80)                                  /* coral */
#define SOFT RGB(150, 172, 182)                                 /* cool grey */

static int isqrt_i(int v) { int r = 0; while ((r + 1) * (r + 1) <= v) r++; return r; }
static float ease3(float k) { k = clamp01(k); return 1.0f - (1.0f - k) * (1.0f - k) * (1.0f - k); }

static void neon_sky(int hy, uint32_t now)
{
    int y, i;
    for (y = 0; y < hy; y += 3) {
        int k = y * 255 / hy, r, g, b, h = hy - y < 3 ? hy - y : 3;
        if (y + h <= g_oy || y >= g_oy + 120) continue;
        if (k < 140) { r = 4 + 6 * k / 140; g = 6 + 30 * k / 140; b = 10 + 36 * k / 140; }
        else { r = 10 + 170 * (k - 140) / 115; g = 36 + 56 * (k - 140) / 115; b = 46 + 18 * (k - 140) / 115; }
        NSR(0, y, 240, h, RGB(r, g, b));
    }
    for (i = 0; i < 22; i++) {                                    /* stars, a few twinkle */
        int x = (i * 97 + 13) % 236 + 2, sy = (i * 53 + 7) % (hy * 2 / 3);
        int tw = ((now / 260u) + (uint32_t)i * 7u) % 6u == 0u;
        if (sy + 2 <= g_oy || sy >= g_oy + 120) continue;
        NSR(x, sy, tw ? 2 : 1, tw ? 2 : 1, tw ? RGB(235, 255, 200) : RGB(90, 116, 124));
    }
}

/* the orb: a glowing core and thin rings that breathe (centre cx, ccy; radius r), cut at the horizon hy */
static void neon_sun(int cx, int ccy, int r, int hy, uint32_t now)
{
    int ring, y;
    int core = r / 3 + (int)(2.0f * fm_sin_turns((float)(now % 3000u) / 3000.0f));
    for (y = ccy - core; y < ccy + core && y < hy; y++) {                 /* the core */
        int dy = y - ccy, half = isqrt_i(core * core - dy * dy);
        if (y + 1 <= g_oy || y >= g_oy + 120) continue;
        NSR(cx - half, y, 2 * half, 1, RGB(255, 170 - 60 * (y - ccy + core) / (2 * core), 110));
    }
    for (ring = 0; ring < 3; ring++) {
        int rr = r * (ring + 2) / 4 + (int)(2.5f * fm_sin_turns((float)((now + (uint32_t)ring * 900u) % 4000u) / 4000.0f));
        uint16_t c = ring == 0 ? RGB(200, 255, 60) : ring == 1 ? RGB(110, 150, 50) : RGB(60, 84, 50);
        for (y = ccy - rr; y <= ccy + rr && y < hy; y++) {
            int dy = y - ccy, ho = isqrt_i(rr * rr - dy * dy), hi = rr - dy * dy / (rr ? rr : 1) > 0 ? isqrt_i((rr - 2) * (rr - 2) - dy * dy) : 0;
            if (y + 1 <= g_oy || y >= g_oy + 120) continue;
            if (dy * dy > (rr - 2) * (rr - 2)) NSR(cx - ho, y, 2 * ho, 1, c);
            else { NSR(cx - ho, y, ho - hi, 1, c); NSR(cx + hi, y, ho - hi, 1, c); }
        }
    }
}

/* the dot-matrix floor of the intro: rows of dots that rush towards us and spread out */
static void neon_floor(int hy, int bot, uint32_t now)
{
    int i, j, y, H = bot - hy;
    float fr = (float)(now % 900u) / 900.0f;
    NSR(0, hy, 240, H, RGB(5, 11, 13));
    for (i = 0; i < 10; i++) {
        float p = ((float)i + fr) / 10.0f;
        int sp, n2, d;
        y = hy + 2 + (int)(p * p * (float)(H - 2));
        if (y >= bot) continue;
        sp = 5 + (int)(p * 26.0f);
        d = p > 0.55f ? 2 : 1;
        for (j = -14; j <= 14; j++) {
            int x = 120 + j * sp;
            n2 = (int)(40.0f + 190.0f * p);
            if (x < 0 || x > 239) continue;
            NSR(x, y, d, d, RGB(n2 * 4 / 5, n2, n2 / 4));
        }
    }
    NSR(0, hy, 240, 1, RGB(150, 172, 182));
}

/* ---- the characters: 10 original shapes (no game's characters): inside(kind, x, y) in the shape's own pixels */
#define NCHR 10
static const char *const CHR_NAME[NCHR] = {"CUBE", "DIAMOND", "SPIKE", "ORB", "HEXAGON", "CROSS", "STAR", "ARROW", "RING", "HOURGLASS"};
static const uint8_t CHR_COL[NCHR][3] = {{200, 255, 60}, {255, 110, 80}, {120, 200, 255}, {255, 190, 90}, {110, 240, 190},
                                         {236, 230, 214}, {255, 150, 120}, {90, 170, 230}, {210, 170, 255}, {160, 255, 150}};
static int chr_in(int kind, float x, float y)
{
    float ax = fm_fabsf(x), ay = fm_fabsf(y), r2 = x * x + y * y;
    switch (kind) {
    case 0: return ax <= 6.5f && ay <= 6.5f;
    case 1: return ax + ay <= 9.0f;
    case 2: return y >= -8.0f && y <= 6.5f && ax <= (y + 8.0f) * 0.5f;
    case 3: return r2 <= 49.0f;
    case 4: return ay <= 6.5f && ax + ay * 0.5774f <= 7.5f;
    case 5: return (ax <= 2.6f && ay <= 8.0f) || (ay <= 2.6f && ax <= 8.0f);
    case 6: return ax * ay <= 6.0f && ax + ay <= 10.0f;
    case 7: return (x >= -8.0f && x <= 0.0f && ay <= 2.6f) || (x > 0.0f && x <= 8.0f && ay <= (8.0f - x) * 0.9f);
    case 8: return r2 >= 20.0f && r2 <= 56.0f;
    default: return ay <= 7.5f && ax <= ay * 0.9f + 0.8f;
    }
}

static void neon_char(int kind, int cx, int cy, float ang)
{
    float c = fm_sin_turns(ang * 0.15915494f + 0.25f), s = fm_sin_turns(ang * 0.15915494f);
    const uint8_t *fc = CHR_COL[kind];
    int dx, dy;
    for (dy = -11; dy <= 11; dy++)
        for (dx = -11; dx <= 11; dx++) {
            float lx = (float)dx * c + (float)dy * s, ly = -(float)dx * s + (float)dy * c;
            if (!chr_in(kind, lx, ly)) continue;
            if (!(chr_in(kind, lx + 1.3f, ly) && chr_in(kind, lx - 1.3f, ly) && chr_in(kind, lx, ly + 1.3f) && chr_in(kind, lx, ly - 1.3f)))
                NPS(cx + dx, cy + dy, RGB(245, 250, 238));
            else if (fm_fabsf(lx) < 1.6f && fm_fabsf(ly) < 1.6f) NPS(cx + dx, cy + dy, RGB(8, 14, 16));
            else NPS(cx + dx, cy + dy, RGB(fc[0], fc[1], fc[2]));
        }
}

static void neon_spike(int x, int gy)
{
    int r;
    for (r = 0; r < 11; r++) {
        int w = 1 + r * 11 / 10;
        NSR(x + 6 - w / 2, gy - 11 + r, w, 1, r < 3 ? RGB(255, 190, 160) : ACC2);
    }
}
static void neon_block(int x, int gy)
{
    NSR(x, gy - 12, 12, 12, RGB(10, 22, 26));
    NSR(x, gy - 12, 12, 1, ACC1); NSR(x, gy - 1, 12, 1, ACC1);
    NSR(x, gy - 12, 1, 12, ACC1); NSR(x + 11, gy - 12, 1, 12, ACC1);
    NSR(x + 4, gy - 8, 4, 4, RGB(90, 120, 40));
}

static const struct { uint8_t type; int16_t x; } OBST[5] = {{0, 40}, {0, 56}, {1, 130}, {0, 200}, {1, 262}};

/* an obstacle under the runner at column cx, the ground scrolled by wo px (it does not stop on one) */
static int obst_near(int wo, int cx)
{
    int i, per = 300;
    for (i = 0; i < 5; i++) {
        int d = ((OBST[i].x - wo) % per + per) % per - 40 + 6 - cx;
        if (d > -26 && d < 26) return 1;
    }
    return 0;
}

/* the runner: the ground at row gy, the character at column cx; the ground scrolled by wo px (the obstacles move left as
 * the runner runs), the character hops over each one (and when a chord is played) */
static void neon_runner(int gy, int cx, int wo, int with_obst, uint32_t now)
{
    int i, per = 300;
    float h = 0.0f, ang = 0.0f;
    uint32_t u = now - hop_at;
    NSR(0, gy, 240, 2, ACC1);
    NSR(0, gy + 2, 240, 1, RGB(60, 84, 90));
    if (with_obst)
        for (i = 0; i < 5; i++) {
            int x = ((OBST[i].x - wo) % per + per) % per - 40, ox = x + 6, d = ox - cx;
            if (OBST[i].type == 0) neon_spike(x, gy); else neon_block(x, gy);
            if (d > -24 && d < 24) {
                float k = 1.0f - (float)(d * d) / 576.0f, a = (float)(24 - d) / 48.0f * 1.5708f;
                if (k * 19.0f > h) { h = k * 19.0f; ang = a; }
            }
        }
    if (u < 420u) {
        float k = (float)u / 420.0f, hh = 15.0f * fm_sin_turns(k * 0.5f);
        if (hh > h) { h = hh; ang = k * 1.5708f; }
    }
    if (h < 0.5f) { ang = 0.0f; h = 0.0f; }
    if (!with_obst || run.speed > 0.5f)                  /* the speed trail, while it runs */
        for (i = 1; i <= 3; i++) NSR(cx - 9 - i * 6, gy - 4, 3, 3, i == 1 ? RGB(170, 220, 60) : i == 2 ? RGB(100, 140, 50) : RGB(50, 70, 40));
    neon_char(set.rsv[0] < NCHR ? set.rsv[0] : 0, cx, gy - 8 - (int)(h + 0.5f), ang);
}

#define TITLE_1 "CHORD"                 /* the name, two lines on the intro: change these two words (and APP_NAME in app.h) */
#define TITLE_2 "RUN"

/* ---- the intro (and the help: HOME held): the sun rises, the title drops in, the floor rushes, the runner crosses */
static void neon_splash(int bottom)
{
    const int hy = 150;
    float ms = sc.title_ms, e = ease3((ms - 100.0f) / 1100.0f);
    uint32_t now = plat_ms(), t = (uint32_t)(ms < 0.0f ? 0.0f : ms);
    int ty, cx;
    char ln[24];
    g_oy = bottom ? 120 : 0;
    neon_sky(hy, now);
    neon_sun(120, hy - 12 + (int)((1.0f - e) * 80.0f), 52, hy, now);
    neon_floor(hy, 240, now);
    cx = (int)(-20.0f + ms * 0.115f);
    if (cx > 270) cx = 270;
    neon_runner(214, cx, 0, 0, now);
    if (ms > 1500.0f && ms < 2300.0f && cx > 60) {}                    /* (the runner just crosses) */
    {   /* the title: two lines drop in with a bounce, in three colours (a split) */
        float k = ease3((ms - 250.0f) / 700.0f), q = 1.0f - k;
        const char *l1 = TITLE_1, *l2 = TITLE_2;
        int y1, y2, x1 = (240 - text_w(&FONT_L, l1)) / 2, x2 = (240 - text_w(&FONT_L, l2)) / 2;
        ty = -(int)(q * 90.0f) + (int)(6.0f * fm_sin_turns(q * 1.5f) * q);
        y1 = 22 + ty; y2 = 58 + ty;
        cv_text(x1, y1 - g_oy, &FONT_L, l1, RGB(245, 250, 238));
        cv_text(x2, y2 - g_oy, &FONT_L, l2, ACC1);
        NSR(x2, y2 + 36, text_w(&FONT_L, l2), 2, ACC2);
        if (ms > 900.0f) cv_text((240 - text_w(&FONT_S, "BY SWTDR34MZ")) / 2, y2 + 44 - g_oy, &FONT_S, "BY SWTDR34MZ", RGB(245, 250, 238));
    }
    if (t > 1100u) {                                                    /* the credit, the author's own words in the help */
        NSR(0, 226, 240, 14, RGB(5, 9, 11));
        ln[0] = 0; text_cat(ln, "BASED ON BLOP BY OBSIMO");
        cv_text((240 - text_w(&FONT_XS, ln)) / 2, 226 - g_oy, &FONT_XS, ln, SOFT);
    }
    if (ui.help && t > 600u) {                                         /* the help (HOME held): the player's progress */
        char a[32], n[12];
        NSR(0, 176, 240, 48, RGB(5, 9, 11));
        a[0] = 0; text_cat(a, "TOTAL "); int_text((int32_t)prog.metres, n); text_cat(a, n);
        text_cat(a, " M   BEST "); int_text((int32_t)prog.best, n); text_cat(a, n); text_cat(a, " M");
        cv_text((240 - text_w(&FONT_XS, a)) / 2, 180 - g_oy, &FONT_XS, a, ACC1);
        a[0] = 0; text_cat(a, "CHARACTERS "); int_text(chars_unlocked(), n); text_cat(a, n);
        text_cat(a, "/10   MISSIONS "); int_text((int32_t)prog.missions, n); text_cat(a, n);
        cv_text((240 - text_w(&FONT_XS, a)) / 2, 196 - g_oy, &FONT_XS, a, RGB(245, 250, 238));
        a[0] = 0; text_cat(a, "SONGS PLAYED "); int_text(popcount8(prog.songs), n); text_cat(a, n);
        text_cat(a, "/"); int_text(NGRID, n); text_cat(a, n);
        cv_text((240 - text_w(&FONT_XS, a)) / 2, 212 - g_oy, &FONT_XS, a, SOFT);
    }
}

/* ---- the main screen: the set's chords as neon tiles (the one playing lit), the chord playing big, the runner below.
 * bottom = 0: the upper canvas (screen rows 0..119), 1: the lower (120..239). */
static void box(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t c)
{
    cv_rect(x, y, w, 1, c); cv_rect(x, y + h - 1, w, 1, c);
    cv_rect(x, y, 1, h, c); cv_rect(x + w - 1, y, 1, h, c);
}

/* sparks around (cx, cy) in screen rows, age ms old (a celebration) */
static void party(int cx, int cy, uint32_t age)
{
    int i;
    float k = (float)age / 1600.0f, r = 8.0f + 46.0f * ease3(k);
    for (i = 0; i < 14; i++) {
        float a = (float)i / 14.0f + k * 0.15f;
        int x = cx + (int)(r * fm_sin_turns(a + 0.25f)), y = cy + (int)(r * 0.7f * fm_sin_turns(a)) + (int)(20.0f * k * k);
        uint16_t c = i % 3 == 0 ? ACC1 : i % 3 == 1 ? ACC2 : RGB(120, 200, 255);
        if (k < 0.85f || (age / 80u) % 2u) NSR(x, y, 3, 3, c);
    }
}

static void draw_screen(int bottom)
{
    const uint16_t W = RGB(245, 250, 238), K = RGB(8, 14, 10), L = SOFT, PU = RGB(70, 96, 104), TB = RGB(9, 20, 25);
    int n = grid_n[set.grid], cs = -1, cvar = 0, i;
    uint32_t now = plat_ms();
    char b[20], t[24];
    g_oy = bottom ? 120 : 0;
    neon_sky(HY, now);
    if (active && cur_idx >= 0) { int o; cs = key_slot(cur_idx, &cvar, &o); }
    if (bottom) {
        const char *r = RICH_NAME[set.rich];
        int32_t x;
        neon_sun(198, HY - 4, 34, HY, now);
        neon_runner(HY - 2, 146, (int)run.scroll, 1, now);
        if (now - run.party_at < 1600u && run.party_at) party(146, HY - 12, now - run.party_at);
        if (active) cv_text(8, 2, text_w(&FONT_L, chord_txt) <= 100 ? &FONT_L : &FONT_B, chord_txt, W);
        x = cv_text(8, 36, &FONT_XS, SOUND_NAME[set.sound], L);
        cv_text(x + 8, 36, &FONT_XS, r, ACC1);
        {   /* the metres: this run's (lime) while it runs, else all of them; under them the mission and how far it is */
            char m[32], pr[12];
            int32_t w;
            b[0] = 0;
            if (run.running && run.run_m >= 1.0f) { text_cat(b, "RUN "); int_text((int32_t)run.run_m, t); text_cat(b, t); text_cat(b, " M"); }
            else { int_text((int32_t)prog.metres, b); text_cat(b, " M"); }
            cv_text(232 - text_w(&FONT_S, b), 1, &FONT_S, b, run.running ? ACC1 : W);
            mission_text(m, pr);
            w = text_w(&FONT_T, m) + (pr[0] ? text_w(&FONT_T, pr) + 5 : 0);
            cv_rect(232 - w - 8, 23, 4, 4, ACC2);
            x = cv_text(232 - w, 18, &FONT_T, m, W);
            if (pr[0]) cv_text(x + 5, 18, &FONT_T, pr, ACC1);
        }
        return;
    }
    if (now < ui.msg_until) {                                   /* a message (SET ..., OCTAVE ...) takes the header's place */
        int32_t x = cv_text(8, 0, &FONT_XS, ui.msg[0], W);
        cv_text(x + 6, 0, &FONT_XS, ui.msg[1], ACC1);
    } else {
        cv_text(8, 0, &FONT_XS, GRID_DEF[set.grid].name, W);
        cv_text(8, 10, &FONT_XS, GRID_DEF[set.grid].by, L);
        t[0] = 0; text_cat(t, "KEY "); text_cat(t, NOTE[set.key]);
        cv_text(232 - text_w(&FONT_XS, t), 0, &FONT_XS, t, ACC1);
    }
    cv_rect(8, 22, 224, 1, PU);
    if (lp.st != LP_OFF) {   /* the loop: its state at the right, under the key; its round as a bar on the line */
        uint16_t lc = lp.st == LP_REC ? ACC2 : lp.st == LP_PLAY ? ACC1 : L;
        float pos = 0.0f;
        t[0] = 0;
        if (lp.st == LP_READY) text_cat(t, (now / 500u) % 2u ? "LOOP READY" : "");
        else if (lp.st == LP_PAUSE) text_cat(t, "LOOP PAUSED");
        else {
            int32_t el = (int32_t)(now - lp.t0);
            float stp = step_ms();
            pos = el < 0 ? 0.0f : (float)el / (stp * (float)lp.len);
            if (pos > 1.0f) pos = 1.0f;
            text_cat(t, lp.st == LP_REC ? "REC " : "LOOP ");
            int_text(el < 0 ? 1 : 1 + (int32_t)((float)el / (stp * 16.0f)), b); text_cat(t, b);
            text_cat(t, "/"); int_text(lp.len / 16, b); text_cat(t, b);
        }
        if (t[0]) cv_text(232 - text_w(&FONT_T, t), 11, &FONT_T, t, lc);
        if (lp.st == LP_REC || lp.st == LP_PLAY) cv_rect(8, 22, (int32_t)(224.0f * pos), 1, lc);
    }
    for (i = 0; i < n; i++) {
        int one = n <= 4, sel = i == cs;
        int32_t x = 6 + (one ? i : i % 4) * 58, y = one ? 32 : 28 + (i / 4) * 45, w = 54, h = one ? 62 : 41, tw, fh;
        const felucca_font_t *f = &FONT_L;
        chord_t c = slot_chord(i, 0, 0);
        const char *s;
        chord_name(&c, b);
        s = sel ? chord_txt : b;
        if (text_w(f, s) > w - 6) f = &FONT_S;
        tw = text_w(f, s);
        fh = f == &FONT_L ? 24 : 14;
        cv_rect(x, y, w, h, sel ? ACC1 : TB);
        box(x, y, w, h, sel ? ACC1 : PU);
        t[0] = (char)('1' + i); t[1] = 0;
        cv_text(x + 4, y + 3, &FONT_XS, t, sel ? K : L);
        cv_text(x + (w - tw) / 2, y + (h - fh) / 2 + 3, f, s, sel ? K : W);
    }
    if (now < toast.until) {   /* a celebration: a new character, a mission done, a best run */
        uint32_t age = now - toast.at;
        int32_t bw = 200, bh = 52, bx = 20, by = 40 + (age < 200u ? (int32_t)((200u - age) / 10u) : 0);
        uint16_t bc = (now / 150u) % 2u ? ACC1 : ACC2;
        cv_rect(bx, by, bw, bh, RGB(6, 14, 16));
        box(bx, by, bw, bh, bc);
        box(bx + 2, by + 2, bw - 4, bh - 4, PU);
        cv_text(120 - text_w(&FONT_S, toast.a) / 2, by + 8, &FONT_S, toast.a, bc);
        cv_text(120 - text_w(&FONT_XS, toast.b) / 2, by + 30, &FONT_XS, toast.b, W);
    }
    (void)K;
}


/* the first screen at boot (the platform's splash, while the FM-1 starts): the intro's very first picture (night sky,
 * sun, floor; the title is still above the screen), so the start runs on into the animation without any other screen */
void app_splash(void)
{
    scene_init();
    sc.title = 1.0f; sc.title_ms = 0.0f;
    cv_begin(240, 120, 0);
    neon_splash(0);
    cv_blit(0, 0);
    cv_begin(240, 120, 0);
    neon_splash(1);
    cv_blit(0, 120);
}

void app_ui_init(void) { scene_init(); }

void app_frame(void)
{
    uint32_t now = plat_ms();
    input();
    if (now - ui.frame_at >= 40u) {
        float dt = ui.frame_at ? (float)(now - ui.frame_at) * 0.001f : 0.04f;
        if (dt > 0.1f) dt = 0.1f;
        ui.frame_at = now;
        scene_step(now, dt);
        game_step(now, dt);
        {
            int over = sc.title < 0.5f;     /* the minimal screen; the intro and the help keep the old picture */
            cv_begin(240, 120, 0);
            if (over) draw_screen(0);
            else neon_splash(0);
            cv_blit(0, 0);
            cv_begin(240, 120, 0);
            if (over) { draw_screen(1); if (!ui.clean) draw_bottom_text(); }
            else neon_splash(1);
            cv_blit(0, 120);
        }
    }
    ann_tick(now);
}

#ifdef PLAT_HOST
int app_expect(const char *what, int *got)
{
    if (!strcmp(what, "grid")) *got = set.grid;
    else if (!strcmp(what, "key")) *got = set.key;
    else if (!strcmp(what, "lead")) *got = ld_cur >= 0;
    else if (!strcmp(what, "sound")) *got = set.sound;
    else if (!strcmp(what, "rhythm")) *got = set.rhythm;
    else if (!strcmp(what, "chord")) *got = cur_idx;
    else if (!strcmp(what, "active")) *got = active;
    else if (!strcmp(what, "rich")) *got = set.rich;
    else if (!strcmp(what, "octave")) *got = set.octave;
    else if (!strcmp(what, "dirty")) *got = set_dirty;
    else if (!strcmp(what, "bpm")) *got = set.bpm;
    else if (!strcmp(what, "syncbpm")) *got = (int)(ck_bpm + 0.5f);
    else if (!strcmp(what, "side")) *got = set.side;
    else if (!strcmp(what, "page")) *got = set.page;
    else if (!strcmp(what, "drums")) *got = set.drums;
    else if (!strcmp(what, "help")) *got = ui.help;
    else if (!strcmp(what, "bowl")) *got = ui.bowl;
    else if (!strcmp(what, "clean")) *got = ui.clean;
    else if (!strcmp(what, "cutoff")) *got = set.cutoff;
    else if (!strcmp(what, "delay")) *got = set.delay;
    else if (!strcmp(what, "bsound")) *got = set.bsound;
    else if (!strcmp(what, "reverb")) *got = set.reverb;
    else if (!strncmp(what, "mel", 3) && what[3] >= '0' && what[3] <= '9') *got = (int)mel_pitch(atoi(what + 3));   /* a white key's note now */
    else if (!strcmp(what, "loop")) *got = lp.st;
    else if (!strcmp(what, "loopn")) *got = lp.n;
    else if (!strcmp(what, "metres")) *got = (int)prog.metres;
    else if (!strcmp(what, "metres_h")) *got = (int)(prog.metres / 100u);   /* (hundreds: the run's own metres vary) */
    else if (!strcmp(what, "best_h")) *got = (int)(prog.best / 100u);
    else if (!strcmp(what, "running")) *got = run.running;
    else if (!strcmp(what, "unlocked")) *got = chars_unlocked();
    else if (!strcmp(what, "missions")) *got = (int)prog.missions;
    else if (!strcmp(what, "mission")) *got = run.mtype;
    else if (!strcmp(what, "character")) *got = set.rsv[0];
    else if (!strcmp(what, "dpat")) *got = set.dpat;
    else if (!strcmp(what, "msound")) *got = set.msound;
    else return 0;
    return 1;
}
#endif
