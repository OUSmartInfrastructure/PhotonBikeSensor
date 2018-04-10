#include <Adafruit_VC0706.h>
#include <SPI.h>
#include <SdFat.h>
#include <ParticleSoftSerial.h>

// Setup SPI configuration
#define SPI_CONFIGURATION 0
// Primary SPI with DMA
// SCK => A3, MISO => A4, MOSI => A5, SS => A2 (default)


//Declarations
ParticleSoftSerial cameraconnection = ParticleSoftSerial(D2, D3);
Adafruit_VC0706 cam = Adafruit_VC0706(&cameraconnection);
SdFat sd;
const uint8_t chipSelect = SS;
String logFile = "sensor.log";
//cameraDelay = 0;

//TCP Setup
TCPClient client;
byte server[] = {#,#,#,#}; // Local machine
int port = #;

void setup() {
    File myFile;
    pinMode(chipSelect, OUTPUT); // SS on Photon
    //SPI.setClockSpeed(4,MHz);
    SPI.begin();

    Serial.begin(9600);

    while (!Serial) {
        SysCall::yield();
    }
//Press a button to start setup
// This is being used for debug now, but will be disabled or removed
    while (Serial.read() <= 0) {
        SysCall::yield();
    }
    delay(1000);

//------------------------------------------------------------------------------
    //Added extra cam.begin() to allow the second one to return true
    //Unsure why this is needed (3/12/18)
    cam.begin();

//Try to locate the camera
        if (cam.begin()) {
          Serial.println("Camera Found:");
        } else {
          Serial.println("No camera found?");
          //return;
        }
//Check for SD card
        delay(1000);
        Serial.println("Initializing SD Card");
        if (!sd.begin(chipSelect, SPI_FULL_SPEED)) {
            sd.initErrorHalt();
        }

        delay(1000);
        cam.setImageSize(VC0706_640x480);        // biggest
        cam.setMotionDetect(true);           // turn it on
        cam.setCompression(99);

//Start write to SD card by opening file
        if (!myFile.open(logFile, O_RDWR | O_CREAT | O_AT_END)) {
            sd.errorHalt("opening sensor.log for write failed");
        }
//If the file opened okay, write to it:
        Serial.println("Writing to sensor.log ...");
        myFile.printf("Startup complete: ");
        myFile.println(Time.timeStr() + "\n");
//Close the file:
        myFile.close();
        Serial.println("Startup complete");
        Serial.println(Time.timeStr());

        Serial.println(WiFi.localIP());
        delay(50);
        //client.connect(server,51234);
}


void loop() {
    File myFile;

    if (cam.motionDetected()) {
       Serial.println("Motion!");
//Turn off motion detect to keep buffer for saving image
       cam.setMotionDetect(false);
        
      //delay(cameraDelay) // To allow subjust to move a bit mroe into frame, untested    
      if (! cam.takePicture())
        Serial.println("Failed to snap!");
      else
        Serial.println("Picture taken!");

      char filename[13];
//Currently allows for up to 99 photos to be saved to SD
      strcpy(filename, "IMAGE00.JPG");
      for (int i = 0; i < 100; i++) {
        filename[5] = '0' + i/10;
        filename[6] = '0' + i%10;
//Create if does not exist, do not open existing, write, sync after write
        if (! sd.exists(filename)) {
          break;
        }
      }
//Print info to log file
      myFile.open(logFile,FILE_WRITE);
      myFile.printf("Picture taken: ");
      myFile.printf(filename);
      myFile.printf(" ");
      myFile.printf(Time.timeStr());
      myFile.println();
      myFile.close();

      File imgFile = sd.open(filename, FILE_WRITE);

      uint16_t jpglen = cam.frameLength();
      Serial.print(jpglen, DEC);
      Serial.println(" byte image");

      Serial.print("Writing image to "); Serial.print(filename);
      int32_t time = millis();
//While there is still data to read...
      while (jpglen > 0) {
// read 96 bytes at a time;
        uint8_t *buffer;
        uint8_t bytesToRead = min(96, jpglen); // change 32 to 64 for a speedup but may not work with all setups!
        buffer = cam.readPicture(bytesToRead);
//Write the image
        imgFile.write(buffer, bytesToRead);
        jpglen -= bytesToRead;
      }

      imgFile.close();
      Serial.println("...Done!");
      Serial.println("Sending file to server...");

      myFile.open(filename, O_READ);
      int data;
      long int now = millis();
      if (client.connect(server,port)){
          Serial.println("Connected!");
          while ((data = myFile.read()) >= 0){
              client.write(data);
          }
          client.stop();
      }
      long int sendTime = millis() - now;
      Serial.printlnf("Time to write was: %d",sendTime);
      //Particle.publish("Picture-Taken",filename);


      time = millis() - time;
      client.println(time);
      Serial.println("done!");
      Serial.print(time); Serial.println(" ms elapsed");
      cam.resumeVideo();
      cam.setMotionDetect(true);
     }
}
