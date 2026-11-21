# Crédits / Credits

CHORD RUN by swtdr34mz, based on OSR-1 / Blop by Obsimo (OSR Records), under GPL-3.0-only.
CHORD RUN's own parts: the song sets and their sounds, the melody voice, the eight added sounds (NYLON PLUCK .. POCKET
LEAD), the drum styles and kits, the runner, missions and loop, the 2027 screens.

The GRAND PIANO's samples (res/piano.bin, made by tools/make_piano.py): nine notes of the **Salamander Grand Piano V3**
by **Alexander Holm** (a Yamaha C5), https://archive.org/details/SalamanderGrandPianoV3, under **CC-BY 3.0**
(https://creativecommons.org/licenses/by/3.0/), as remapped in https://github.com/sfzinstruments/SalamanderGrandPiano;
changed: mono, 22,050 Hz, levelled, shortened with a crossfaded loop, 4-bit ADPCM.

OSR-1 par Obsimo (OSR Records), sous GPL-3.0-only.

## Construit sur

| Projet | Auteur | Licence | Utilisé pour |
| --- | --- | --- | --- |
| [fm1-base](https://github.com/charlesvestal/fm1-base) | Charles Vestal | MPL-2.0 | plateforme : écran, panneau, audio, MIDI, stockage, simulateur, outils de build ; `web/emu/worklet.js` |
| [felucca-bsp](https://github.com/hugelton/felucca-bsp) | Leo Kuroshita (Hügelton Instruments) et contributeurs | MPL-2.0 | couche matérielle, chargeur de mise à jour, empaquetage |
| SDK AC79 de JieLi | JieLi Technology | Apache-2.0 | trois fichiers dans chaque paquet `.fwsc` (récupérés au build, jamais copiés ici) |

## Code repris d'autres firmwares

| Projet | Auteur | Licence | Utilisé pour |
| --- | --- | --- | --- |
| [DRIFT](https://github.com/FBNFRTG/drift-fm1) 0.9.6 / [Felucca-Engines](https://github.com/hugelton/Felucca-Engines) (moteur ANALOG) | Fabian Freitag ; Leo Kuroshita (Hügelton Instruments) | GPL-3.0-only ; MIT | `data.h` : les 13 sons (familles AMBIENT et SOFT KEYS) reprennent, convertis pour le moteur « sub » d'OSR-1, des réglages d'usine de DRIFT : PAPER STRINGS, STILL WATER, SOFT HORIZON, VELVET TAPE, AIR AND SALT, LOW CLOUD, AIRFIELD, SLOW BLOOM, GLASS TIDE, DUST PLUCK, MOON KEYS, SINE DROPS, NIGHT SIGNAL |

Chaque nouvel emprunt sera listé ici : projet, auteur, licence, fichiers concernés.
