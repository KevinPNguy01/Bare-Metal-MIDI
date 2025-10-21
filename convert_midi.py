import mido


def ticks_to_micro_seconds(ticks, tempo, ticks_per_beat):
    return ticks * (tempo / ticks_per_beat)


songs = [
    {
        "file": "midi/fireworks_festival.mid",
        "title": "Fireworks Festival",
        "artist": "Weathering with You",
    },
    {
        "file": "midi/flower_dance.mid",
        "title": "Flower Dance",
        "artist": "DJ Okawari",
    },
    {
        "file": "midi/one_last_time.mid",
        "title": "One Last Time",
        "artist": "Ariana Grande",
    },
    {
        "file": "midi/gravity_falls.mid",
        "title": "Gravity Falls",
        "artist": "Brad Breeck",
    },
    {
        "file": "midi/golden_hour.mid",
        "title": "Golden Hour",
        "artist": "JVKE",
    },
    {
        "file": "midi/we_dont_talk_anymore.mid",
        "title": "We Don't Talk Anymore",
        "artist": "Charlie Puth ft. Selena Gomez",
    },
]

with open("inc/songs.h", "w") as h_file, open("src/songs.c", "w") as c_file:
    h_file.write(f"#ifndef INC_SONGS_H_\n")
    h_file.write(f"#define INC_SONGS_H_\n")
    h_file.write("\n")
    h_file.write(f'#include "midi.h"\n')
    h_file.write(f'#include "song.h"\n')
    h_file.write("\n")
    h_file.write(f"#define NUM_SONGS {len(songs)}\n")
    h_file.write("\n")

    c_file.write(f'#include "songs.h"\n')

    for index, song in enumerate(songs):
        file, title, artist = song.values()
        print(f"Processing {title} by {artist} from {file}")
        mid = mido.MidiFile(file)
        ticks_per_beat = mid.ticks_per_beat
        tempo = 500000

        notes = []

        for i, track in enumerate(mid.tracks):
            time = 62.5 * 1000
            print("Track {}: {}".format(i, track.name))
            for msg in track:
                if msg.type == "set_tempo":
                    tempo = msg.tempo
                if msg.type == "note_on" or msg.type == "note_off":
                    microseconds = ticks_to_micro_seconds(
                        msg.time, tempo, ticks_per_beat
                    )
                    time += microseconds
                    notes.append((msg.note, round(time), msg.type == "note_on"))
                    if (msg.velocity == 0) and (msg.type == "note_on"):
                        notes[-1] = (msg.note, round(time), False)

        num_notes = 4999
        notes = sorted(notes, key=lambda x: x[1])
        notes = notes[:num_notes]

        h_file.write("\n")
        h_file.write(f"#define SONG_{index}_NUM_NOTES {len(notes)}\n")
        h_file.write(f"extern const char song_{index}_title[];\n")
        h_file.write(f"extern const char song_{index}_artist[];\n")
        h_file.write(
            f"extern const struct note_message song_{index}_note_messages[SONG_{index}_NUM_NOTES];\n"
        )

        c_file.write("\n")
        c_file.write(f'const char song_{index}_title[] = "{title}";\n')
        c_file.write(f'const char song_{index}_artist[] = "{artist}";\n')
        c_file.write(
            f"const struct note_message song_{index}_note_messages[SONG_{index}_NUM_NOTES] = {{"
        )
        for note, time, on in notes:
            c_file.write(f"{{0x{10 * time:x},{note},{1 if on else 0}}},")
        c_file.write("};\n")

    h_file.write("\n")
    h_file.write(f"extern const struct Song songs[NUM_SONGS];\n")
    h_file.write("\n")
    h_file.write(f"#endif /* INC_SONGS_H_ */\n")

    c_file.write("\n")
    c_file.write(f"const struct Song songs[NUM_SONGS] = {{\n")
    for index in range(len(songs)):
        c_file.write(
            f"\t{{song_{index}_note_messages, SONG_{index}_NUM_NOTES, song_{index}_title, song_{index}_artist}},\n"
        )
    c_file.write("};\n")
