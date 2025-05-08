# Week 1, 1/20
## 1/21
- Partnered with Nathan Kim
- Initial Web Board post
	> As a new guitar player, keeping my guitar in tune can be a hassle. Although there are many clip-on guitar tuners currently on the market, they still require the player to manually tune each string, using the attached tuner as a pitch guide. Our idea is to create a fully automatic guitar tuner can attaches to the head of the guitar and is able to tune each string to a set pitch/note. It will use a microphone, hammer, and motor system to determine a string's frequency in real time and adjust each string without any work required from the user. (Nathan Kim 1/21)
- Consider challenges of different brands having different headstock shapes
## 1/22 
- Prof Oelze Comment 
	> There are many automatic guitar tuners on the market already. How would you differentiate from them? Do you have some innovation to the existing solutions?
- Initial Research into Market
	- [Tronical](https://www.tronicaltune.net/tronicaltune-plus/) - Custom frets and tuning attached to headstock, display for each string
	- Robot Guitar
	- Axcent - button/foot switch triggered, can switch tunings on the fly, bridge
	- Tune-o-matic - bridge 
# Week 2, 1/27
## 1/28
- Initial Research into Past Projects
	- SP 2020 team 71 - Three strings at a time with automated strumming
	[Proposal](https://courses.grainger.illinois.edu/ece445/getfile.asp?id=16908), [Design Doc](https://courses.grainger.illinois.edu/ece445/getfile.asp?id=18170),  [Final](https://courses.grainger.illinois.edu/ece445/getfile.asp?id=18265)
	- FA 2014 team 10 - User interface, one at a time, user plucked, manual motor control
	[Design Review](https://courses.grainger.illinois.edu/ece445/getfile.asp?id=6275)
	- SP 2012 team 14 - UI, auto pluck, all six motors on same side, mounted to board
- Design Considerations
	- Aim for something simple with longevity
	- Use for alternate tunings and not just tuning
	- Tremolo/vibrato functionality?
	- Polyphonic?
## 1/31
- CAD assigment due
	- Gained familiarity with PCB layout, placements, tracing 
# Week 3, 2/3
## 2/5
- Prof Oelze Comment
	> If you are using contact transducers, you will likely affect the sound coming off of the guitar by loading the guitar string with the transducer. When you remove the contact transducers, then the sound will be different from what was perceived during tuning. How will you get around this effect? Or if you are placing a contact transducer on the body of the guitar, how will you differentiate the pitch. There will be harmonics and other interference in the signals from all of the strings. I am a little unclear about the vibration sensing and signal processing aspects of this idea.
- Design Decisions and Further Considerations
	- Automated / **Manual Strumming**
	- If manual, one vs multiple strums
	- Control system to lock in pitch
	- User Interface / Display?
	- Maybe, especially if we want the user to be able to set the pitches (alt tuning?)
	- **Automated** / Manual Motor Control
	- Flexibility with different guitar brands / headstocks
	- Safety considerations / limitations
	- Use as a tuner or on-the-fly alternate tuning adjuster
	- Portability / ease of attaching and removing
	- Weight
	- Battery life
## 2/6 
- Project Approved!
# Week 4, 2/10
## 2/10
- Soldering Assignment Submitted
	- Gained familiarity with ECE445 Lab Equipment
## 2/11 TA meeting notes

Good Example: team 1 fall 2022 [charm](https://courses.grainger.illinois.edu/ece445/getfile.asp?id=20868)

label all signals in the block diagram (DC/AC/Wifi, etc)

separate the subsystem requirements
- ensure that the requirements are for the whole subsystem, not a component
- quantative requirements

## Custom PCB stuff

Has to have a processor (stm32 probably)

Breadboard design first

Don’t forget microcontroller programmer

find which H bridge to use

## 2/13 Design review
Created CAD mockup of project
- Headstock mounted power, motor, and processing units
- image
- Sensor mounted at neck of guitar
- image
# Week 5, 2/17
## Peer Design Review Notes (2/17)

battery considerations (how long will the battery last?), voltage for our motors / chips? (3.3?), more detail in our subsystem recs

detailed tolerance analysis - temperature?

safety - 445 guidelines - 

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
# Week 6, 2/24
## TA meeting - 2/27

- Purchasing materials from three places
	- Electronic service shop - mainly passive components
	- Supply Center - power cables, jumpers, etc
	- Online order - My.ece - mcmaster or other vendors
		- Use vendors already in the system

The rounds of PCB order are not mandatory

- Get whatever one we can order

Kit 91 18-36-48, Locker J1

# Week 7, 3/3
## 3/4
Started mocking up part that connects motors to tuning peg
## TA meeting 3/6
getting the PCB - make account on PCBway and upload the gerber - wait for PCBWAY to approve it and send screenshot of approval to our TA. Also send TA the schematic with the gerber if we're ahead of time and he can double check it.

professor meeting - breadboard demo doesn't have to be complete functonality but at least one subsystem has to be ready

breadboard demo rubric has been uploaded! follow that and pander

proposals have been graded, design doc no specific guidance

for throughholes without footprints we can use a soldering pad and drill a hole through it, PCBway can drill the holes for us
# Week 8, 3/10
## 3/11
Created 3D parts:
## 3/12 
Started PCB Layout and Design
# Week 9, 3/17 - Spring Break	
# Week 10, 3/24
## 3/25 
- PCB submitted for wave 3
- Image wave 3
# Week 11, 3/31
- PCB submitted for wave 4
- Image wave 4
# Week 12, 4/7
## 4/9 
Started Assembly of parts
- Image assembly
### 3D Printing info
[tuning motor connection dimensions](https://www.amazon.co.uk/Guitar-String-Multifunctional-Restringing-Ukulele/dp/B0BW8S5B4G)

### parts files and order 4/10
Parts needed
Resistors
2 x 2.2 k (eshop)

5 x 10 k (eshop)

1 x 20 k ([dKey](https://www.digikey.com/en/products/detail/panasonic-electronic-components/ERA-6AEB203V/1465780))

3 x 100 k (eshop)

1 x 1 M (eshop)

Caps

1 x 10 nF ([dKey](https://www.digikey.com/en/products/detail/samsung-electro-mechanics/CL21B103KBANNNC/3886673))

1 x 15 nF ([dKey](https://www.digikey.com/en/products/detail/kemet/C0805C153K5RACTU/411160))

10 x 100 nF (eshop)

1 x 0.33 uF ([dKey](https://www.digikey.com/en/products/detail/samsung-electro-mechanics/CL21B334KBFNNNE/3886781))

3 x 0.1 uF (eshop)

2 x 1 uF (eshop)

1 x 10 uF (eshop)

Diodes

1 x 5V Zener Diode_THT:D_DO-41_SOD81_P10.16mm_Horizontal (1N4733A, C7 D2 self)

1 x MBR0520 Diode_SMD:D_SOD-123 ([dKey](https://www.digikey.com/en/products/detail/onsemi/MBR0520LT1G/918574))

24 x 1N5817 Diode_THT:D_DO-41_SOD81_P10.16mm_Horizontal (C7 D4 self)

Connectors

10 x Connector_Molex:Molex_KK-254_AE-6410-02A_1x02_P2.54mm_Vertical ([dKey](https://www.digikey.com/en/products/detail/molex/0022232021/26667))

1 x 2x3 connector for Boot0 ([dKey](https://www.digikey.com/en/products/detail/molex/0901301206/760959))

1 x Conn_ARM_JTAG_SWD_10 Connector_IDC:IDC-Header_2x05_P2.54mm_Vertical ([dKey](https://www.digikey.com/en/products/detail/on-shore-technology-inc/302-S101/2178422))
# Week 13, 4/14
### 3D Modeling Notes 4/17

Guitar model is fairly accurate to our guitar, except a barebones version and a non-angled headstock  

The dowel rod is used to hold the sensor in place and to connect the motors

The magnet mount allows for an easily removable sensor mount

The motor part is the high torque one that is able to move the tuning pegs

The motor-dowel part is the connectors between the dowel rod and the motors

The motor-tuningpeg is the current part that transmits the motor force to the tuning pegs

- motor-tuning-mini is a smaller version that will print faster

- N20shaft_sizing was used to find the shaft size of the original small motor

The tuningpeg part is the model of only the tuning peg of the guitar

PCB 1 is 4.5" x 4.65"

PCB 2 is 4" x 4.2" 

# Week 14, 4/21
- Finished physical assembly
# Week 15, 4/28
- Created Mock Presentation Slides
# Week 16, 5/5
- Created Final Presentation Slides
- Submitted Final Paper




