/*
   Ardiono Yun wifi/Web controlled rover.

   Goal in life...
      Follwoing your commands :-)

   Written by Scott Beasley - 2015
   Free to use or modify. Enjoy.
*/

/*
   Uses the l29NDriver library for the L9110 H-bridge.

   Download from https://github.com/jscottb/l29NDriver or clone the zip from
   https://github.com/jscottb/l29NDriver/archive/master.zip
   Unzip and copy to your Arduino library folder or follow the instructions
   here: http://arduino.cc/en/guide/libraries

   Note: You may need to remove the '-master' from the archive file name to add
   the library
*/

#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>
#include <Servo.h>
#include <l29NDriver.h>

L29N_Motor motor_l (13, 12, 10);
L29N_Motor motor_r (11, 8, 9);

Servo panservo;
Servo tiltservo;
int pan_pos = 90;
int tilt_pos = 60;

int motor_l_speed = 0; // speed of the left motor
int motor_r_speed = 0; // speed of the right motor
int state = 0;

YunServer server;
Process fswebcam;

void setup ( ) {
   panservo.attach (3);
   tiltservo.attach (4);

   Bridge.begin ( );
   Console.begin ( );

   server.listenOnLocalhost ( );
   server.begin ( );
}

void loop ( ) {
   YunClient client = server.accept ( );

   if (client) {
      client.setTimeout (5); // Sets the max ms to wait for stream data
      process (client);
      client.flush ( );
      client.stop ( );
   }

   delay (50);
}

// Process any restful calls sent from the web client to the Linux side.
void process (YunClient client) {
   String command = client.readString ( );
   command.trim ( );

   Console.print ("<");
   Console.print (command);
   Console.println (">");

   if (state == 0) {
      state = 1;
      digitalWrite (13, HIGH);
   } else {
      state = 0;
      digitalWrite (13, LOW);
   }

   if (command == "backward") {
      motor_l.setSpeed (0);
      motor_r.setSpeed (0);
      delay (250);

      motor_l_speed = 255;
      motor_l.setSpeed (motor_l_speed);
      motor_r_speed = 255;
      motor_r.setSpeed (motor_r_speed);

      motor_l.run (FORWARD|RELEASE);
      motor_r.run (BACKWARD|RELEASE);
   } else if (command == "stop") {
      motor_l.setSpeed (0);
      motor_r.setSpeed (0);
   } else if (command == "left") {
      motor_l.setSpeed (0);
      motor_r.setSpeed (0);
      delay (250);

      motor_l_speed = 255;
      motor_l.setSpeed (motor_l_speed);
      motor_r_speed = 255;
      motor_r.setSpeed (motor_r_speed);

      motor_l.run (FORWARD|RELEASE);
      motor_r.run (FORWARD|RELEASE);

      delay (900);
      motor_l.setSpeed (0);
      motor_r.setSpeed (0);
   } else if (command == "right") {
      motor_l.setSpeed (0);
      motor_r.setSpeed (0);
      delay (250);

      motor_l_speed = 255;
      motor_l.setSpeed (motor_l_speed);
      motor_r_speed = 255;
      motor_r.setSpeed (motor_r_speed);

      motor_l.run (BACKWARD|RELEASE);
      motor_r.run (BACKWARD|RELEASE);

      delay (900);
      motor_l.setSpeed (0);
      motor_r.setSpeed (0);
    } else if (command == "ltforward") {
       motor_l.setSpeed (0);
       motor_r.setSpeed (0);
       delay (250);

       motor_l_speed = 255;
       motor_l.setSpeed (motor_l_speed);
       motor_r_speed = 255;
       motor_r.setSpeed (motor_r_speed);

       motor_l.run (FORWARD|RELEASE);
       motor_r.run (FORWARD|RELEASE);

       delay (450);
       motor_l.setSpeed (0);
       motor_r.setSpeed (0);
    } else if (command == "rtforward") {
       motor_l.setSpeed (0);
       motor_r.setSpeed (0);
       delay (250);

       motor_l_speed = 255;
       motor_l.setSpeed (motor_l_speed);
       motor_r_speed = 255;
       motor_r.setSpeed (motor_r_speed);

       motor_l.run (BACKWARD|RELEASE);
       motor_r.run (BACKWARD|RELEASE);

       delay (450);
       motor_l.setSpeed (0);
       motor_r.setSpeed (0);
   } else if (command == "ltbackward") {
      motor_l.setSpeed (0);
      motor_r.setSpeed (0);
      delay (250);

      motor_l_speed = 255;
      motor_l.setSpeed (motor_l_speed);
      motor_r_speed = 255;
      motor_r.setSpeed (motor_r_speed);

      motor_l.run (BACKWARD|RELEASE);
      motor_r.run (BACKWARD|RELEASE);

      delay (450);
      motor_l.setSpeed (0);
      motor_r.setSpeed (0);
   } else if (command == "rtbackward") {
      motor_l.setSpeed (0);
      motor_r.setSpeed (0);
      delay (250);

      motor_l_speed = 255;
      motor_l.setSpeed (motor_l_speed);
      motor_r_speed = 255;
      motor_r.setSpeed (motor_r_speed);

      motor_l.run (FORWARD|RELEASE);
      motor_r.run (FORWARD|RELEASE);

      delay (450);
      motor_l.setSpeed (0);
      motor_r.setSpeed (0);
   } else if (command == "forward") {
      motor_l.setSpeed (0);
      motor_r.setSpeed (0);
      delay (250);

      motor_l_speed = 255;
      motor_l.setSpeed (motor_l_speed);
      motor_r_speed = 255;
      motor_r.setSpeed (motor_r_speed);

      motor_l.run (BACKWARD|RELEASE);
      motor_r.run (FORWARD|RELEASE);
   } else if (command == "panlt") {
      pan_pos--;
      if (pan_pos <= 0)
         pan_pos = 0;

      panservo.write (pan_pos);
      delay(15);
   } else if (command == "panrt") {
      pan_pos++;
      if (pan_pos >= 179)
         pan_pos = 179;

      panservo.write (pan_pos);
      delay(15);
   } else if (command == "tiltlt") {
      tilt_pos--;
      if (tilt_pos <= 0)
         tilt_pos = 0;

      tiltservo.write (tilt_pos);
      delay(15);
   } else if (command == "tiltrt") {
      tilt_pos++;
      if (tilt_pos >= 179)
         tilt_pos = 179;

      tiltservo.write (tilt_pos);
      delay(15);
   } else if (command == "camon") {
      Process p;
//      mjpg_streamer -i "input_uvc.so -d /dev/video0 -r 370x275 -f 30" -o
// "output_http.so -p 8080 -w /www/webcam"
      p.begin ("mjpg_streamer");
//      fswebcam.addParameter ("-c .fswebcamconfig");
//      fswebcam.runShellCommand("fswebcam /mnt/sda1/arduino/www/test.jpg -r 370x275 -p YUYV");
      p.addParameter ("-i \"input_uvc.so\"");
      p.addParameter ("-d /dev/video0");
      p.addParameter ("-r 370x275");
      p.addParameter ("-f 30");
      p.addParameter ("-o \"output_http.so -p 8080 -w /www/webcam\"");
      p.runAsynchronously ( );
//      while(fswebcam.running ( ));
      delay (1000);
      //fswebcam.close ( );
   } else if (command == "camoff") {
      Process p;
      p.runShellCommandAsynchronously ("killall mjpg_streamer");
      delay (1000);
      p.close ( );
   } else {
      Console.println ("unknow resource.");
   }

   //Show the state of the motors after each action
   Console.print ("Left: ");
   Console.print (motor_l.getSpeed ( ));
   Console.print (" - ");
   Console.print (decodeState (motor_l.getState ( )));
   Console.print (" - ");
   Console.println (decodeState (motor_l.getDirection ( )));

   Console.print ("Right: ");
   Console.print (motor_r.getSpeed ( ));
   Console.print (" - ");
   Console.print (decodeState (motor_r.getState ( )));
   Console.print (" - ");
   Console.println (decodeState (motor_r.getDirection ( )));
}

String decodeState (int state) {
   String result = "";

   switch (state) {
      case FORWARD:
         result = "Forward";
         break;

      case BACKWARD:
         result = "Backward";
         break;

      case BRAKE:
         result = "Brake";
         break;

      case RELEASE:
         result = "Release";
         break;
   }

   return result;
}
