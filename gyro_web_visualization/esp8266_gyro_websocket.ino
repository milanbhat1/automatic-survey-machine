#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MPU6050_tockn.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
MPU6050 mpu(Wire);

const char* ssid = "abded"; // phone name what u given
const char* password = "jefhrf"; // hotspot passward

ESP8266WebServer server(80);
WebSocketsServer webSocket(81);

bool startSending = false;

// time counter
long t = 0;

// Display IP on OLED
void showIP(String ip){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("WiFi Connected!");
  display.println("IP Address:");
  display.setTextSize(2);
  display.println(ip);
  display.display();
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t len){}

void handleRoot() {
  String page = R"====(
<!DOCTYPE html>
<html>
<head>
<title>Automatic Survey Machine</title>
<script src="https://cdn.plot.ly/plotly-2.27.0.min.js"></script>
</head>
<body>
<h2>Automatic Survey Machine - 3D Gyroscope Plot</h2>

<div id="plot" style="width:90vw;height:80vh;"></div>

<script>
var socket = new WebSocket("ws://" + location.hostname + ":81/");

// incoming data: time, gyroY, gyroZ
socket.onmessage = function(event){
    var d = event.data.split(",");
    Plotly.extendTraces('plot',{
        x:[[parseFloat(d[0])]],   // time (s)
        y:[[parseFloat(d[1])]],   // gyro Y
        z:[[parseFloat(d[2])]]    // gyro Z
    }, [0]);
};

var trace = {
    x:[], y:[], z:[],
    mode:'lines+markers',
    type:'scatter3d',
    marker:{ size:3 },
    line:{ width:2 }
};

var layout = {
    scene:{
        xaxis:{title:'Time (s)'},    // UPDATED LABEL
        yaxis:{title:'Gyro Y (°/s)'},
        zaxis:{title:'Gyro Z (°/s)'}
    }
};

Plotly.newPlot('plot',[trace],layout);
</script>
</body>
</html>
)====";

  server.send(200,"text/html",page);
}

void setup(){
  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC,0x3C)){
    Serial.println("OLED ERROR");
    while(1);
  }

  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Connecting WiFi...");
  display.display();

  Wire.begin(D2, D1);
  mpu.begin();
  mpu.calcGyroOffsets(true);

  WiFi.begin(ssid, password);
  while(WiFi.status()!=WL_CONNECTED){
    delay(300);
    Serial.print(".");
  }
  showIP(WiFi.localIP().toString());

  server.on("/", handleRoot);
  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  startSending = true;
}

void loop(){
  server.handleClient();
  webSocket.loop();

  if(startSending){
    mpu.update();

    float gyroY = mpu.getGyroY();   // degree/sec
    float gyroZ = mpu.getGyroZ();   // degree/sec

    // send: time, gyroY, gyroZ
    String data = String(t) + "," + String(gyroY,2) + "," + String(gyroZ,2);
    webSocket.broadcastTXT(data);

    t++;  // time increases
    delay(200);
  }
}
