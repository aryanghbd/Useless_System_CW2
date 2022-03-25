## Classes


# Notes




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

```
Function Name:
Inputs:
Outputs:
Function Description:
```
