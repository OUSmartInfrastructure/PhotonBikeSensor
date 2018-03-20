#include <Adafruit_VC0706.h>
#include <SPI.h>
#include <SdFat.h>
#include <ParticleSoftSerial.h>

// Setup SPI configuration
#define SPI_CONFIGURATION 0
// Primary SPI with DMA
// SCK => A3, MISO => A4, MOSI => A5, SS => A2 (default)
SdFat sd;
const uint8_t chipSelect = SS;


ParticleSoftSerial cameraconnection = ParticleSoftSerial(D2, D3);

Adafruit_VC0706 cam = Adafruit_VC0706(&cameraconnection);

int cameraLED = D5; // Blue


//TCP Setup
TCPClient client;
byte server[] = { 192, 168, 1, 104}; // Local Phone

void setup() {
    File myFile;
    pinMode(chipSelect, OUTPUT); // SS on Photon
    //SPI.setClockSpeed(4,MHz);
    SPI.begin();

    Serial.begin(9600);

    while (!Serial) {
        SysCall::yield();
    }

    while (Serial.read() <= 0) {
        SysCall::yield();
    }
    delay(1000);

//------------------------------------------------------------------------------
    //Added extra cam.begin() to allow the second one to return true
    //Unsure why this is needed (3/12/18)
    cam.begin();

    // Try to locate the camera
        if (cam.begin()) {
          Serial.println("Camera Found:");
        } else {
          Serial.println("No camera found?");
          //return;
        }
        // Check for SD card
        delay(1000);
        Serial.println("Initializing SD Card");
        if (!sd.begin(chipSelect, SPI_FULL_SPEED)) {
            sd.initErrorHalt();
        }


        delay(1000);
        cam.setImageSize(VC0706_640x480);        // biggest
        cam.setMotionDetect(true);           // turn it on
        cam.setCompression(99);




        //Start write to SD card
        if (!myFile.open("sensor.log", O_RDWR | O_CREAT | O_AT_END)) {
            sd.errorHalt("opening sensor.log for write failed");
        }
        // if the file opened okay, write to it:
        Serial.println("Writing to sensor.log ...");
        myFile.printf("Startup complete: ");
        myFile.println(Time.timeStr() + "\n");
        // close the file:
        myFile.close();
        Serial.println("Startup complete");
        Serial.println(Time.timeStr());
        //Finish writing to SD

        Serial.println(WiFi.localIP());
        if (client.connect(server,2221)) {
            Serial.println("Connected to Server");
        }

}


void loop() {
    File myFile;
    //Serial.println(System.freeMemory());

    //client.connect(server,8889);
    //Serial.println(client.connect(server,8889));
    //client.println("Hello World!");
    //client.stop();

    if (cam.motionDetected()) {
       Serial.println("Motion!");
       digitalWrite(D5,HIGH);
       cam.setMotionDetect(false);

      if (! cam.takePicture())
        Serial.println("Failed to snap!");
      else
        Serial.println("Picture taken!");
        client.println("Picture taken!");


      char filename[13];
      strcpy(filename, "IMAGE00.JPG");
      for (int i = 0; i < 100; i++) {
        filename[5] = '0' + i/10;
        filename[6] = '0' + i%10;
        // create if does not exist, do not open existing, write, sync after write
        if (! sd.exists(filename)) {
          break;
        }
      }
      myFile.open("sensor.log",FILE_WRITE);
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
      while (jpglen > 0) {
        // read 32 bytes at a time;
        uint8_t *buffer;
        uint8_t bytesToRead = min(96, jpglen); // change 32 to 64 for a speedup but may not work with all setups!
        buffer = cam.readPicture(bytesToRead);

        //Serial.println(buffer.length());
        imgFile.write(buffer, bytesToRead);

        //Serial.print("Read ");  Serial.print(bytesToRead, DEC); Serial.println(" bytes");

        jpglen -= bytesToRead;
        //Serial.println(System.freeMemory());
      }
      imgFile.close();
      Serial.println("...Done!");
      Serial.println("Sending file to server...");
      myFile.open(filename, O_READ);
      //int data;
/*    while ((data = myFile.read()) >= 0){
          Serial.println(data, HEX);
          //client.write(data);
      }
*/
      Particle.publish("Picture-Taken",filename);

      time = millis() - time;
      client.println(time);
      Serial.println("done!");
      Serial.print(time); Serial.println(" ms elapsed");
      cam.resumeVideo();
      cam.setMotionDetect(true);
     }
}
