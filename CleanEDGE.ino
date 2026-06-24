#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ================= LCD =================
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ================= WIFI / WEB GUI MODE =================
// Tukar ikut WiFi rumah / hotspot kau
const char* ssid = "hyzenn";
const char* password = "13572468";

WebServer server(80);

const char CLEANEDGE_LOGO[] PROGMEM = R"rawliteral(data:image/webp;base64,UklGRkwEAABXRUJQVlA4WAoAAAAQAAAAPgAAPwAAQUxQSI8BAAABkEPbtqk9++KP3dqqzCpdUtm2rdpOuqSy0VmVbZS27eS9c8/+jXfrjBERE4AirQXqjz/48C/LVp6enNsKMBYldUC3PZ+ZzV/HegKuBA5ND5AMQctMQyB5qgOcKYaxGP2VQZTZVBGm82FMEcZiDSnMsii35RhTiPHYyFSZbU140lpTwGMdE2Y/4V44AA5DmDDGhPPgYE3dr0Gj0DRpY5zDfqaMU3gGFm0ZGKuwO7CFaTx6DLXeUaMhfzceyMB4hXM2qER16AZDRMrn76gRkfozsv9mprF9pMb1PCrl94MMEQVeX0CJSLi+TaIRKXvgsko0QR9VwDjGk3IhTNWXGiLR8Ku28ZjENJKUy+DhzEVKFIH3K1oDa5r9CCECTUNXOAAOAymaOU05Cx4FPaZSQsY05XJ4FO4xhZRMCbkIDkV79H1PCZkJKX9PgDPFgEejo6SIZkBFyPOt4VB8Bwy9RzJI0DLQIIHko2kGHiW1Fr7v4W8s+x8nRlUGLErRAWg8YuvNV39K7d/rmzvGNQXgUUwAVlA4IJYCAACwDACdASo/AEAAPsFUpEunpKOhrhbaqPAYCWwAvCALfD8ll6D5+MbtmfMB0Lf8lviHoAeW57Ii/UmgoGfbe+AUQ6ybbQd5NDrLaQVQerxrEUVj4ADXptyyg57wpWaUHy1dbgtM30T03r0ynOeu4AD9E5BHbZXX/QyneIlQesuqxgzh6X4P+29GRk/bPOgsp3q7Cc7P9MHGM95164vXFzI/+fi67XSan467+UR+8XNaGQKxYm+30VF0bnuhXjE1UTQboE0LTX9ZwAkB/ICmUy+vvmMivrOMJlu5OorI65oJaekATIBz/N+teOdqwPLL2QRRgAff6biqFcwq7Rr9dLP6Au+CEFjgV9/LFUBcxYpx01LX6/1/jN775eYgv9UVH3iJcQQ2NSJvTTPWTQqzZTFyL0hxyGOkxc8aYRJ7vjariesWnvpju237QoZIazOwqU4cHv3udhZuR0TJKhmcqWItZVGY+i6J/vrjIGgKwv7ooTX3MY06R/Jq/HFhpYsvpggMBdCDNjez+RZSy1a8UWGIn2Hdthu2egXJp5SfFGJ1ClfM8JlUGNE7rtuLjaPLsH9z5o0P2IUfXD00bzMUasgn0eF4EtQHvHrTJao/MdIHaCLxtGJeGru4716e3GOidSlN3DT8/t2v2fmr5bZDRN5oxqs+Cb23tfXL2aiMsNIxpgoGu4R2mPgxjKNZpA+BH0MbJ6lJ5rH9WCHC1Av/wOyEmFJabpsxWwGk7hkskn5DsnRaP1kY5kaFtPaiqV0SnOUKfGzhqmrtiQa9IHzWj4VE8jaDpHthrWHjYO2FdgFcWFEEkKahLcFNZxqKlKUfRdQyRrdrv3ulZ1hpF6acf07EZ7SfLpsIcD6EaPO+BpuNG5hrhHiUAA==)rawliteral";

const char FIVECORE_LOGO[] PROGMEM = R"rawliteral(data:image/webp;base64,UklGRuoBAABXRUJQVlA4IN4BAABQCwCdASpWAC8APt1orlGopaQipzQJuRAbiUAaQyexANsBdlbWjdArQZaelKmg+Oj6f9gBSey+nzXOxMzTZHeTdqQSu/wlW9EWeDUnJ/khFuMS+N4lnCCQ5I3QnCpbmifBcAAA/vikDW84wKWf28ekx5xACu2VPtDD1bW7Ir+Vg7paKesn3iGRtozN+9t5HteQUjVqwfgsU/2v0YT+iS+HjsOYYcD+kpXUFccT9XZHqR6ktpTYm2RvpWdKWtXFuxI/fgmZ0O2+Om1m1SQDGYxBuEc7cN4Opl4laGuj+w9vmIHwIQZ0ca0sA0VW/FoFKLpnvnErOBbTiZD12M2LRR8HoSEsUWAXYJMI06Yz0pFEru/yng3r/thsA67CP4iqaUFWHRNb2dN/Hmyd9Tj6fG1H0+H08q+3yQ3sTlTIduJ6uX66Wu5FyFlK/5CvK9ueTlf0eTDiXLNRcDnoNhl0CgLDuihLG/GzU3CW6b5JKZYZjV3xQj8qgTjDGUPStz3J+AtkDz7mXFsNge7BhgEeiOt134RfE8dHz8lwuU6VTVAYcRhq2oQ7A56LneyYY2cAA9fEgLHVDDFDHj0OEaWHxP50GCPnq43WrcxBvItfnjaVQrZJ0C44oaUNMCMoAAAA)rawliteral";


// ================= RELAY PUMP =================
// IN1 = Soap Pump
// IN2 = Water Pump
#define SOAP_PUMP_RELAY   13
#define WATER_PUMP_RELAY  14

// Relay korang active HIGH
#define RELAY_ON   HIGH
#define RELAY_OFF  LOW

// ================= SMARTDRIVEDUO MOTOR =================
// GPIO18 -> AN1 = speed/PWM
// GPIO19 -> IN1 = direction
#define MOTOR_PWM 18
#define MOTOR_DIR 19

int motorSpeed = 160;   // slower motor speed (0 - 255)

// ================= BUZZER =================
#define BUZZER 27

// ================= LIMIT SWITCH / DOOR SAFETY =================
// Door closed = limit switch pressed = LOW
// Door open   = limit switch released = HIGH
#define LIMIT_SWITCH 16
#define DOOR_CLOSED LOW
#define DOOR_OPEN   HIGH

// ================= BUTTON =================
#define STOP_BUTTON   17   // RED button = Stop / Reset
#define MODE_BUTTON   25   // BLU button = Select Mode / Offline
#define START_BUTTON  33   // GRN button = Start / Pause / Online

#define BUTTON_PRESSED LOW

// ================= MODE =================
int selectedMode = 0;

String modeName[] = {
  "Quick Wash",
  "Normal Wash",
  "Rinse Only"
};

// ================= TIMING =================
// Quick Wash total = 95 seconds
unsigned long quickStage1  = 20000;  // Water + Motor = 20s
unsigned long quickStage2  = 25000;  // Water + Soap + Motor = 25s
unsigned long quickStage3  = 20000;  // Final Rinse Water + Motor = 20s
unsigned long quickDrySpin = 20000;  // Motor only dry spin = 20s

// Normal Wash total = 145 seconds / 2 min 25s
unsigned long normalStage1  = 30000;  // Water + Motor = 30s
unsigned long normalStage2  = 45000;  // Water + Soap + Motor = 45s
unsigned long normalStage3  = 30000;  // Final Rinse Water + Motor = 30s
unsigned long normalDrySpin = 30000;  // Motor only dry spin = 30s

// Rinse Only total = 60 seconds
unsigned long rinseStage1  = 40000;  // Water + Motor = 40s
unsigned long rinseDrySpin = 20000;  // Motor only dry spin = 20s

unsigned long stopTime     = 5000;   // Stop pause = 5s

// ================= STATE =================
enum MachineState {
  IDLE,
  STAGE_1_WATER_MOTOR,
  WAIT_1,
  STAGE_2_WATER_SOAP_MOTOR,
  WAIT_2,
  STAGE_3_WATER_MOTOR,
  DRY_SPIN,
  COMPLETE,
  PAUSED,
  STOPPED,
  DOOR_ERROR
};

MachineState currentState = IDLE;
MachineState previousState = IDLE;

// For door-open resume
MachineState stateBeforeDoorOpen = IDLE;
bool doorResumeAvailable = false;

unsigned long stateStartTime = 0;
unsigned long stateDuration = 0;
unsigned long remainingTime = 0;

// ================= BUTTON PREVIOUS STATES =================
bool lastStartState = HIGH;
bool lastModeState  = HIGH;
bool lastStopState  = HIGH;

// ================= DOOR PREVIOUS STATE =================
bool lastDoorState = HIGH;

// ================= WEB STATUS =================
String webStatus = "System Ready";
String webStage = "Idle";
String espIP = "No IP";
bool onlineMode = false;

// ========================================================
// SETUP
// ========================================================
void setup() {
  Serial.begin(115200);
  delay(1000);

  // Output setup
  pinMode(SOAP_PUMP_RELAY, OUTPUT);
  pinMode(WATER_PUMP_RELAY, OUTPUT);
  pinMode(MOTOR_PWM, OUTPUT);
  pinMode(MOTOR_DIR, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  // Input setup
  pinMode(START_BUTTON, INPUT_PULLUP);
  pinMode(MODE_BUTTON, INPUT_PULLUP);
  pinMode(STOP_BUTTON, INPUT_PULLUP);
  pinMode(LIMIT_SWITCH, INPUT_PULLUP);

  allOff();

  // LCD setup
  Wire.begin(21, 22);
  lcd.begin(16, 2);
  lcd.backlight();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("CleanEDGE");
  lcd.setCursor(0, 1);
  lcd.print("Starting...");

  // Startup option: Offline or Online mode
  chooseStartupMode();

  if (onlineMode) {
    connectToWiFi();

    if (onlineMode) {
      // Web server routes
      server.on("/", handleRoot);
      server.on("/quick", handleQuick);
      server.on("/normal", handleNormal);
      server.on("/rinse", handleRinse);
      server.on("/start", handleStartPauseWeb);
      server.on("/pause", handleStartPauseWeb);
      server.on("/stop", handleStopWeb);
      server.on("/status", handleStatus);

      server.begin();
      Serial.println("Web server started");
    }
  } else {
    espIP = "Offline";
    WiFi.mode(WIFI_OFF);
    Serial.println("Offline mode selected. WiFi disabled.");
  }

  lastDoorState = digitalRead(LIMIT_SWITCH);

  if (doorIsClosed()) {
    showHomeScreen();
  } else {
    currentState = DOOR_ERROR;
    showDoorOpen();
  }

  beepReady();
}

// ========================================================
// LOOP
// ========================================================
void loop() {
  if (onlineMode) {
    server.handleClient();
  }

  checkDoorSafety();
  readButtons();
  runDishwasher();
}

// ========================================================
// STARTUP MODE SELECTION
// ========================================================
void chooseStartupMode() {
  // Welcome screen first
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("CleanEDGE");
  lcd.setCursor(0, 1);
  lcd.print("Ready!");

  Serial.println("CleanEDGE Ready!");

  beepButton();
  delay(2000);

  // Then show mode selection
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Select Mode:");
  lcd.setCursor(0, 1);
  lcd.print("GRN On BLU Off");

  Serial.println("Startup mode selection:");
  Serial.println("GRN button = Online Mode");
  Serial.println("BLU button = Offline Mode");

  unsigned long startTime = millis();

  while (true) {
    bool modeButtonState = digitalRead(MODE_BUTTON);    // BLU button
    bool startButtonState = digitalRead(START_BUTTON);  // GRN button

    if (modeButtonState == BUTTON_PRESSED) {
      onlineMode = false;
      espIP = "Offline";

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Offline Mode");
      lcd.setCursor(0, 1);
      lcd.print("Physical Button");

      beepButton();
      delay(1500);
      return;
    }

    if (startButtonState == BUTTON_PRESSED) {
      onlineMode = true;

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Online Mode");
      lcd.setCursor(0, 1);
      lcd.print("WiFi Starting");

      beepButton();
      delay(1500);
      return;
    }

    // If user does not choose within 10 seconds, auto offline
    if (millis() - startTime > 10000) {
      onlineMode = false;
      espIP = "Offline";

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Auto Offline");
      lcd.setCursor(0, 1);
      lcd.print("No WiFi Delay");

      beepButton();
      delay(1500);
      return;
    }
  }
}

// ========================================================
// WIFI CONNECTION
// ========================================================
bool attemptWiFiConnection(String attemptLabel) {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(true);
  delay(300);
  WiFi.begin(ssid, password);

  Serial.println(attemptLabel);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connecting WiFi");
  lcd.setCursor(0, 1);
  lcd.print(ssid);

  int retry = 0;

  // 16 x 0.5s = around 8 seconds only
  while (WiFi.status() != WL_CONNECTED && retry < 16) {
    delay(500);
    Serial.print(".");
    retry++;
  }

  Serial.println("");

  if (WiFi.status() == WL_CONNECTED) {
    IPAddress IP = WiFi.localIP();
    espIP = IP.toString();

    Serial.println("WiFi connected!");
    Serial.print("ESP32 IP address: ");
    Serial.println(IP);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ESP32 IP:");
    lcd.setCursor(0, 1);
    lcd.print(espIP);

    delay(8000);
    return true;
  }

  Serial.println("WiFi connection failed.");
  return false;
}

void setOfflineAfterWiFiFail() {
  onlineMode = false;
  espIP = "Offline";
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Offline Mode");
  lcd.setCursor(0, 1);
  lcd.print("Physical Button");

  Serial.println("Switched to Offline Mode.");
  delay(2000);
}

void connectToWiFi() {
  bool connected = attemptWiFiConnection("Connecting to WiFi...");

  if (connected) {
    return;
  }

  // First attempt failed: ask user once whether to reconnect
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi Failed");
  lcd.setCursor(0, 1);
  lcd.print("GRN Retry BLU No");

  Serial.println("WiFi failed. GRN button = retry once, BLU button = offline.");
  beepDoorError();

  unsigned long choiceStart = millis();

  while (true) {
    bool modeButtonState = digitalRead(MODE_BUTTON);    // BLU button = No / Offline
    bool startButtonState = digitalRead(START_BUTTON);  // GRN button = Retry once

    if (startButtonState == BUTTON_PRESSED) {
      beepButton();

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Retry WiFi");
      lcd.setCursor(0, 1);
      lcd.print("Please wait...");
      delay(1000);

      connected = attemptWiFiConnection("Retrying WiFi once...");

      if (connected) {
        onlineMode = true;
        return;
      }

      // Retry failed: go offline directly
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Retry Failed");
      lcd.setCursor(0, 1);
      lcd.print("Going Offline");
      beepDoorError();
      delay(2000);

      setOfflineAfterWiFiFail();
      return;
    }

    if (modeButtonState == BUTTON_PRESSED) {
      beepButton();
      setOfflineAfterWiFiFail();
      return;
    }

    // If no choice within 8 seconds, go offline to avoid long waiting
    if (millis() - choiceStart > 8000) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("No Selection");
      lcd.setCursor(0, 1);
      lcd.print("Going Offline");
      beepButton();
      delay(1500);

      setOfflineAfterWiFiFail();
      return;
    }
  }
}

// ========================================================
// WIFI GUI PAGE - DARK CLEANEDGE GUI + LOGO
// ========================================================
void handleRoot() {
  String html = "";

  html += "<!DOCTYPE html><html><head>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>CleanEDGE</title>";

  html += "<style>";
  html += "*{box-sizing:border-box;}";
  html += "body{font-family:Arial,Helvetica,sans-serif;background:radial-gradient(circle at top,#164e63 0%,#0f172a 42%,#020617 100%);margin:0;padding:20px;text-align:center;color:#e2e8f0;}";
  html += ".container{max-width:530px;margin:auto;}";
  html += ".card{background:rgba(15,23,42,0.94);padding:24px;border-radius:28px;box-shadow:0 18px 45px rgba(0,0,0,0.5);border:1px solid rgba(148,163,184,0.24);}";

  html += ".brandRow{display:flex;align-items:center;justify-content:center;gap:10px;margin-top:4px;margin-bottom:4px;}";
  html += ".cleanLogo{width:46px;height:46px;border-radius:12px;box-shadow:0 0 16px rgba(56,189,248,0.45);}";
  html += ".title{font-size:42px;font-weight:900;color:#f8fafc;margin:0;letter-spacing:-1px;text-shadow:0 0 20px rgba(56,189,248,0.55);}";
  html += ".poweredRow{display:flex;align-items:center;justify-content:center;gap:8px;margin-bottom:8px;flex-wrap:wrap;}";
  html += ".subtitle{font-size:15px;font-weight:700;color:#94a3b8;margin:0;}";
  html += ".fiveLogoBox{width:86px;height:56px;border-radius:18px;overflow:hidden;background:#f8fafc;display:flex;align-items:center;justify-content:center;padding:5px;box-shadow:0 0 14px rgba(56,189,248,0.28);}";
  html += ".fiveLogo{width:100%;height:100%;object-fit:contain;border-radius:12px;}";
  html += ".online{font-size:13px;font-weight:900;color:#22c55e;margin-bottom:16px;letter-spacing:.6px;}";

  html += ".grid{display:grid;grid-template-columns:1fr 1fr;gap:12px;margin:18px 0;}";
  html += ".box{background:rgba(30,41,59,0.96);border:1px solid rgba(125,211,252,0.22);border-radius:18px;padding:14px 10px;min-height:90px;box-shadow:0 8px 18px rgba(0,0,0,0.28);}";
  html += ".boxTitle{font-size:12px;font-weight:900;color:#94a3b8;text-transform:uppercase;letter-spacing:.7px;margin-bottom:8px;}";
  html += ".boxValue{font-size:17px;font-weight:900;color:#7dd3fc;word-wrap:break-word;}";

  html += ".boxOperation{background:rgba(15,23,42,0.96);border-color:#38bdf8;}";
  html += ".boxDoorClosed{background:rgba(20,83,45,0.92);border-color:#22c55e;}";
  html += ".boxDoorClosed .boxValue{color:#bbf7d0;}";
  html += ".boxDoorOpen{background:rgba(120,53,15,0.92);border-color:#f59e0b;}";
  html += ".boxDoorOpen .boxValue{color:#fed7aa;}";
  html += ".boxMode{background:rgba(76,29,149,0.82);border-color:#a78bfa;}";
  html += ".boxMode .boxValue{color:#ddd6fe;}";
  html += ".boxTime{background:rgba(120,53,15,0.90);border-color:#f59e0b;}";
  html += ".boxTime .boxValue{color:#fed7aa;font-size:32px;letter-spacing:1px;}";

  html += "h3{color:#bae6fd;margin-top:22px;margin-bottom:10px;font-size:18px;}";
  html += "button{width:92%;padding:15px;margin:8px 0;font-size:17px;border:none;border-radius:16px;color:white;font-weight:bold;box-shadow:0 8px 18px rgba(0,0,0,0.30);cursor:pointer;}";
  html += ".quick{background:linear-gradient(135deg,#0284c7,#0369a1);}";
  html += ".normal{background:linear-gradient(135deg,#0f766e,#115e59);}";
  html += ".rinse{background:linear-gradient(135deg,#7c3aed,#5b21b6);}";
  html += ".controlBtn{width:94%;padding:19px;margin:10px 0;font-size:20px;border-radius:18px;}";
  html += ".start{background:linear-gradient(135deg,#16a34a,#15803d);}";
  html += ".stop{background:linear-gradient(135deg,#dc2626,#991b1b);}";
  html += ".footer{margin-top:20px;padding:12px;border-radius:16px;background:rgba(15,23,42,0.85);border:1px solid rgba(148,163,184,0.2);font-size:13px;color:#cbd5e1;}";
  html += ".footer b{color:#7dd3fc;}";
  html += "</style>";
  html += "</head><body>";

  html += "<div class='container'><div class='card'>";

  html += "<div class='brandRow'>";
  html += "<img class='cleanLogo' src='" + String(CLEANEDGE_LOGO) + "'>";
  html += "<div class='title'>CleanEDGE</div>";
  html += "</div>";

  html += "<div class='poweredRow'>";
  html += "<div class='subtitle'>powered by FiveCore Technologies SDN BHD</div>";
  html += "<div class='fiveLogoBox'>";
  html += "<img class='fiveLogo' src='" + String(FIVECORE_LOGO) + "'>";
  html += "</div>";
  html += "</div>";

  html += "<div id='onlineStatus' class='online'>&#9679; ONLINE</div>";

  html += "<div class='grid'>";
  html += "<div class='box boxOperation'><div class='boxTitle'>System Status</div><div id='systemStatus' class='boxValue'>" + getSystemStatusText() + "</div></div>";

  if (doorIsClosed()) {
    html += "<div id='doorBox' class='box boxDoorClosed'>";
  } else {
    html += "<div id='doorBox' class='box boxDoorOpen'>";
  }
  html += "<div class='boxTitle'>Door Safety</div><div id='doorSafety' class='boxValue'>" + String(doorIsClosed() ? "Door Closed" : "Door Open") + "</div></div>";

  html += "<div class='box boxMode'><div class='boxTitle'>Selected Mode</div><div id='selectedMode' class='boxValue'>" + modeName[selectedMode] + "</div></div>";
  html += "<div class='box boxTime'><div class='boxTitle'>Time Left</div><div id='timeLeft' class='boxValue'>" + getRemainingSeconds() + "</div></div>";
  html += "</div>";

  html += "<h3>Washing Mode</h3>";
  html += "<button class='quick' onclick=\"sendCommand('/quick')\">Quick Wash</button>";
  html += "<button class='normal' onclick=\"sendCommand('/normal')\">Normal Wash</button>";
  html += "<button class='rinse' onclick=\"sendCommand('/rinse')\">Rinse Only</button>";

  html += "<h3>Machine Control</h3>";
  html += "<button class='start controlBtn' onclick=\"sendCommand('/start')\">START / PAUSE</button>";
  html += "<button class='stop controlBtn' onclick=\"sendCommand('/stop')\">STOP / RESET</button>";

  html += "<div class='footer'><b>Connected</b><br>IP: " + espIP + "</div>";
  html += "</div></div>";

  html += "<script>";
  html += "function sendCommand(path){fetch(path).then(()=>setTimeout(updateStatus,200)).catch(()=>alert('Failed to connect to CleanEDGE ESP32.'));}";
  html += "function updateStatus(){fetch('/status').then(r=>r.json()).then(d=>{";
  html += "document.getElementById('systemStatus').innerText=d.systemStatus;";
  html += "document.getElementById('doorSafety').innerText=d.doorSafety;";
  html += "document.getElementById('selectedMode').innerText=d.selectedMode;";
  html += "document.getElementById('timeLeft').innerText=d.timeLeft;";
  html += "document.getElementById('onlineStatus').innerHTML=d.online?'&#9679; ONLINE':'&#9679; OFFLINE';";
  html += "document.getElementById('onlineStatus').style.color=d.online?'#22c55e':'#ef4444';";
  html += "let doorBox=document.getElementById('doorBox');";
  html += "doorBox.classList.remove('boxDoorClosed','boxDoorOpen');";
  html += "doorBox.classList.add(d.doorClosed?'boxDoorClosed':'boxDoorOpen');";
  html += "}).catch(e=>console.log(e));}";
  html += "setInterval(updateStatus,1000);";
  html += "</script>";

  html += "</body></html>";

  server.send(200, "text/html", html);
}

void handleStatus() {
  String json = "{";
  json += "\"online\":" + String((onlineMode && WiFi.status() == WL_CONNECTED) ? "true" : "false") + ",";
  json += "\"doorClosed\":" + String(doorIsClosed() ? "true" : "false") + ",";
  json += "\"systemStatus\":\"" + getSystemStatusText() + "\",";
  json += "\"doorSafety\":\"" + String(doorIsClosed() ? "Door Closed" : "Door Open") + "\",";
  json += "\"selectedMode\":\"" + modeName[selectedMode] + "\",";
  json += "\"timeLeft\":\"" + getRemainingSeconds() + "\"";
  json += "}";

  server.send(200, "application/json", json);
}

// ========================================================
// WEB BUTTON HANDLERS
// ========================================================
void handleQuick() {
  if (canChangeMode()) {
    selectedMode = 0;
    webStatus = "Quick Wash selected";
    showModeSelected();
    beepButton();
  }
  redirectHome();
}

void handleNormal() {
  if (canChangeMode()) {
    selectedMode = 1;
    webStatus = "Normal Wash selected";
    showModeSelected();
    beepButton();
  }
  redirectHome();
}

void handleRinse() {
  if (canChangeMode()) {
    selectedMode = 2;
    webStatus = "Rinse Only selected";
    showModeSelected();
    beepButton();
  }
  redirectHome();
}

void handleStartPauseWeb() {
  handleStartPause();
  redirectHome();
}

void handleStopWeb() {
  handleStopReset();
  redirectHome();
}

void redirectHome() {
  server.sendHeader("Location", "/");
  server.send(303);
}

// ========================================================
// DOOR SAFETY WITH RESUME
// ========================================================
bool doorIsClosed() {
  return digitalRead(LIMIT_SWITCH) == DOOR_CLOSED;
}

bool isRunningState(MachineState state) {
  return state == STAGE_1_WATER_MOTOR ||
         state == WAIT_1 ||
         state == STAGE_2_WATER_SOAP_MOTOR ||
         state == WAIT_2 ||
         state == STAGE_3_WATER_MOTOR ||
         state == DRY_SPIN;
}

void checkDoorSafety() {
  bool doorState = digitalRead(LIMIT_SWITCH);

  // Door opened
  if (doorState == DOOR_OPEN && lastDoorState == DOOR_CLOSED) {

    if (isRunningState(currentState)) {
      stateBeforeDoorOpen = currentState;

      unsigned long elapsed = millis() - stateStartTime;

      if (elapsed < stateDuration) {
        remainingTime = stateDuration - elapsed;
      } else {
        remainingTime = 0;
      }

      doorResumeAvailable = true;
    } else {
      doorResumeAvailable = false;
    }

    allOff();

    currentState = DOOR_ERROR;
    webStatus = "Door opened";
    webStage = doorResumeAvailable ? "Paused for safety" : "Safety stop";

    showDoorOpen();
    beepDoorError();

    Serial.println("Door opened. Operation stopped for safety.");
  }

  // Door closed again
  if (doorState == DOOR_CLOSED && lastDoorState == DOOR_OPEN) {
    if (currentState == DOOR_ERROR) {

      if (doorResumeAvailable) {
        currentState = PAUSED;
        previousState = stateBeforeDoorOpen;
        webStatus = "Door closed";
        webStage = "Ready to resume";
        Serial.println("Door closed. Ready to resume.");
      } else {
        currentState = STOPPED;
        webStatus = "Door closed";
        webStage = "Ready";
        Serial.println("Door closed. Ready.");
      }

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Door Closed");
      lcd.setCursor(0, 1);
      lcd.print("Press START");

      beepButton();
    }
  }

  lastDoorState = doorState;
}

void showDoorOpen() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("DOOR OPEN!");
  lcd.setCursor(0, 1);
  lcd.print(doorResumeAvailable ? "Close to Resume" : espIP);
}

// ========================================================
// BUTTON READ
// ========================================================
void readButtons() {
  bool startState = digitalRead(START_BUTTON);
  bool modeState  = digitalRead(MODE_BUTTON);
  bool stopState  = digitalRead(STOP_BUTTON);

  if (startState == BUTTON_PRESSED && lastStartState != BUTTON_PRESSED) {
    handleStartPause();
    delay(250);
  }

  if (modeState == BUTTON_PRESSED && lastModeState != BUTTON_PRESSED) {
    handleModeSelect();
    delay(250);
  }

  if (stopState == BUTTON_PRESSED && lastStopState != BUTTON_PRESSED) {
    handleStopReset();
    delay(250);
  }

  lastStartState = startState;
  lastModeState  = modeState;
  lastStopState  = stopState;
}

// ========================================================
// BUTTON FUNCTIONS
// ========================================================
bool canChangeMode() {
  return currentState == IDLE || currentState == COMPLETE || currentState == STOPPED || currentState == DOOR_ERROR;
}

void handleModeSelect() {
  if (canChangeMode()) {
    selectedMode++;

    if (selectedMode > 2) {
      selectedMode = 0;
    }

    allOff();
    doorResumeAvailable = false;

    showModeSelected();
    beepButton();

    webStatus = "Mode selected";
    webStage = modeName[selectedMode];

    Serial.print("Mode selected: ");
    Serial.println(modeName[selectedMode]);
  }
}

void showModeSelected() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Mode Selected");
  lcd.setCursor(0, 1);
  lcd.print(modeName[selectedMode]);
}

void handleStartPause() {
  beepButton();

  if (!doorIsClosed()) {
    allOff();
    currentState = DOOR_ERROR;
    webStatus = "Cannot start";
    webStage = "Door open";
    showDoorOpen();
    beepDoorError();
    Serial.println("Cannot start. Door is open.");
    return;
  }

  // Door opened during operation, then closed again: resume saved state
  if (doorResumeAvailable) {
    resumeFromDoorOpen();
    return;
  }

  if (currentState == IDLE || currentState == COMPLETE || currentState == STOPPED || currentState == DOOR_ERROR) {
    startCycle();
  }
  else if (currentState == PAUSED) {
    resumeCycle();
  }
  else {
    pauseCycle();
  }
}

void handleStopReset() {
  allOff();

  currentState = STOPPED;
  doorResumeAvailable = false;

  webStatus = "System stopped";
  webStage = "Stopped";

  if (doorIsClosed()) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SYSTEM STOPPED");
    lcd.setCursor(0, 1);
    lcd.print("Press START");
  } else {
    showDoorOpen();
  }

  beepStop();
  Serial.println("System stopped/reset");
}

// ========================================================
// MACHINE FLOW
// ========================================================
void startCycle() {
  allOff();
  doorResumeAvailable = false;

  if (!doorIsClosed()) {
    currentState = DOOR_ERROR;
    webStatus = "Cannot start";
    webStage = "Door open";
    showDoorOpen();
    beepDoorError();
    return;
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Starting...");
  lcd.setCursor(0, 1);
  lcd.print(modeName[selectedMode]);

  webStatus = "Cycle started";
  webStage = modeName[selectedMode];

  beepStart();

  Serial.print("Starting mode: ");
  Serial.println(modeName[selectedMode]);

  delay(1000);

  changeState(STAGE_1_WATER_MOTOR, getStageDuration(1));
}

void pauseCycle() {
  previousState = currentState;

  unsigned long elapsed = millis() - stateStartTime;

  if (elapsed < stateDuration) {
    remainingTime = stateDuration - elapsed;
  } else {
    remainingTime = 0;
  }

  allOff();

  currentState = PAUSED;
  webStatus = "Paused";
  webStage = "Paused";

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("PAUSED");
  lcd.setCursor(0, 1);
  lcd.print("Press START");

  beepPause();
  Serial.println("Cycle paused");
}

void resumeCycle() {
  if (!doorIsClosed()) {
    currentState = DOOR_ERROR;
    webStatus = "Cannot resume";
    webStage = "Door open";
    showDoorOpen();
    beepDoorError();
    return;
  }

  currentState = previousState;
  stateDuration = remainingTime;
  stateStartTime = millis();

  webStatus = "Resumed";
  webStage = "Running";

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Resuming...");
  lcd.setCursor(0, 1);
  lcd.print("Please wait");

  beepStart();
  Serial.println("Cycle resumed");

  delay(800);
}

void resumeFromDoorOpen() {
  currentState = stateBeforeDoorOpen;
  stateDuration = remainingTime;
  stateStartTime = millis();

  doorResumeAvailable = false;

  webStatus = "Resumed after door close";
  webStage = "Running";

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Resuming...");
  lcd.setCursor(0, 1);
  lcd.print("Safety OK");

  beepStart();

  Serial.println("Cycle resumed from door safety stop.");

  delay(800);
}

void runDishwasher() {
  if (!doorIsClosed()) {
    if (currentState != DOOR_ERROR) {
      allOff();
      currentState = DOOR_ERROR;
      webStatus = "Door opened";
      webStage = "Safety stop";
      showDoorOpen();
      beepDoorError();
    }
    return;
  }

  switch (currentState) {
    case IDLE:
      allOff();
      break;

    case STAGE_1_WATER_MOTOR:
      waterPumpOn();
      soapPumpOff();
      motorForward(motorSpeed);

      webStage = "Stage 1: Water + Motor";
      showTimeLCD("Water + Motor", "Stage 1");

      if (stateFinished()) {
        allOff();

        if (selectedMode == 2) {
          changeState(DRY_SPIN, getDrySpinDuration());
        } else {
          changeState(WAIT_1, stopTime);
        }
      }
      break;

    case WAIT_1:
      allOff();

      webStage = "Stop 5 seconds";
      showTimeLCD("Stop", "Wait 5s");

      if (stateFinished()) {
        changeState(STAGE_2_WATER_SOAP_MOTOR, getStageDuration(2));
      }
      break;

    case STAGE_2_WATER_SOAP_MOTOR:
      // Stage 2 must activate BOTH pumps + motor.
      // Force relay pins directly here so soap pump definitely receives GPIO13 HIGH.
      digitalWrite(WATER_PUMP_RELAY, RELAY_ON);
      digitalWrite(SOAP_PUMP_RELAY, RELAY_ON);
      motorForward(motorSpeed);

      webStage = "Stage 2: Water + Soap + Motor";
      showTimeLCD("Water+Soap+Motor", "Stage 2");

      if (stateFinished()) {
        allOff();
        changeState(WAIT_2, stopTime);
      }
      break;

    case WAIT_2:
      allOff();

      webStage = "Stop 5 seconds";
      showTimeLCD("Stop", "Wait 5s");

      if (stateFinished()) {
        changeState(STAGE_3_WATER_MOTOR, getStageDuration(3));
      }
      break;

    case STAGE_3_WATER_MOTOR:
      waterPumpOn();
      soapPumpOff();
      motorForward(motorSpeed);

      webStage = "Stage 3: Water + Motor";
      showTimeLCD("Water + Motor", "Stage 3");

      if (stateFinished()) {
        allOff();
        changeState(DRY_SPIN, getDrySpinDuration());
      }
      break;

    case DRY_SPIN:
      waterPumpOff();
      soapPumpOff();
      motorForward(motorSpeed);

      webStage = "Dry Spin: Motor Only";
      showTimeLCD("Dry Spin", "Motor Only");

      if (stateFinished()) {
        allOff();
        finishCycle();
      }
      break;

    case COMPLETE:
      allOff();
      break;

    case PAUSED:
      allOff();
      break;

    case STOPPED:
      allOff();
      break;

    case DOOR_ERROR:
      allOff();
      break;
  }
}

unsigned long getStageDuration(int stageNumber) {
  if (selectedMode == 0) { // Quick Wash
    if (stageNumber == 1) return quickStage1;
    if (stageNumber == 2) return quickStage2;
    if (stageNumber == 3) return quickStage3;
  }
  else if (selectedMode == 1) { // Normal Wash
    if (stageNumber == 1) return normalStage1;
    if (stageNumber == 2) return normalStage2;
    if (stageNumber == 3) return normalStage3;
  }
  else if (selectedMode == 2) { // Rinse Only
    return rinseStage1;
  }

  return 15000;
}

unsigned long getDrySpinDuration() {
  if (selectedMode == 0) {        // Quick Wash
    return quickDrySpin;
  }
  else if (selectedMode == 1) {   // Normal Wash
    return normalDrySpin;
  }
  else if (selectedMode == 2) {   // Rinse Only
    return rinseDrySpin;
  }

  return 15000;
}

void changeState(MachineState newState, unsigned long duration) {
  currentState = newState;
  stateDuration = duration;
  stateStartTime = millis();

  lcd.clear();

  beepStage();

  Serial.print("State changed to: ");
  Serial.println(newState);

  if (newState == STAGE_2_WATER_SOAP_MOTOR) {
    Serial.println("Stage 2 active: Water Pump ON, Soap Pump ON, Motor ON");
    Serial.println("Soap Pump Relay GPIO13 should be HIGH now.");
  }
}

bool stateFinished() {
  return millis() - stateStartTime >= stateDuration;
}

void finishCycle() {
  allOff();

  currentState = COMPLETE;
  doorResumeAvailable = false;

  webStatus = "Cycle complete";
  webStage = modeName[selectedMode];

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Cycle Complete");
  lcd.setCursor(0, 1);
  lcd.print(modeName[selectedMode]);

  beepComplete();

  Serial.println("Cycle complete");
}

// ========================================================
// LCD FUNCTIONS
// ========================================================
void showHomeScreen() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("CleanEDGE");
  lcd.setCursor(0, 1);
  lcd.print(modeName[selectedMode]);

  webStatus = "System Ready";
  webStage = "Idle";
}

void printLCDLine(int row, String text) {
  if (text.length() > 16) {
    text = text.substring(0, 16);
  }

  while (text.length() < 16) {
    text += " ";
  }

  lcd.setCursor(0, row);
  lcd.print(text);
}

void showTimeLCD(String line1, String line2) {
  unsigned long elapsed = millis() - stateStartTime;
  unsigned long remaining = 0;

  if (elapsed < stateDuration) {
    remaining = (stateDuration - elapsed) / 1000;
  }

  printLCDLine(0, line1);
  printLCDLine(1, line2 + " " + String(remaining) + "s");
}

// ========================================================
// PUMP FUNCTIONS
// ========================================================
void soapPumpOn() {
  digitalWrite(SOAP_PUMP_RELAY, RELAY_ON);
}

void soapPumpOff() {
  digitalWrite(SOAP_PUMP_RELAY, RELAY_OFF);
}

void waterPumpOn() {
  digitalWrite(WATER_PUMP_RELAY, RELAY_ON);
}

void waterPumpOff() {
  digitalWrite(WATER_PUMP_RELAY, RELAY_OFF);
}

// ========================================================
// MOTOR FUNCTIONS
// ========================================================
void motorForward(int speedValue) {
  digitalWrite(MOTOR_DIR, HIGH);
  analogWrite(MOTOR_PWM, speedValue);
}

void motorStop() {
  analogWrite(MOTOR_PWM, 0);
}

// ========================================================
// ALL OFF
// ========================================================
void allOff() {
  soapPumpOff();
  waterPumpOff();
  motorStop();
  digitalWrite(BUZZER, LOW);
}

// ========================================================
// BUZZER FUNCTIONS
// ========================================================
void beepButton() {
  beepPassive(3000, 150);
}

void beepReady() {
  beepPassive(2500, 200);
  delay(100);
  beepPassive(3000, 200);
}

void beepStart() {
  beepPassive(3000, 300);
  delay(100);
  beepPassive(3500, 300);
}

void beepStage() {
  beepPassive(3200, 250);
}

void beepPause() {
  beepPassive(1800, 600);
}

void beepStop() {
  beepPassive(1000, 800);
  delay(150);
  beepPassive(1000, 800);
}

void beepDoorError() {
  for (int i = 0; i < 3; i++) {
    beepPassive(900, 500);
    delay(150);
  }
}

void beepComplete() {
  for (int i = 0; i < 4; i++) {
    beepPassive(3500, 300);
    delay(150);
  }
}

void beepPassive(int frequency, int durationMs) {
  int period = 1000000 / frequency;
  int halfPeriod = period / 2;
  long cycles = (long)frequency * durationMs / 1000;

  for (long i = 0; i < cycles; i++) {
    digitalWrite(BUZZER, HIGH);
    delayMicroseconds(halfPeriod);
    digitalWrite(BUZZER, LOW);
    delayMicroseconds(halfPeriod);
  }
}

// ========================================================
// WEB GUI EXTRA INFO
// ========================================================
String getRemainingSeconds() {
  unsigned long remainingMs = 0;

  if (currentState == DOOR_ERROR && doorResumeAvailable) {
    remainingMs = remainingTime;
  }
  else if (currentState == PAUSED) {
    remainingMs = remainingTime;
  }
  else if (
    currentState == IDLE ||
    currentState == COMPLETE ||
    currentState == STOPPED ||
    currentState == DOOR_ERROR
  ) {
    return "--:--";
  }
  else {
    unsigned long elapsed = millis() - stateStartTime;

    if (elapsed >= stateDuration) {
      remainingMs = 0;
    } else {
      remainingMs = stateDuration - elapsed;
    }
  }

  unsigned long totalSeconds = remainingMs / 1000;
  unsigned long minutes = totalSeconds / 60;
  unsigned long seconds = totalSeconds % 60;

  String timeText = "";

  if (minutes < 10) timeText += "0";
  timeText += String(minutes);
  timeText += ":";

  if (seconds < 10) timeText += "0";
  timeText += String(seconds);

  return timeText;
}

String getActiveOutputText() {
  switch (currentState) {
    case STAGE_1_WATER_MOTOR:
      return "Water + Motor";

    case STAGE_2_WATER_SOAP_MOTOR:
      return "Water + Soap + Motor";

    case STAGE_3_WATER_MOTOR:
      return "Water + Motor";

    case DRY_SPIN:
      return "Motor Only";

    case WAIT_1:
    case WAIT_2:
      return "No Output";

    case IDLE:
    case COMPLETE:
    case PAUSED:
    case STOPPED:
    case DOOR_ERROR:
      return "Idle";

    default:
      return "-";
  }
}

String getPhaseText() {
  switch (currentState) {
    case IDLE:
      return "Ready";

    case STAGE_1_WATER_MOTOR:
      return "Initial Rinse";

    case WAIT_1:
      return "Pause 1";

    case STAGE_2_WATER_SOAP_MOTOR:
      return "Soap Wash";

    case WAIT_2:
      return "Pause 2";

    case STAGE_3_WATER_MOTOR:
      return "Final Rinse";

    case DRY_SPIN:
      return "Dry Spin";

    case COMPLETE:
      return "Finished";

    case PAUSED:
      return "Paused";

    case STOPPED:
      return "Stopped";

    case DOOR_ERROR:
      if (doorResumeAvailable) {
        return "Ready to Resume";
      } else {
        return "Safety Stop";
      }

    default:
      return "-";
  }
}

String getSystemStatusText() {
  if (currentState == IDLE) {
    return "Ready";
  }
  else if (currentState == STAGE_1_WATER_MOTOR) {
    if (selectedMode == 2) {
      return "Rinsing";
    }
    return "Initial Rinse";
  }
  else if (currentState == STAGE_2_WATER_SOAP_MOTOR) {
    return "Soap Wash";
  }
  else if (currentState == STAGE_3_WATER_MOTOR) {
    return "Final Rinse";
  }
  else if (currentState == DRY_SPIN) {
    return "Dry Spin";
  }
  else if (currentState == WAIT_1 || currentState == WAIT_2) {
    return "Pausing";
  }
  else if (currentState == PAUSED && doorResumeAvailable) {
    return "Ready to Resume";
  }
  else if (currentState == PAUSED) {
    return "Paused";
  }
  else if (currentState == COMPLETE) {
    return "Completed";
  }
  else if (currentState == STOPPED) {
    return "Stopped";
  }
  else if (currentState == DOOR_ERROR && doorResumeAvailable) {
    return "Door Paused";
  }
  else if (currentState == DOOR_ERROR) {
    return "Door Warning";
  }
  else {
    return "Unknown";
  }
}

String getStateText() {
  switch (currentState) {
    case IDLE:
      return "Idle";
    case STAGE_1_WATER_MOTOR:
      return "Running";
    case WAIT_1:
      return "Waiting";
    case STAGE_2_WATER_SOAP_MOTOR:
      return "Running";
    case WAIT_2:
      return "Waiting";
    case STAGE_3_WATER_MOTOR:
      return "Running";
    case DRY_SPIN:
      return "Dry Spin";
    case COMPLETE:
      return "Complete";
    case PAUSED:
      return "Paused";
    case STOPPED:
      return "Stopped";
    case DOOR_ERROR:
      if (doorResumeAvailable) {
        return "Door Paused";
      } else {
        return "Door Error";
      }
    default:
      return "Unknown";
  }
}
