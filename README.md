# CHORD RUN

A firmware for the M-VAVE FM-1 by **swtdr34mz**, based on [Blop / OSR-1](https://github.com/OSR-Experiment/OSR-1) by
Obsimo (OSR Records). Built on [fm1-base](https://github.com/charlesvestal/fm1-base) and
[felucca-bsp](https://github.com/hugelton/felucca-bsp).

Play a song's chords with one finger and a melody over them, while a little runner runs as long as you play.

## What it does

- **Songs:** 19 song sets. A song's chords sit on the **black keys** (left to right). Each song brings its own
  sounds, tempo, bass and drum style.
- **Melody:** the **white keys** play a melody in the scale of the chord that played last, so it always fits.
- **Sounds:** 21 sounds in five families: ambient pads, soft keys, plucks (guitar, mallet, kalimba), keys (e-piano,
  grand keys) and synths (80s brass, supersaw, pocket lead).
- **Drums:** ten current styles, each with its own kit: house, pop, trap, phonk, drill, afrobeat, reggaeton, lo-fi,
  dreamcore and angelcore (hi-hat rolls, claps, cowbell, washed snares).
- **Loop:** press REC, play 4 bars of chords, and they play round while you play the melody on top.
- **The run:** the runner runs while you play. The metres add up, the best run is kept, ten characters unlock, and
  there is always a mission (play 24 chords, run 120 m at once, record a loop...) that gives extra metres.

## Controls

| Control | What it does |
| --- | --- |
| Black keys | The song's chords. With a rhythm the chord keeps playing (the same key again stops it); without one (HELD) it plays while you hold the key |
| White keys | The melody |
| SELECT | Song (its own sounds, tempo and drums come with it) |
| ALGORITHM | Chord rhythm (8): held, quarters, eighths, offbeat, pump, syncopated, tresillo, half-time |
| PRESET | The chords' sound |
| K1..K4 | The page's four settings (shown at the bottom of the screen) |
| FX | Delay, tremolo, chorus, reverb |
| SCL | Chords: key, voicing, richness, strum |
| ENV | Groove: BPM, swing, density, sidechain |
| LFO | Bass: sound, level, riff, filter |
| EDIT | Melody: cutoff, level, SPACE (a reverb that grows, then echoes), preset |
| GLO | Drums: style, kick, hats, snare |
| HOME | Back to the main page. Held: the title and your progress |
| REC | The loop: ready → the first chord records 4 bars → it plays. Again: pause / play. Held 1 s: cleared |
| SEQ | The next character (the unlocked ones) |
| SAVE | The music heard from outside the room |
| PLAY | Hide the info |
| OCT- / OCT+ | Octave |
| HOME + REC held 3 s | Back to the first-start settings (your progress stays) |

Settings save themselves a few seconds after the last change; the progress after a few quiet seconds.

Chords and bass go out as MIDI over USB (channel 1), and an incoming MIDI clock sets the tempo.

## Building

Push to GitHub: the workflow builds the firmware (`.fwsc`) on every push (Actions → Artifacts). A tag `v1.3`, `v1.4`...
(or a release made on GitHub with such a tag) builds a release with the `.fwsc` attached.

Locally (Linux x86-64), the simulator and its tests:

    git clone --recursive <this repository> && cd plop
    python3 fm1-base/tools/build.py . --host
    build/host/chordrun_host tests/basic.txt build/host     # the basics: screenshots, sounds
    build/host/chordrun_host tests/run.txt build/host       # the run, missions, the loop, the progress kept

## Licence and credits

GPL-3.0-only (see `LICENSE`), as Blop / OSR-1 it is based on. fm1-base and felucca-bsp stay under MPL-2.0. The
installable package carries three files of JieLi's AC79 SDK (Apache-2.0). Every borrowed part is listed in
`CREDITS.md`.
