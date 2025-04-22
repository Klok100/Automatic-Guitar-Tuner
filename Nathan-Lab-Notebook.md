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
    - 5V Voltage Regulator (LM317T)
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

### (2/17/2025)
- Completed Peer Proposal Review (Automatic Card Shuffler)
- Decided to use the STM32H7B0RBT6 microcontroller ([Datasheet](https://github.com/user-attachments/files/19480645/stm32h7b0rb.pdf)) 
  - Designed for high performance --> important for fast tuning algorithm
  - Includes a full set of DSP instructions
  - Built-in ADC and Motor Control pins
- Updated Project Proposal for the Proposal Review
  - The microcontroller runs at 3.3V so we need both a 3.3V and 5V Voltage Regulator
  - Updated Parts List:
    - Added 3.3 Voltage Regulator (AZ1117CD-3.3TRG1)
    - Replaced motors for a higher torque output (ROB-11696 --> GA12-N20)
  - Added a Battery Analysis of the entire system with the added/updated parts:
    | Component                   | Quantity | Voltage    | Current per Unit | Total Current |
    | --------------------------- | -------- | ---------- | ---------------- | ------------- |
    | STM32 Microcontroller       | 1        | 3.3V       | 100mA            | 100mA         |
    | L298N H-Bridge              | 3        | 5V         | 70mA             | 210mA         |
    | GA12-N20 Motors             | 6        | 5V         | 40mA             | 240mA         |
    | TXJ-055-US Piezo Transducer | 1        | N/A        | N/A              | N/A           |
    | LM386N-1 Amplifier          | 1        | 9V         | 8mA              | 8mA           |
    | LED Indicator               | 2        | 3.3V       | 10mA             | 20mA          |
    | AZ1117CD Voltage Regulator  | 2        | 9V-5V/3.3V | 10mA             | 20mA          |
    - Total Current Draw: 598mA
    - Average 9V Battery: 500mAh
    - Battery Life = $\frac{BatteryCapacity(mAh)}{TotalCurrentDraw(mA)} = \frac{500mAh}{598mA} \approx 0.83 hours$
      - Assuming it takes 1 minute to finish tuning, our system can be used up to 50 times from one 9V battery
  - Updated Block Diagram with full labeling: ![ECE 445 Block Diagram V2](https://github.com/user-attachments/assets/7cecf9ec-2bf3-41f9-a881-44dee75acd75)

### (2/19/2025)
- Completed Professor Proposal Review
  - Verify that readings from the Piezo Disc Transducer are accurate
  - Need to think more about the torque of the motors and how to properly limit it as to not break the guitar

## Week 6

### (2/25/2025)
- Vibration-Sensing Subsystem Experimentation and Testing
  - Tested Piezo Disc Transducer output using Scopy to verify accurate frequency readings
  - Determined that the transducer needs to be placed on the bridge of the guitar as the vibrations are the strongest there
  - Have to rethink the design for how to attach the transducer to the guitar as it can't really just cllp on to the bridge
  - Strumming all 6 strings at once does produce distinct frequency peaks but we need to differentiate between the root notes and harmonics when tuning
- Motor Subsystem Experimentation and Testing
  - Tested the motors to see if they can properly turn the tuning pegs
  - Ethan 3D printed motor-peg attachments to be able to fit around the tuning pegs
  - The current motors we have are too weak to turn the pegs
  - Need to look into stronger motors and may have to adjust subsystem voltage requirements

### (2/27/2025)
- Had 2nd TA Meeting
  - Can order parts through my.ece --> Purchasing Tab
  - Additional Feedback from Professor
    - Need to consider what sampling frequency we will use when reading from the Piezo Disc Transducer
    - What motor RPM would be optimal?
    - Block Diagram is too small/too busy
  - Obtained Kit 91 from Locker J1 (18-36-48)
 
### (2/28/2025)
- Decided to change which voltage regulators based on ECE supplies
  - Updated Parts List:
    - Replaced 3.3V Voltage Regulator (AZ1117CD-3.3TRG1 --> LM2937-3.3)
    - Replaced 5V Voltage Regulator (LM317T --> LM2937)
    - *The updated Battery Analysis remains essentially the same
      
## Week 7

### (3/4/2025)
- Began drafting Design Document
  - Created Subsystem Requirements and Verifications Table
  - Analyzed the Cost Breakdown of the entire project:
  
    | Part Description       | Part Number          | Vendor   | Quantity | Total Cost |
    | ---------------------- | -------------------- | -------- | -------- | ---------- |
    | 3.3V Voltage Regulator | LM2937-3.3           | DigiKey  | 1        | $1.81      |
    | 5V Voltage Regulator   | μA7805               | DigiKey  | 1        | $1.16      |
    | H-Bridge               | L298N                | E-Shop   | 3        | -          |
    | Motors                 | GA12-N20             | Amazon   | 6        | $44.94     |
    | Microcontroller        | STM32H7B0RBT6        | DigiKey  | 1        | $9.31      |
    | Piezo Disc Transducer  | TXJ-055-US           | Amazon   | 15       | $6.99      |
    | Signal Amplifier       | LM386N-1             | Mouser   | 1        | $0.93      |
    | LEDs                   | RED/GREEN LED        | E-Shop   | 2        | -          |
    | Guitar                 | First Act Guitar 222 | Facebook | 1        | $55        |
    | 9V Battery             | Energizer MAX        | Amazon   | 4        | $11.88     |
    | Battery Holder         | 9V-Switch            | Amazon   | 2        | $5.99      |
    
  - Ethan added an analysis of the 3.3V and 5V Voltage Regulators
  - Updated Block Diagram:

    ![ECE 445 Block Diagram V3](https://github.com/user-attachments/assets/16eff804-28c0-459e-9232-37d7f62de796)

### (3/5/2025)
- Picked up actual guitar we will be using for the project
  - Verified that the Piezo Disc Transducers still obtain accurate readings with this guitar
  - Need to adjust physical designs to accomodate the smaller guitar size
- Completed Teamwork Evaluation I
- Began looking into how to program STM32 Microcontroller ([STM32H7 User Guide](https://github.com/user-attachments/files/19483219/STM32H7.User.Guide.pdf))

### (3/6/2025)
- Had 3rd TA Meeting
  - We should upload our PCB files to PCBWay for approval --> Then send the approval screenshot to the TA so he can also verify everything
- Finalized Design Document
  - Decided to use a magnetic mounting system to attach the Piezo Disc Transducer to the bridge of the guitar
  - Added a weekly schedule going forward:
    | Week      | Tasks                                                                                                                                      | Responsibility                           |
    | --------- | ------------------------------------------------------------------------------------------------------------------------------------------ | ---------------------------------------- |
    | 3/3/2025  | 1. Finish Design Document <br /> 2. Sign up for Breadboard Demo <br /> 3. Order all parts for Breadboard Demo                              | 1. Both <br /> 2. Both <br /> 3. Ethan   |
    | 3/10/2025 | 1. Complete Breadboard Design for Demo <br /> 2. Complete PCB Design for PCBWay <br /> 3. Create rough physical design and mounting system | 1. Both <br /> 2. Both <br /> 3. Ethan   |
    | 3/17/2025 | 1. Spring Break                                                                                                                            | 1. Both                                  |
    | 3/24/2025 | 1. Solder PCB <br /> 2. Tuning Algorithm Development <br /> 3. Prototype Motor Subsystem                                                   | 1. Both <br /> 2. Nathan <br /> 3. Ethan |
    | 3/31/2025 | 1. Individual Progress Report <br /> 2. Prototype Vibration-Sensing Subsystem <br /> 3. Prototype Processing Subsystem                     | 1. Both <br /> 2. Both <br /> 3. Both    | 
    | 4/7/2025  | 1. Fine Tune PCB Design <br /> 2. Unit Test All Subsystems <br /> 3. Test Subsystem Integration                                            | 1. Ethan <br /> 2. Both <br /> 3. Nathan |
    | 4/14/2025 | 1. Ensure High Level Requirements are Met                                                                                                  | 1. Both                                  |
    | 4/21/2025 | 1. Present Mock Demo <br /> 2. Implement Feedback from Mock Demo                                                                           | 1. Both <br /> 2. Both                   |
    | 4/28/2025 | 1. Present Final Demo <br /> 2. Give Mock Presentation <br /> 3. Implement Feedbackl from Mock Presentation                                | 1. Both <br /> 2. Both <br /> 3. Both    | 
    | 5/5/2025  | 1. Give Final Presentation <br /> 2. Complete Final Paper <br /> 3. Lab Checkout                                                           | 1. Both <br /> 2. Both <br /> 3. Both    | 

### (3/8/2025)
- Added key STM32 initialization functions
  - GPIO_Init --> For Motor Control and Control Buttons
  - PWM_Init --> For Motor Speed Control
  - ADC_Init --> For Piezo Disc Transducer Frequency Detection
  - UART_Init --> For general debugging features
  - *Currently unable to actually test any of these as our microcontroller still has not come in

## Week 8

### (3/10/2025)
- Began to develop basic tuning algorithm (written initially in Python for easier testing and debugging)
  - Obtained frequency output graphs from the Piezo Disc Transducer via Scopy
  - Given an in-tune guitar, we are able to categorize the frequency of each string when all 6 strings are strummed by filtering out the harmonics within a set tolerance value <br />
    <img width="463" alt="ECE 445 Python Script Output" src="https://github.com/user-attachments/assets/78c536eb-6195-4d7f-be35-e06daa009999" />
  - Still need further testing for out-of-tune guitars, especially if neighbor strings interfere with each other when they are severly out-of-tune
  - Full Script Available Here: [ECE 445 Frequency Analysis Script](https://colab.research.google.com/drive/1fa643Y6XStVupCan_778ElVVapWFxAj5?usp=sharing)

### (3/11/2025)
- Could not finish Breadboard Demo as our microcontroller arrived too late to attempt to program

## Helpful Links

### Datasheets
- Power Subsystem
  - 3.3V Voltage Regulator: [LM2937-3.3](https://rocelec.widen.net/view/pdf/oggg0ilb2c/NATLS20760-1.pdf?t.download=true&u=5oefqw%5D)
  - 5V Voltage Regulator: [μA7805](https://www.ti.com/lit/ds/symlink/ua78.pdf)
- Motor Subsystem
  - H-Bridge: [L298N](https://www.st.com/resource/en/datasheet/l298.pdf)
- Processing Subsystem
  - Microcontroller: [STM32H7B0RBT6](https://www.st.com/resource/en/datasheet/stm32h7b0rb.pdf)
- Vibration-Sensing Subsystem
  - Signal Amplifier: [LM386N-1](https://www.ti.com/lit/ds/symlink/lm386.pdf)

### Microcontroller Guides 
- [STM32H7B0xB Datasheet](https://github.com/user-attachments/files/19664395/STM32H7B0RB.Datasheet.pdf)
- [STM32 Arduino IDE Setup](https://community.st.com/t5/stm32-mcus/how-to-program-and-debug-the-stm32-using-the-arduino-ide/ta-p/608514)
- [JTAG Pinout](https://developer.arm.com/documentation/dui0499/d/ARM-DSTREAM-Target-Interface-Connections/ARM-JTAG-20)
- [STM32 Boot Mode](https://www.st.com/resource/en/application_note/an2606-stm32-microcontroller-system-memory-boot-mode-stmicroelectronics.pdf)
- [STLINK-V3-MINIE User Guide](https://www.st.com/resource/en/user_manual/dm00813040.pdf)
- [STM32H753 Datasheet](https://www.st.com/resource/en/datasheet/stm32h753vi.pdf)
- [STM32H7 Nucleo Board User Manual](https://www.st.com/resource/en/user_manual/um2407-stm32h7-nucleo144-boards-mb1364-stmicroelectronics.pdf)
- [STM32H7 Nucleo Board Schematic](https://www.st.com/resource/en/schematic_pack/mb1364-h753zi-c01_schematic.pdf)
- [STM32H753 User Manual](https://www.st.com/resource/en/reference_manual/rm0433-stm32h742-stm32h743753-and-stm32h750-value-line-advanced-armbased-32bit-mcus-stmicroelectronics.pdf)
- [STM32 Hello World Tutorial Guide](https://youtu.be/8S78Ih4SaiE?si=6Xj7R8O0bKv14Vj-)
- [STM32 UART Guide](https://youtu.be/ttzu-j77jPg?si=z0fbmpthiJIxOkbx)
- [STM32 ADC Single-Input Mode Guide](https://youtu.be/q2R8jqOQuj8?si=JdF-AFtofjSvF06V)
- [STM32 ADC Differential Mode Guide](https://youtu.be/E47Alkv0Uko?si=PrPSLkQyITmeg1vr)

### Similar Projects

### Misc
