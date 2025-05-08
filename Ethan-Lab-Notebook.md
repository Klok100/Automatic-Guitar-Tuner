# Week 1, 1/20
## 1/21
- Partnered with Nathan Kim
- Initial Web Board post
	> As a new guitar player, keeping my guitar in tune can be a hassle. Although there are many clip-on guitar tuners currently on the market, they still require the player to manually tune each string, using the attached tuner as a pitch guide. Our idea is to create a fully automatic guitar tuner can attaches to the head of the guitar and is able to tune each string to a set pitch/note. It will use a microphone, hammer, and motor system to determine a string's frequency in real time and adjust each string without any work required from the user. (Nathan Kim 1/21)
- Consider challenges of different brands having different headstock shapes
## 1/22 
- Prof Oelze Comment 
	> There are many automatic guitar tuners on the market already. How would you differentiate from them? Do you have some innovation to the existing solutions?
- Partnered with Nathan to find Initial Research into Market
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
	- Gained familiarity with PCB layout, placements, traces
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
- Discussed with Nathan and Split roles into Hardware (me) and Software (Nathan)
## 2/6 
- Project Approved! 
- Response to Prof Oelze
	- Our transducer is extremely small compared to the mass of the guitar, so there is negligible change in the sound when it is attached vs taken off
	- Harmonics can be addressed through a simple harmonic reduction algorithm since harmonics are integer multiples of the fundamental frequency
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
- Nathan Created a Block Diagram and started work on the Tuning Algorithm
![initbl](https://github.com/user-attachments/assets/e3138cf9-7a8d-469c-9454-604c1eef953b)

## Custom PCB stuff

Has to have a processor (stm32 probably)

Breadboard design first

Don’t forget microcontroller programmer

find which H bridge to use

## 2/13 [Project Proposal!](https://docs.google.com/document/d/1nwYWKY9DeY5X0FPsXHrTtyqf1Pt8vyyrZwkM8g2xa2k/edit?usp=sharing)
- Visual Overview (made by me)
	- ![visov](https://github.com/user-attachments/assets/5412fd99-4717-40c9-9c53-1bb083ca9661)
	- Seperated the project into Power, Motor, Processing, Vibration-Sensing Subsystems
Created CAD mockup of project
- Headstock mounted power, motor, and processing units
- ![inticadhead](https://github.com/user-attachments/assets/68bed91c-eee8-4dcc-88a9-0aa5b2bb8647)
- Sensor mounted at neck of guitar
- ![initcadsens](https://github.com/user-attachments/assets/f1df78ad-22c6-4460-babe-5ab94501eb90)
- Performed a battery analysis to determine battery life, found a 50 minute lifetime
![bms](https://github.com/user-attachments/assets/06f885b4-ba77-4ccf-8793-92cdb1e346c7)
$Battery Life=\frac{Battery Capacity (mAh)}{Total Current Draw (mA)}=\frac{500mAh}{600mA}=\text{0.83 hours or 50 minutes}$
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
# 2/28
Explored motor options
	- [Antrader 30 RPM DC 6V](https://www.amazon.com/Antrader-Electric-GA12-N20-Deceleration-Reduction/dp/B07FYBQ7Z4)
	- [Greartisan DC 12V 250RPM](https://www.amazon.com/Reversible-Reduction-Electric-15-200RPM-Diameter/dp/B07D28H965/)
	- [DC 12V Reversible High Reduction](https://www.amazon.com/Greartisan-Self-Locking-Reversible-Reductioon-Electric/dp/B07YBXF7XW/)
	- [L298N Motor Drive Controller Board](https://www.amazon.com/Qunqi-Controller-Module-Stepper-Arduino/dp/B014KMHSW6/)
	- [Greartisan DC 12V 120RPM Turbo Worm](https://www.amazon.com/Greartisan-120RPM-Turbine-Reduction-JSX69-370/dp/B071XG53B4/)
# Week 7, 3/3
## 3/4
Started mocking up part that connects motors to tuning peg
- ![mini](https://github.com/user-attachments/assets/3921e01f-b5db-4d86-a799-34e9f767ea4b)
- This part was a testing part to fit the N20 shaft of the motors
- Through developing this part I found that the N20 motor did not have enough torque, so I looked further into the motor optiones listed above

Nathan started taking the sensor data that I measured and testing the tuning algorithm on it
## 3/5 
Performed linear regulator analysis, found we needed larger linear regulators as the current ones would overheat
![3 3](https://github.com/user-attachments/assets/3fbb2f2d-441b-4d36-a879-92ad642ea2cb)
![5](https://github.com/user-attachments/assets/f7888884-107d-41c7-9a4b-41e3fbeb8882)
## TA meeting 3/6
getting the PCB 
- make account on PCBway and upload the gerber
- wait for PCBWAY to approve it and send screenshot of approval to our TA
- Also send TA the schematic with the gerber if we're ahead of time and he can double check it.

professor meeting - breadboard demo doesn't have to be complete functonality but at least one subsystem has to be ready

breadboard demo rubric has been uploaded! follow that and pander

proposals have been graded, design doc no specific guidance

for throughholes without footprints we can use a soldering pad and drill a hole through it, PCBway can drill the holes for us

# Week 8, 3/10
## 3/11
Created initial 3D part for the magnet mount
- ![magmounttall](https://github.com/user-attachments/assets/ed30cf15-c8b7-4d2b-baf0-d2faf84969ac)
## 3/12 
Started PCB Layout and Design, followed recs from data sheets for decoupling caps and these videos for setup
- Phil Lab Mixed Signals (https://www.youtube.com/watch?v=AxEL3eESL9I)
- Phil Lab Mixed Signals (https://www.youtube.com/watch?v=v6fTa6LRJLI)
- Phil Lab KiCAD (https://www.youtube.com/watch?v=14_jh3nLSsU)
# Week 9, 3/17 - Spring Break	
# Week 10, 3/24
## 3/25 
- PCB submitted for wave 3
- ![pcb1](https://github.com/user-attachments/assets/fa688f77-66eb-4933-b57a-0efec8930ff1)
## 3/27
- Made magnet mount shorter to allow for a better angle for the sensor
# Week 11, 3/31
- PCB submitted for wave 4
- ![pbc2](https://github.com/user-attachments/assets/7e9ad16c-24c8-486a-a1c3-de5e51cf142b)
	- Major changes include: reducing size and making parts more compact, moving capacitors and traces to allow for better versatility 
- Finished 3D modeled Parts
- ![Screenshot 2025-05-08 133139](https://github.com/user-attachments/assets/6816b94b-b889-46d8-9e39-367a3a55944b)
## 4/1 
- [Individual Progress Report Due](https://docs.google.com/document/d/1xCeTGyqa3WRrXZtf5XI8WRg5vLMVNV7AbIkBLHfD1d0/edit?usp=sharing)
- Low Pass Filter for piezo sensor 

$F_C=\frac{1}{2\pi RC}=\frac{1}{2\pi * (10000 Ohms)*(16 nF)}=994.72 Hz$

- Voltage Divider for Piezo sensor to prevent saturation of MCU pin

$V_{out}=V_{in}*\frac{R1}{R1+R2}=5V(\frac{20 kOhms}{10 kOhms + 20 kOhms})=3.33 V$

- Performed verification on the power subsystem, Motor Subsystem, and Vibration Subysystem
- Nathan continues to work on the tuning algorithm, connecting it to the Dev board and testing the FFT
# Week 12, 4/7
## 4/9 
Started Assembly of parts
- ![assemb](https://github.com/user-attachments/assets/9b323bd0-8b1c-45e1-a7b1-0a6c9c9818b0)

Verification of Amplifer before we realized it was unnecessary

![amp](https://github.com/user-attachments/assets/522377db-02c1-43a1-b5b2-974be1bb71fd)
### 3D Printing info
[tuning motor connection dimensions](https://www.amazon.co.uk/Guitar-String-Multifunctional-Restringing-Ukulele/dp/B0BW8S5B4G)

### parts files and order 4/10 for PCB round 4
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
## 4/16
Removed signal amplifier as the processing subsystem was more accurate without it (added noise)
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
- ![phys](https://github.com/user-attachments/assets/8d750beb-f023-4528-a9ed-323dc0a35de0)
- Performed RVs for the remaining subsystems
- Nathan has a roadblock in FFT 
- Performed verifications on all subsystems as possible
# Week 15, 4/28
- Created [Mock Presentation Slides](https://docs.google.com/presentation/d/1YZ9aDIgzMchjUdUq0ANk6EUFnHSx6i4nLlUX_31I9No/edit?usp=sharing)
- Nathan Solves FFT roadblock, processing subystem is fully functional on the dev board 
# Week 16, 5/5
- Created [Final Presentation Slides](https://docs.google.com/presentation/d/1uwvnbpcpJETUpdRG_x2eOsW6RKY6nreKlF5R9wPtcf8/edit?usp=sharing)
- Submitted Final Paper

Full PCB Schematic:
![full](https://github.com/user-attachments/assets/b05e7a1e-4b8b-48e2-a9a3-3cb5381f6d97)




