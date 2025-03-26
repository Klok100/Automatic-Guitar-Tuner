# ECE 445 Lab Notebook

## Week 1

### (1/21/2025)
- Partered with Ethan Lin (ethanl7)
- Started brainstorming Automatic Guitar Tuner idea
  - Initial idea is to use a microphone, hammer, and motor system to be able to detect and automatically tune each guitar string
  - By using a hammer system (similar to a piano), we would be able to detect each string's frequency in real-time and tune based on that without any work required from the user
 
### (1/23/2025)
- Feeback from Professor: How is this automatic guitar tuner differ from the ones already on the market?
- Feedback from TAs: How are we going to differentiate from past ECE 445 Guitar Tuner projects?

## Week 2

### (1/28/2025)
- Began implementing project idea feedback:
  - We can use a vibration sensor instead of a microphone to eliminate outside noise from affecting the tuning process
  - We can tune all 6 strings at once instead of one at a time to make the overall tuning process more efficient

### (1/29/2025)
- Completed Labratory Safety Training
- Concerned about the hammer system as it might be tough to find hammers that can fit on all 6 strings --> Might just have the user strum all 6 strings at once a couple of times but that then requires user action

### (1/30/2025)
- Completed CAD Assignment
- Settled on not using the hammers as it would make the build very awkward to set up --> User instead strum all 6 strings (can be seen as interactive)

## Week 3

### (2/4/2025)
- Began drafting RFA document
  - Our project will have 4 subsystems: the power subsystem, the motor subsystem, the processing subsystem, and the vibration-sensing subsystem
  - We decided to use a Piezo Disc Transducer that will attach to and collect vibrational data from the body of the guitar
  - We need to create a specific algorithm to determine which string corresponds to which output frequency from
  - We want our entire system to run on a 9 volt battery as many guitarists already carry them around to power their guitar pickups
  - Note of Caution: Can't let the motors be too powerful such that they break the guitar (potentially torque-limited?)

### (2/6/2025)
- Finalized the details for the RFA document
  - We want to be able to tune each string to within 12 cents of the intended string frequency
- Current Parts List:
  - Power Subsystem:
    - 9V Battery
    - Step Down Voltage Regulator (LM317T)
  - Motor Subsystem
    - Motors (ROB-11696)
    - H-Bridges (4489)
  - Processinfg Subsystem
    - Microcontroller (TBD)
  - Vibratio-Sensing Subsystem
    - Piezo Disc Transducer (DZS Elec 35mm)
    - Signal Amplifier (LM386N-1)

### (2/7/2025)
- Learned and improved upong soldering skills via the Soldering Assignment

## Week 4

### (2/10/2025)
- Began drafting Project Proposal to prepare for the TA meeting
  - Created Initial Block Diagram: ![ECE 445 Initial Block Diagram](https://github.com/user-attachments/assets/1179d8e5-3d7b-453e-a700-23392906c309)
  - Decided that the guitar will always be tuned to the standard guitar tuning (EADGBE which has a general frequency range of 80Hz - 350Hz)
  - We will use an LED to indicate when the guitar has finished tuning
  - After looking closer at the Piezo Disc Transducer, we need one with a higher resonant frequency than the standard guitar tuning range as to interfere with any vibrational readings
    - Ours have a resonant frequency around 4.6kHz which is well outside the tuning frequency range

### (2/11/2025)
- Had 1st TA Meeting
  - Need to fully label all connections in the block diagram
  - Add a separate section for subsystem requirements that a more qualitative
  - Recommended us to refer to a Fall 2022 Project's Design Document for a good example ([CHARM](https://courses.grainger.illinois.edu/ece445/getfile.asp?id=20868))

### (2/13/2025)
- Added Subsystem Requirements Section to Project Proposal
  - We plan to power all subsystems at a constant 5V
  - Determined that the torque limit for the motors should be <= 0.5 N*m to prevent damage to the guitars
  - We will amplify the signal from the Piezo Disc Transducer by 20dB before sending it to the microcontroller for processing

### (2/14/2025)
- Completed Team Contract
  - Assigned task roles:
    - *Nathan Kim*: Software development, microcontroller programming, frequency analysis (FFT implementation)* 
    - *Ethan Lin*: Hardware design, motor control, circuit design (piezo amplifier, power regulation)*
    - *Shared Tasks*: System integration, testing, debugging, and final documentation
    - *We will split working on FFT implementation and circuit design based on real-time difficulties or challenges encountered along the way as they both are core parts of the project 

## Week 5

## Week 6

## Week 7

## Week 8

## Helpful Links

