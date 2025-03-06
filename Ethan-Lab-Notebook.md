Trying to differentiate from existing 445 products and existing products on market

445 Projects

SP 2020 team 71 - Three strings at a time with automated strumming

[Proposal](https://courses.grainger.illinois.edu/ece445/getfile.asp?id=16908), [Design Doc](https://courses.grainger.illinois.edu/ece445/getfile.asp?id=18170),  [Final](https://courses.grainger.illinois.edu/ece445/getfile.asp?id=18265)

FA 2014 team 10 - User interface, one at a time, user plucked, manual motor control

[Design Review](https://courses.grainger.illinois.edu/ece445/getfile.asp?id=6275)

SP 2012 team 14 - UI, auto pluck, all six motors on same side, mounted to board

Market

[Tronical](https://www.tronicaltune.net/tronicaltune-plus/) - Custom frets and tuning attached to headstock, display for each string

Robot Guitar

Axcent - button/foot switch triggered, can switch tunings on the fly, bridge

Tune-o-matic - bridge 

Reddit Thread

Aim for something simple with longevity

Use for alternate tunings and not just tuning

Tremolo/vibrato functionality?

Polyphonic?

**Solution pathways**

Number of strings tuned at once

**Polyphonic capability**

**Multiple motors**

Automated / **Manual Strumming**

If manual, one vs multiple strums

Control system to lock in pitch

User Interface / Display?

Maybe, especially if we want the user to be able to set the pitches (alt tuning?)

**Automated** / Manual Motor Control

Flexibility with different guitar brands / headstocks

Probably not

Safety considerations / limitations

Use as a tuner or on-the-fly alternate tuning adjuster

Portability / ease of attaching and removing

Weight?

Battery life?

### Custom PCB stuff

Has to have a processor (stm32 probably)

Breadboard design first

Don’t forget microcontroller programmer

FILL THE Gaps

find which H bridge to use

### 2/11 TA meeting notes

team 1 fall 2022 [charm](https://courses.grainger.illinois.edu/ece445/getfile.asp?id=20868)

label all signals in the system (DC/AC/Wifi, etc)

separate the subsystem requirements
	ensure that the requirements are for the whole subsystem, not a component
	quantative requirements

### Implementation notes pre design review

**High Level Process**

Put piezo on the guitar, need solid connection to accurately get the vibrations
	Make sure piezo's usable range matches the guitar range, not within the resonany frequency area

Put signal through an op-amp with a gain of 20db, then feed to ADC pin of the STM32

Sample at 10 kHz (Nyquist) since highest freq we want is ~5kHz

FFT at max definition we can for our chip

Compare FFT to guitar normal frange frequencies, identify which strings need to go which directions

Send signals to H-bridge to tune

**other considerations**

pick motor carefully to avoid damage to guitar, less than .5 Nm (ideally .1-.3 nM), depends on string tension


### Peer Design Review Notes (2/17)

battery considerations (how long will the battery last?), voltage for our motors / chips? (3.3?), more detail in our subsystem recs

detailed tolerance analysis - temperature?

safety - 445 guidelines - 

### TA meeting 1 - 2/27

Purchasing materials from three places
	Electronic service shop - mainly passive components
 	Supply Center - power cables, jumpers, etc
  	Online order - My.ece - mcmaster or other vendors
   		use vendors already in the system
The rounds of PCB order are not mandatory
	Get whatever one we can order
 Kit 91 18-36-48 Locker J1

### 3D Printing info
[tuning motor connection dimensions](https://www.amazon.co.uk/Guitar-String-Multifunctional-Restringing-Ukulele/dp/B0BW8S5B4G)

###TA meeting 2 3/6
getting the PCB - make account on PCBway and upload the gerber - wait for PCBWAY to approve it and send screenshot of approval to our TA. Also send TA the schematic with the gerber if we're ahead of time and he can double check it.

professor meeting - breadboard demo doesn't have to be complete functonality but at least one subsystem has to be ready

breadboard demo rubric has been uploaded! follow that and pander

proposals have been graded, design doc no specific guidance

for throughholes without footprints we can use a soldering pad and drill a hole through it, PCBway can drill the holes for us


