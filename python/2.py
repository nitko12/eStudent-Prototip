from mido import MidiFile
import pysine


mid = MidiFile('HesaPirate.mid')

# for msg in MidiFile('song.mid'):
#     time.sleep(msg.time)
#     if not msg.is_meta:
#         port.send(msg)


def noteToFreq(note):
    a = 440  # frequency of A (coomon value is 440Hz)
    return (a / 32) * (2 ** ((note - 9) / 12))


for msg in mid.play():

    if msg.type == "note_on":
        pysine.sine(frequency=noteToFreq(msg.bytes()[1]), duration=msg.time)
        print(msg)
