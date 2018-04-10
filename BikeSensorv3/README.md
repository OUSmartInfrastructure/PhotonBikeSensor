# BikeSensorv3

A Particle project named BikeSensorv3. This project had the goal of creating an image sensor that is low cost, low power, and able to
a remote server, among other goals. It was created by graduate student Riley Engle at Ohio University under the advisment of 
Dr. Julio Arauz, PhD. 

This project has the goal of measuring bike traffic. This is done by connecting an image sensor (VC0706) to a wireless Internet 
enabled device (A particle Photon) and transmitting an image to a server with an installation of TensorFlow, the open-source 
deep learning software from Google. TensorFlow is then able to determine if a bicycle is in the image or not, and allow for the 
counting of a particular category of object. Again, in this case, we are specifically interested in bicycle traffic, but the 
TensorFlow installation is capable of roughly classifying pedestrians, bicycles, and cars.

The sensor works by first detecting movement in the camera image and snapping an image. The image is saved to the SD card for 
manual collection should Internet connectivity not be available. The image is then sent over the Internet to the TensorFlow 
installation, where a pre-trained model is used to identify if a bicycle is in the image. The Photon itself is low power, 
and the additional devices are capable of being supported by batteries or solar panels. Additionally, the sensor is low cost 
compared to other devices attempting to count bikes, cars, or pedestrians.

The Machine Learning element may be a bit much, but this project shows how data collected from a camera can be sent from an IoT
device and used in additional data processing on an external server. The same idea can be accomplished easier by using road 
tubes, IR, or other methods that do not reqire additional processing (for example to tell if a bike or car has gone by) from an 
external server, or be applied to Machine Learning. 

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
Using the supplied code, your Photon, camera, and SD card can be wired up using the Fritzing diagram Found in the repository.

//Add examples of results and images
//Add Python scripts on the server side for receiving data
