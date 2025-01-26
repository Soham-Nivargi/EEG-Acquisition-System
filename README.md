# EEG-EDL-Project-Grp-14
### _This document describes the file organisation for our project - a 16 channel EEG recording system._
### Original problem statement
Primary objective of the device is to record electroencephalogram (EEG) signals from human brain with high fidelity. EEG acquisition is a non-invasive method of capturing brain signals. The device has two major parts, (a) the recording electrodes (sensors) and (b) the data capture electronics. 
In this work, we will work on developing the electronic system to capture multi-channel EEG. The term "channel" corresponds to number of electrodes (or spatial locations on the brain) that we intend to record. The primary requirement is that of recording fidelity in the presence of numerous noise sources. Noise sources in this context, mainly refers to ambient noise, one of them being the 50 Hz power-line interference. The main constraint is the form-factor of the device. 

Over the course of our project, we were forced to pivot from the original plan.

Thus, these files describe two possible implementations of a 16 channel EEG recording system.

    1. Original Plan : 16 Channels - 2 ADS1299 ICs on a single 4-layer PCB
    2. Modified Plan : 16 Channels - 2 breakout boards, each with 2 layers. Each breakout board having one ADS1299.

The files are organised as follows:

### 1. Main PCB files
   
        A. Main PCB schematic files
        B. Main PCB KiCAD project files (layout)
        
 ### 2. Breakout Board PCB files
   
        A. Breakout board PCB schematic files
        B. Breakout board PCB KiCAD project files (layout)
        C. Breakout board gerber files - for final prototype production
        
 ### 3. Intermediate schematics
   (these are building-block schematics that after being made once could be used with modifications in 
    other PCB schematics)

        A. ADS schematic
        B. ESD schematic
        C. Power Circuit schematic
        D. Bluepill schematic
        
 ### 4. Preprocessing and testing
 
        A. Code files for testing
        B. Text files with output of testing

 ### 5. Symbols and footprints 
   (contains the downloaded footprints and symbols used by us in the schematics)

 ### 6.Handwritten documentation
   (analysis of datasheets, lecture notes, discussion with professors and seniors)

 ### 7. CAD files
   (Files for 3D printing the outer casing/asssembly for our interfaced PCB)

    
