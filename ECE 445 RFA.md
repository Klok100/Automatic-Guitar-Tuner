# Automatic Guitar Tuner

Team Members:
- Nathan Kim (nrkim2)
- Ethan Lin (ethanl7)

# Problem
For many guitar players, keeping their guitar in tune or wanting to change the tuning of their guitar can be a hassle. Looking at the tuners currently on the market, the most common type of guitar tuner is a clip-on tuner where the player is required to manually tune each string using the attached tuner as a pitch guide. There also exist automatic guitar tuners but these are  limited by either the number of strings that can be tuned at once, the price of the tuner, or the amount of work needed to be done by the player (i.e. the player still has to move the tuner around the pegs or strum the strings).

# Solution
Our solution is to develop an automatic guitar tuner that attaches to all six tuning pegs of the guitar and can tune each string to a pitch that is set by the user. So, the user will intermittently strum all six strings until an LED flashes which indicates that all strings are correctly tuned to whatever has been set by the user - an attached Piezo Disk Transducer will be used to determine the real-time frequencies and vibrations within the guitar. To accomplish this overall task, we will introduce 4 essential subsystems: a power subsystem, motor subsystem, processing subsystem, and a vibration-sensing subsystem. 

# Additional Notes
This project draws inspiration from projects in SP 1999 and SP 2020, both being automated guitar tuners. The 2020 project features a fully automated system, including strumming, for three strings at a time. Our solution differs from this project by tuning all six strings at once, utilizing a Piezo Disk Transducer instead of a microphone system to determine the pitch of the strings. As a result, our solution will be more noise-resistant, especially when tuning in more chaotic environments, as it will be based on vibrational pitch sensing. In addition, our solution will be safer, limiting the motor strength so as to not break the string, and will also include a more robust user interface, allowing the user to set the pitch of each string within a set range. 

# Solution Components

## Subsystem 1: Power System
The power system will provide power for the motors and processing system. As the design will be portable, it will be run from a 9V battery (233) and require a step down voltage regulator (LM317T) to get the power to an acceptable level for our motor and processing systems.

## Subsystem 2: Motor System
The motor system will be responsible for turning the tuning pegs based on the processing system output. The motors (ROB-11696) will be driven by H-bridges (4489) on the PCB and will have limited torque and power in order to ensure the system will not damage the guitar.

## Subsystem 3: Processing System
The processing system is the heart of the project, as it will take input from the vibration system, distinguish between all six strings, process which direction to tune each string, and finally send out power to the motor system to tune the guitar.

## Subsystem 4: Vibration-Sensing System
This system will take input from a piezo disk transducer (DZS Elec 35mm Piezo Disc Transducer Contact Microphone Trigger Sound Sensor with 4 Inches) and amplify (LM386N-1 Digikey 296-44414-5-ND) it to an acceptable level for the processing system to handle. This system may also take input from multiple transducers and combine them for a more accurate and reliable input. 

# Criterion For Success
Our criterion for success is to be able to identify the pitch of each string from a series of strums. From here, our solution should be able to tune all six strings within ±12 cents of the set tone per string (this is said to be the value where people can start to detect when something is out-of-tune). In addition, the entire tuning process should finish within a minute.
