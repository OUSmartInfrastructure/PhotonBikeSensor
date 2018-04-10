# BikeSensorv3

A Particle project named BikeSensorv3. This project had the goal of creating an image sensor that is low cost, low power, and able to
a remote server, among other goals. It was created by graduate student Riley Engle at Ohio University under the advisment of 
Dr. Julio Arauz. 

This project has the goal of measuring bike traffic. This is done by placing an image sensor (A VC0706) to a wireless Internet enabled 
device (A particle Photon) and transmitting it to a server with an installation of TensorFlow, the open-source deep learning software 
from Google. 

The sensor works by detecting movement in the camera image, and snapping an image. The image is first saved to the SD card for manual
retrevial. The image is then sent over the Internet to the TensorFlow installation, where a pre-trained model is used to identify if a 
bicycle is in the image. The Photon itself is low power, and the additional devices are capable of being supported by batteries or solar
panels. 

## Parts Used
* Adafruit Weatherproof serial camera (VC0706 image sensor)
* Adafruit SD card breakout board
* Particle Photon
* SD Card
* TensorFlow

## Required Libraries
Note, the Adafruit VC0706 library is not a direct copy, and was modified to work on the Particle Photon. This was a replacement of the 
original SerialSoft and NewSerial libraries to use the ParticleSerialSoft library. All references have to be replaced. 

## How to 

//Add Fritzing Diagram
//Add examples of results and images
