import mido


def ticks_to_micro_seconds(ticks, tempo, ticks_per_beat):
    return ticks * (tempo / ticks_per_beat)


mid = mido.MidiFile("midi/fireworks_festival.mid")
ticks_per_beat = mid.ticks_per_beat
tempo = 500000

notes = []

for i, track in enumerate(mid.tracks):
    time = 0
    print("Track {}: {}".format(i, track.name))
    for msg in track:
        if msg.type == "set_tempo":
            tempo = msg.tempo
            print("Tempo: {:.2f} BPM".format(tempo))
        if msg.type == "note_on" or msg.type == "note_off":
            microseconds = ticks_to_micro_seconds(msg.time, tempo, ticks_per_beat)
            time += microseconds
            notes.append((msg.note, round(time), msg.type == "note_on"))

num_notes = 1999
notes = sorted(notes, key=lambda x: x[1])
notes = notes[:num_notes]

with open("inc/songs.h", "w") as f:
    f.write(f"#ifndef INC_SONGS_H_\n")
    f.write(f"#define INC_SONGS_H_\n")
    f.write("\n")
    f.write(f'#include "midi.h"\n')
    f.write("\n")
    f.write(f"#define NUM_MIDI_NOTES {len(notes)}\n")
    f.write(f"extern const struct note_message note_messages[NUM_MIDI_NOTES];\n")
    f.write("\n")
    f.write(f"#endif /* INC_SONGS_H_ */\n")

with open("src/songs.c", "w") as f:
    f.write(f'#include "songs.h"\n')
    f.write("\n")
    f.write(f"const struct note_message note_messages[NUM_MIDI_NOTES] = {{")
    for note, time, on in notes:
        f.write(f"{{0x{time:x},{note},{1 if on else 0}}},")
    f.write("};\n")
