#include <WiFi.h>

double FREQ_SIGNAL = 30.0f;
double AMPLITUDE_SIGNAL = 40.0f;
double OFFSET_SIGNAL = 0.20f;

double FREQ_NOISE = 2400.0f;
double AMPLITUDE_NOISE = 0.0f;

double PHASE_SHIFT = 120.0f;

double WAVE_TYPE;

#define SIN 1
#define SQ 2
#define SAW 3

#define DAC_1 25
#define DAC_2 26

#define pi 3.1415

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

const char *ssid = "NOS-DD16";
const char *password = "XE2CPLNU";

double time_cycle = 0;
WiFiServer server(80);
String header;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop()
{
  // put your main code here, to run repeatedly:
  dacWrite(DAC_1, 158 * AMPLITUDE_SIGNAL * 0.01 * sin(2 * pi * FREQ_SIGNAL * time_cycle) + 158 * AMPLITUDE_NOISE * 0.01 * sin(2 * pi * FREQ_NOISE * time_cycle) + (1 - OFFSET_SIGNAL) * 158);
  dacWrite(DAC_2, 158 * AMPLITUDE_SIGNAL * 0.01 * cos(2 * pi * FREQ_SIGNAL * time_cycle) + 158 * AMPLITUDE_NOISE * 0.01 * sin(2 * pi * FREQ_NOISE * time_cycle) + (1 - OFFSET_SIGNAL) * 158);

  WiFiClient client = server.available(); // Listen for incoming clients

  if (client)
  { // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client."); // print a message out in the serial port
    String currentLine = "";       // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime)
    { // loop while the client's connected
      currentTime = millis();
      if (client.available())
      {                         // if there's bytes to read from the client,
        char c = client.read(); // read a byte, then
        header += c;
        if (c == '\n')
        { // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0)
          {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // turns the GPIOs on and off
            Serial.println(header);
            if (header.indexOf("GET /sin") >= 0)
              WAVE_TYPE = SIN;
            else if (header.indexOf("GET /sq") >= 0)
              WAVE_TYPE = SQ;
            else if (header.indexOf("GET /saw") >= 0)
              WAVE_TYPE = SAW;

            

            client.println("<!DOCTYPE html> <html lang=\"en\"> <head> <meta charset=\"UTF-8\"> <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\"> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"> <title>ESP32 DAC</title> </head> <style> button { border-radius:5px ; padding: 10px 40px 10px 40px; background-color: rgb(99, 214, 109); } </style> <script> let Wave_Sin = 0; let Wave_Sq = 0; let Wave_Saw = 0; function changeImag(WaveType) { if(WaveType == 1) { if(document.getElementById(\"Sine\").src.match(\"https://raw.githubusercontent.com/15git-ruimendes/MastersThesis/main/Sin_Button_Off.png\")){ document.getElementById(\"Sine\").src = \"https://raw.githubusercontent.com/15git-ruimendes/MastersThesis/main/Sin_Button_On.png\"; Wave_Sin = 1; document.getElementById(\"Square\").src = \"https://raw.githubusercontent.com/15git-ruimendes/MastersThesis/main/Square_Button_Off.png\"; Wave_Sq = 0; document.getElementById(\"Saw\").src = \"https://raw.githubusercontent.com/15git-ruimendes/MastersThesis/main/Saw_Button_Off.png\"; Wave_Saw = 0; } else { document.getElementById(\"Sine\").src = \"https://raw.githubusercontent.com/15git-ruimendes/MastersThesis/main/Sin_Button_Off.png\"; Wave_Sin = 0; } } else if(WaveType == 2) { if(document.getElementById(\"Square\").src.match(\"https://raw.githubusercontent.com/15git-ruimendes/MastersThesis/main/Square_Button_Off.png\")){ document.getElementById(\"Square\").src = \"https://raw.githubusercontent.com/15git-ruimendes/MastersThesis/main/Square_Button_On.png\"; Wave_Sq = 1; document.getElementById(\"Sine\").src = \"https://raw.githubusercontent.com/15git-ruimendes/MastersThesis/main/Sin_Button_Off.png\"; Wave_Sin = 0; document.getElementById(\"Saw\").src = \"https://raw.githubusercontent.com/15git-ruimendes/MastersThesis/main/Saw_Button_Off.png\"; Wave_Saw = 0; } else{ document.getElementById(\"Square\").src = \"https://raw.githubusercontent.com/15git-ruimendes/MastersThesis/main/Square_Button_Off.png\"; Wave_Sq = 0; } } else { if(document.getElementById(\"Saw\").src.match(\"https://raw.githubusercontent.com/15git-ruimendes/MastersThesis/main/Saw_Button_Off.png\")){ document.getElementById(\"Saw\").src = \"https://raw.githubusercontent.com/15git-ruimendes/MastersThesis/main/Saw_Button_On.png\"; Wave_Saw = 1; document.getElementById(\"Square\").src = \"https://raw.githubusercontent.com/15git-ruimendes/MastersThesis/main/Square_Button_Off.png\"; Wave_Sq = 0; document.getElementById(\"Sine\").src = \"https://raw.githubusercontent.com/15git-ruimendes/MastersThesis/main/Sin_Button_Off.png\"; Wave_Sin = 0; } else{ document.getElementById(\"Saw\").src = \"https://raw.githubusercontent.com/15git-ruimendes/MastersThesis/main/Saw_Button_Off.png\"; Wave_Saw = 0; } } } function sendData() { let opt = \"\"; if(Wave_Sin == 1) opt += \"sin/\"; else if(Wave_Sq == 1) opt += \"sq/\"; else if(Wave_Saw == 1) opt += \"saw/\"; opt += document.getElementById(\"Freq\").value; opt += \"/\"; opt += document.getElementById(\"Offset\").value; opt += \"/\"; opt += document.getElementById(\"Phase Shift\").value; opt += \"/\"; opt += document.getElementById(\"NoiseAmp\").value; opt += \"/\"; opt += document.getElementById(\"NoiseFreq\").value;document.getElementById(\"turn_on\").href = opt; } </script> <body style=\"display: grid;grid-row-gap: 10px ;\"> <h1 style=\"margin: auto;grid-column-start:1;grid-column-end:4;\">ESP32 DAC</h1> <h2 style=\"margin: auto;grid-column-start:1;grid-column-end:4;\">Wave Selection</h2> <img style=\"margin: auto;grid-column-start:1;grid-column-end:2;\" src=\"https://raw.githubusercontent.com/15git-ruimendes/MastersThesis/main/Sin_Button_Off.png\" onclick=\"changeImag(1)\" id = \"Sine\"> <img style=\"margin: auto;grid-column-start:2;grid-column-end:3;\" src=\"https://raw.githubusercontent.com/15git-ruimendes/MastersThesis/main/Square_Button_Off.png\"onclick=\"changeImag(2)\" id = \"Square\"> <img style=\"margin: auto;grid-column-start:3;grid-column-end:4;\" src=\"https://raw.githubusercontent.com/15git-ruimendes/MastersThesis/main/Saw_Button_Off.png\"onclick=\"changeImag(3)\" id = \"Saw\"> <h2 style=\"margin: auto;grid-column-start:1;grid-column-end:4;\">Frequency Selection</h2> <input style=\"margin: auto;grid-column-start:1;grid-column-end:4;\" id = \"Freq\" type = \"number\" step = \"1\" max=\"4000\" min = \"0\"> <h2 style=\"margin: auto;grid-column-start:1;grid-column-end:4;\">Offset Selection</h2> <input style=\"margin: auto;grid-column-start:1;grid-column-end:4;\" id = \"Offset\" type = \"number\" step = \"1\" max=\"255\" min = \"0\"> <h2 style=\"margin: auto;grid-column-start:1;grid-column-end:4;\">Phase Shift</h2> <input style=\"margin: auto;grid-column-start:1;grid-column-end:4;\" id = \"Phase Shift\" type = \"number\" step = \"1\" max=\"180\" min = \"0\"> <h2 style=\"margin: auto;grid-column-start:1;grid-column-end:4;\">Noise Level</h2> <input style=\"margin: auto;grid-column-start:1;grid-column-end:4;\" id = \"NoiseAmp\" type = \"number\" step = \"1\" max=\"255\" min = \"0\"> <h2 style=\"margin: auto;grid-column-start:1;grid-column-end:4;\">Noise Frequency</h2> <input style=\"margin: auto;grid-column-start:1;grid-column-end:4;\" id = \"NoiseFreq\" type = \"number\" step = \"1\" max=\"4000\" min = \"0\"> <a style = \"margin: auto;grid-column-start:2;grid-column-end:3;\" id = \"turn_on\" ><button onclick=\"sendData()\">Activate</button></a> </body> </html>");
            client.println();
            // Break out of the while loop
            break;
          }
          else
          { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        }
        else if (c != '\r')
        {                   // if you got anything else but a carriage return character,
          currentLine += c; // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
  }

  time_cycle += 0.00005;
  if (time_cycle >= 1 / FREQ_SIGNAL)
    time_cycle = 0;
  delayMicroseconds(50);
}
