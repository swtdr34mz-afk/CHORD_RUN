# CHORD RUN

A firmware for the M-VAVE FM-1 by **swtdr34mz**, based on [Blop / OSR-1](https://github.com/OSR-Experiment/OSR-1) by
Obsimo (OSR Records). Built on [fm1-base](https://github.com/charlesvestal/fm1-base) and
[felucca-bsp](https://github.com/hugelton/felucca-bsp).

Play a song's chords with one finger and a melody over them, while a little runner runs as long as you play.

**Play it in the browser:** https://swtdr34mz-afk.github.io/CHORD_RUN/emu/ (the same firmware, built for the browser).

## What it does

- **Songs:** 17 song sets. A song's chords sit on the **black keys** (left to right). Each song brings its own
  sounds, tempo, bass and drum style.
- **Melody:** the **white keys** play the song's scale from its tonic, fitted to the chord that plays (a key keeps its
  note when the chord changes, or moves a semitone). ASSIST (MELODY 2) lights up the chord's notes on the beat, or GUIDE:
  a melody made for each song (an original one, not the record's) that lights up key by key, to play along.
- **Sounds:** 24 sounds in five families: ambient pads, soft keys, plucks (guitar, mallet, kalimba), keys (e-piano,
  grand keys and a **sampled GRAND PIANO**: the Salamander Grand Piano by Alexander Holm, CC-BY 3.0) and synths (80s
  brass, supersaw, pocket lead, an Oberheim-ish brass pad and brass lead). The melody has 18 of them (the guitar, mallet,
  kalimba, e-piano, grand keys and supersaw are for the chords only).
- **Drums:** twelve styles, each with its own kit: house, pop, trap, phonk, drill, afrobeat, reggaeton, lo-fi,
  dreamcore, angelcore, hip hop and 80s pop (808s, metal hi-hats with rolls, claps, cowbell, washed snares).
- **Loop:** press REC, play 4 bars of chords, and they play round while you play the melody on top. PLAY / STOP stops
  and restarts it.
- **The run:** the runner runs while you play. The metres add up, the best run is kept, ten characters unlock, and
  there is always a mission (play 24 chords, run 120 m at once, record a loop...) that gives extra metres.

## Controls

| Control | What it does |
| --- | --- |
| Black keys | The song's chords. With a rhythm the chord keeps playing (the same key again stops it); without one (HELD) it plays while you hold the key. The black keys right of the song's chords play the melody too: the semitone above the white key to their left (the notes the scale leaves out). They light softly when their note fits the chord, and stay dark when it rubs (a semitone over one of the chord's notes) |
| White keys | The melody |
| SELECT | Song (its own sounds, tempo and drums come with it) |
| ALGORITHM | Chord rhythm (9): held (while you hold the key), FULL (the chord held until you press its key again, with the drums), quarters, eighths, offbeat, pump, syncopated, tresillo, half-time |
| PRESET | The chords' sound |
| K1..K4 | The page's four settings (shown at the bottom of the screen) |
| FX | Delay, tremolo, chorus, reverb |
| SEL | Chords: key, voicing, BASS NOTE (root, 3rd, 5th, pedal: slash chords), LEVEL. SEL again: CHORDS 2: WIDTH, SPREAD (the chord's voices across the stereo field: diverge, alternate, diverge2, noteval) |
| ARP | The arpeggiator: MODE (up, down, up-down, random, up / down / up-down over 2 octaves, up over 3), RATE (1/4 to 1/32, triplets too), FLOW (short notes … notes that ring into the next: plucks ring longer, pads blend), SWING. With a chord rhythm (ALGORITHM) the rhythm's steps say which arp notes play; HELD and FULL: all of them |
| ENV | Groove: BPM, swing, density, sidechain |
| LFO | Bass: sound, level, riff, filter |
| EDIT | Melody: cutoff, level, SPACE (a reverb that grows, then echoes), preset. EDIT again: MELODY 2: glide, octave, vibrato, ASSIST (off → CHORD: the chord's notes light up on the beat → GUIDE: a melody made for each song lights up key by key, each hit 2 metres) |
| GLO | Drums: style, kick, hats, snare |
| HOME | Back to the main page. Held: the title and your progress |
| REC | The loop: ready → the first chord records 4 bars → it plays round. While recording: closes it. With a loop kept: ready for a new one (PLAY instead keeps the old one). Held 1 s: cleared |
| SEQ | The next character (the unlocked ones) |
| SAVE | The music heard from outside the room |
| PLAY | Stops the loop and starts it again from its start (the loop is kept); the header shows its bars |
| OCT- / OCT+ | Octave |
| HOME + REC held 3 s | Back to the first-start settings (your progress stays) |

Settings save themselves a few seconds after the last change; the progress after a few quiet seconds.

Chords and bass go out as MIDI over USB (channel 1), and an incoming MIDI clock sets the tempo.

## Building

Push to GitHub: the workflow builds the firmware (`.fwsc`) on every push (Actions → Artifacts). A tag `v1.3`, `v1.4`...
(or a release made on GitHub with such a tag) builds a release with the `.fwsc` attached.

Locally (Linux x86-64), the simulator and its tests:

    git clone --recursive https://github.com/swtdr34mz-afk/CHORD_RUN && cd CHORD_RUN
    python3 fm1-base/tools/build.py . --host
    build/host/chordrun_host tests/basic.txt build/host     # the basics: screenshots, sounds
    build/host/chordrun_host tests/run.txt build/host       # the run, missions, the loop, the progress kept
    build/host/chordrun_host tests/melody.txt build/host    # the melody's notes over the chords, MELODY 2, ASSIST

## Licence and credits

GPL-3.0-only (see `LICENSE`), as Blop / OSR-1 it is based on. The grand piano's samples (`res/piano.bin`, kept in the
FM-1's resource region; `tools/make_piano.py` makes them) are the Salamander Grand Piano V3 by Alexander Holm, CC-BY 3.0. fm1-base and felucca-bsp stay under MPL-2.0. The
installable package carries three files of JieLi's AC79 SDK (Apache-2.0). Every borrowed part is listed in
`CREDITS.md`.
