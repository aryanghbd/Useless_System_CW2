## Classes

# Knob
This class handles knob rotations and presses. All variables are private.

Public Functions
```
Function Name: Knob (initializer)
Inputs: int rotationIncrement, int rotationMin=0, int rotationMax=16, int knobRotation = 0, bool initiallyToggled = false
Outputs: None
Function Description: initializes Knob object. knobRotation is the initial value of the knob.
```
```
Function Name: addKnobRotation
Inputs: int
Outputs: None
Function Description: increments or decrements knob's value by given int
```
```
Function Name: updateRotation
Inputs: uint8_t
Outputs: None
Function Description: input is two bits representing the BA bits in the lab instructions.
```
```
Function Name: setPressed
Inputs: uint8_t
Outputs: None
Function Description: input is bit representing the knob's pushing down bit.
```
```
Function Name: getRotation
Inputs: None
Outputs: int
Function Description: returns value of knob
```
```
Function Name: getPressed
Inputs: None
Outputs: bool
Function Description: returns true if the knob is being pressed down
```
```
Function Name: getToggled
Inputs: None
Outputs: bool
Function Description: returns if the knob is enabled (switches every time knob is clicked)
```
```
Function Name: getToggledStr
Inputs: None
Outputs: String
Function Description: returns 'Y' if getToggled is true, else returns 'N'
```

# Notes
This class stores the step sizes and notes and returns them.
```
Function Name: getStep
Inputs: int octave, int note
Outputs: int32_t
Function Description: Returns stepsize corresponding to the octave and note
```
```
Function Name: getNote
Inputs: int
Outputs: String
Function Description: Returns note as a letter
```


## Tasks Performed – implementation, threads 

The synthesiser plays the appropriate musical tone as a sawtooth wave when a key is pressed, which has been implemented using step size for a phase accumulator.  

The column matrix is read by leveraging the HAL from the board in order to read the various bit-statuses of the individual pins; to this end, we were able to digitally read all 4 columns, the read then accumulates the 4 bits and right shifts them so as to present the bit configurations in an active low, 4-bit nibble format. Thus, a reading of 1011 is indicative of the second key from the left being pressed. Separate methods then enabled a full range of selection through combinational assertions and de-assertions of multiplexer bits in order to select the appropriate rows to extrapolate this behaviour for the other quartets of keys. The 4-bit nibbles are displayed in Hexadecimal format, so the above example would be displayed on the piano-display as B.  

In the high-frequency loop, the MUX selectors increment repeatedly between 0 and 6 in order to select the 3 rows of the key-matrix and displays their state on the main screen cascaded with each other in order to record the state of all 12 keys in a given instant. 

To produce sound, the piano was initially tuned with a target frequency of a 440Hz A frequency, using the aforementioned reference along with provided formulas, step sizes to produce the relevant note frequencies wave-forms were generated in testing, and then stored in a size-12 array – these values were to be fed to an interrupt service routine which would produce the associated analogue waveform corresponding to the input, giving the sound of the desired frequency and note. To achieve this, an atomic routine was leveraged in order to ensure an indivisible and uninterruptable selection of the current step-size based on the individual note pressed, the associated position in the step-size array is fetched and relevant note displayed on-screen. 

Although we have hardcoded the MUX enforcement for stability, there should be no perceptible delay between the pressing of the key and the tone starting. 

The third knob is the volume control know where there are 17 (0 to 16) levels of volume. The functionality of knob is located in the Knob class. Each knob generates two signals, A  and B, that indicate a change in state when rotated. The changes between the current and  previous states are detected and the corresponding bits can then be compared to the state  transition table in order to identify the direction the knob is being rotated. These are  decoded using if-else statements in Knob::updateRotation(), the range is limited to 0 and 16  in Knob::addKnobRotation, and the volume is incremented by 2 for each notch clockwise or  decremented for anticlockwise.  

OLED display shows the name of the note being played alongside the current volume. The  volume is obtained using knob3.getrotation to see what volume the rotation of the knob  corresponds to and this is printed. There is an array of the same dimension as the step sizes,  and when the key is pressed and stepsize accessed in array, the same index is used to pull a  char with its note, we u8g2 display that to screen, ‘set a ‘currentnote = notes[key]’. This has  been implemented in the display update thread. The display also shows the number  corresponding to the key that has been pressed, the current octave, and whether the key is  currently pressed or released. In the scankeystask function the press and release is detected  using the stepsize, where if the stepsize is 0 it is detected as a release.   

const String notes[12] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#",  "A", "A#", "B"}; 
Every 100ms the OLED display refreshes and the LED LD3 on the MCU toggles – no clue wtf  this means but its on the specification  
The synthesiser can be configured to be a sender of receiver – the scankeystask function  contains the code to configure as a sender and the receiver is in the displaytask function.  
When a CAN message is received, it is placed in a queue where the decode block handles  the messages. The CAN message is stored in RX\_Message array where the decode thread  plays the notes in response to this.  
There is also a queue for messages to be transmitted, the transmit thread uses a sephamore  to check when messages can be moved from the queue to the outgoing message box. The  sephamore is also used as a guard access to these messages.  
The scanKeys  function takes  154us to run on average,  and the displayUpdate takes  17430us to run on average.  
A semaphore is used to lock access to data whilst it’s being accessed by a thread. This  method is used to guard access to the keyValue array, as well as RX\_message when the  display is being updated.   
