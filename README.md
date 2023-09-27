# Variable-Temperature-Spectral-Radiation-Simulator
**Interactive Simulation of Spectral Radiation from a Blackbody**
 This code is a C program that generates a graphical representation of the spectral radiation of a blackbody as a function of temperature. It displays a curve of radiation intensity versus wavelength and provides temperature and radiation intensity information on a full-color terminal screen.
 **Some of the main formulas that the script uses:**

 **Planck's Law for Blackbody Spectral Radiation Intensity:**
 The formula to calculate the spectral radiation intensity (I) of a black body in relation to wavelength (λ) and temperature (T) is given by:

 ### I(λ, T) = [(2 * π * h * c^2) / (λ^5)] * [1 / (e^[(h * c) / (λ * k * T)] - 1 )]

**Where:**  

**I(λ, T)** is the spectral radiation intensity.  
**h** is Planck's constant.  
**c** is the speed of light.  
**λ** is the wavelength.  
**k** is the Boltzmann constant.  
**T** is the temperature in Kelvin.  

 **Wien's Law for Peak Wavelength:**  
 The formula to calculate the peak wavelength (λ_max) of the radiation emitted by a black body as a function of _temperature (T) is given by:  

*λ_max = b / T*  

**Where:**  
**λ_max** is the peak wavelength.  
**b** is the Wien constant.  

 **Temperature Conversion to Color Table Index:**
 The program maps the temperature to an index in the kelvin_table color table based on temperature ranges. It is not a specific formula, but a correspondence between the temperature and an index in the table.

 **Maximum Spectral Radiation Intensity:**
 The program calculates the maximum spectral radiation intensity (maxIntensity) in a given wavelength range. This maximum intensity is used to normalize the graphical representation.

In summary, this code is an interactive graphical representation of the spectral radiation intensity of a blackbody as a function of temperature, with a colored terminal interface with the ability to adjust temperature at run time. An interesting application to visualize physics concepts and colors related to temperature.
