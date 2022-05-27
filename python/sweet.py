import mido
from mido import MidiFile
import pysine
import time
from Usb_interface import ArduinoUsbDevice

outport = mido.open_output()

WAIT_TILL_PLAY = 0.01
WAIT_BETWEEN_WRITE = 0.01

pick = [(60, 28), (29, 61), (62, 30), (31, 63)]
last_picked = [0, 0, 0, 0]

notes = [
    [(None, None, 0)],
    [(17, 33, 0)],
    [(49, 33, 0)],
    [(21, 37, 0)],
    [(53, 37, 0)],
    [(25, 41, 0), (None, None, 1)],
    [(57, 41, 0)],
    [(19, 35, 1)],
    [(51, 35, 1)],
    [(23, 39, 1)],
    [(55, 39, 1), (None, None, 2)],
    [(16, 32, 2)],
    [(48, 32, 2)],
    [(20, 36, 2)],
    [(52, 36, 2)],
    [(24, 40, 2), (None, None, 3)],
    [(56, 40, 2)],
    [(18, 34, 3)],
    [(50, 34, 3)],
    [(22, 38, 3)],
    [(54, 38, 3)],
]


def noteToFreq(note):
    a = 440  # frequency of A (coomon value is 440Hz)
    return (a / 32) * (2 ** ((note - 9) / 12))


# -60 za sweet
# -43 za sugar

pjesma = 'midi/sweet.mid'

port = mido.open_output(mido.get_output_names()[-1])
mid = MidiFile(pjesma)

# c je 60

offset2 = -60+1
offset1 = +1

pressed = [-1, -1, -1, -1]
holding = [-1, -1, -1, -1]

if __name__ == "__main__":

    try:
        theDevice = ArduinoUsbDevice(idVendor=0x16c0, idProduct=0x5df)

        print("Found: 0x%04x 0x%04x %s %s" % (theDevice.idVendor,
                                              theDevice.idProduct,
                                              theDevice.productName,
                                              theDevice.manufacturer))
    except:
        pass

    theDevice = ArduinoUsbDevice(idVendor=0x16c0, idProduct=0x5df)

    for x in range(0, 4):
        theDevice.write(pick[x][1 - last_picked[x]])
        time.sleep(0.01)

    for x in range(32, 32+10):
        theDevice.write(x)
        time.sleep(0.01)

    time.sleep(1)

    last_note = -1
    last_time = 0

    mid.ticks_per_beat *= 1

    for msg in mid.play():
        if (msg.type == "note_on" or msg.type == "note_off") and 0 <= int(msg.note) - offset1 <= 127:
            port.send(mido.Message(msg.type, note=(int(msg.note) - offset1)))
        else:
            port.send(msg)

        if time.time() - last_time < 0.01:
            continue

        # print("nice", )

        if msg.type == "note_on" and str(msg).find("channel=0") != -1:
            # print(msg.note)
            # if msg.channel != 1:
            #     continue

            # print("tu")

            nota = (int(msg.note) + offset2)
            print(int(msg.note))

            if(not (0 <= nota < 21)):
                continue

            print(nota)

            mappings = notes[nota]

            mapping = mappings[0]
            if mapping[0] is None:
                continue

            print(mapping, pressed)

            if pressed[mapping[2]] != -1:
                theDevice.write(pressed[mapping[2]])
                time.sleep(WAIT_BETWEEN_WRITE)

            pressed[mapping[2]] = mapping[1]
            holding[mapping[2]] = mapping[0]

            theDevice.write(mapping[0])
            time.sleep(WAIT_BETWEEN_WRITE)

            theDevice.write(pick[mapping[2]][last_picked[mapping[2]]])
            last_picked[mapping[2]] ^= 1
            time.sleep(WAIT_BETWEEN_WRITE)
            time.sleep(WAIT_TILL_PLAY)

            last_note = mapping[1]
            last_time = time.time()

            for x in range(0, 4):
                theDevice.write(holding[x])
                time.sleep(WAIT_BETWEEN_WRITE)

            # pysine.sine(frequency=noteToFreq(msg.bytes()[1]), duration=msg.time)
            # print(msg)
            # print(msg.channel, msg.note-57, msg.time)
