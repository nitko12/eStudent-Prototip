import mido

print(mido.get_output_names()[-1])
# exit()


port = mido.open_output(mido.get_output_names()[-1])


mid = mido.MidiFile('midi/Moji_Drugovi.kar')
for msg in mid.play():
    print(msg)
    port.send(msg)
