#define enroll 14
#define del 15    // ok
#define up 16
#define down 17
#define green 35
#define blue 33
#define red 37
#define yellow 39

void setup()
{
  delay(1000);

  pinMode(enroll, INPUT_PULLUP);
  pinMode(up, INPUT_PULLUP);
  pinMode(down, INPUT_PULLUP);
  pinMode(del, INPUT_PULLUP);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  SD.begin(chipSelect);
  /* Initialise the RFID reader */
  /***********************SET TIME and DATE RTC FUNCTION**************************/
  // myRTC.setDS1302Time(00, 21, 17, 6 , 10, 07, 2020);

  /////////////////////////////////////////
  RC522.init();   // Initiate MFRC522
  lcd.init();                      // initialize the lcd
  // Print a message to the LCD.
  lcd.backlight();

  lcd.print("Automated attedance");
  lcd.setCursor(0, 1);
  lcd.print("System using");
  lcd.setCursor(0, 2);
  lcd.print("biometric and tag");
  delay(5000);
  lcd.clear();
  finger.begin(57600);
  Serial.begin(9600);
  lcd.print("Finding Module");
  lcd.setCursor(0, 1);
  delay(1000);
  if (finger.verifyPassword())
  {
    Serial.println("Found fingerprint sensor!");
    lcd.clear();
    lcd.print("Finger print sensor");
    lcd.setCursor(0, 1);
    lcd.print("     found");
    delay(1000);
  }
  else
  {
    Serial.println("Did not find fingerprint sensor :(");
    lcd.clear();
    lcd.print("module not Found");
    lcd.setCursor(0, 1);
    lcd.print("Check Connections");
    while (1);
  }

  if (SD.begin())
  {
    Serial.println("SD card is ready to use.");
  } else
  {
    Serial.println("SD card initialization failed");
    lcd.clear();
    lcd.print("    Please insert");
    lcd.setCursor(0, 1);
    lcd.print("      SD CARD");
    delay(2000);
    return;

  }

  Datos = SD.open("Datos.csv", FILE_WRITE);        //Will open and write once just for headers
  Datos.println("User ID,    Tag No, Date, Time");       //Print headers (not saved yet)
  Datos.close();                                   //Print saved
  //digitalWrite(green,HIGH);
  //digitalWrite(blue,HIGH);
  //digitalWrite(yellow,HIGH);
  //digitalWrite(red,HIGH);
}

void loop()
{
  if (SD.begin())
  {
    // Serial.println("SD card is ready to use.");
  } else
  {
    //   Serial.println("SD card initialization failed");
    digitalWrite(green, LOW);
    lcd.clear();
    lcd.print("    Please insert");
    lcd.setCursor(0, 1);
    lcd.print("      SD CARD");
    //  delay(1000);
    redLed();
    return;
  }

  byte i;
  digitalWrite(green, HIGH);
  myRTC.updateTime();
  lcd.setCursor(0, 0);
  lcd.print("Press UP/Down button");
  lcd.setCursor(0, 1);
  lcd.print("for the attendance");
  lcd.setCursor(0, 2);
  lcd.print("Date: ");
  lcd.setCursor(7, 2);
  lcd.print(myRTC.dayofmonth); // Element 1
  lcd.print("/");
  lcd.print(myRTC.month); // Element 2
  lcd.print("/");
  lcd.print(myRTC.year); // Element 3
  lcd.print(" ");

  lcd.setCursor(0, 3);
  lcd.print("Time: ");
  lcd.setCursor(7, 3);


  lcd.print(myRTC.hours); // Element 4
  lcd.print(":");

  lcd.print(myRTC.minutes); // Element 5
  lcd.print(":");

  lcd.print(myRTC.seconds); // Element 6
  lcd.print(" "); // Element 6
  //delay(1000);

  if (digitalRead(up) == 0 || digitalRead(down) == 0)
  {
    digitalWrite(green, LOW);
    for (int i = 0; i < 10; i++)
    {
      lcd.clear();
      lcd.print("Place Finger");
      delay(4000);

      int result = getFingerprintIDez();
      //Serial.println(result);
      if (result < 0) {
        lcd.clear();
        lcd.print("No Finger detected");
        digitalWrite(red, HIGH);
        delay(500);
        digitalWrite(red, LOW);
        delay(500);
        digitalWrite(red, HIGH);
        delay(500);
        digitalWrite(red, LOW);
        delay(500);
        digitalWrite(red, HIGH);
        delay(500);
        digitalWrite(red, LOW);
        delay(500);
        digitalWrite(red, HIGH);
        delay(3000);
        digitalWrite(red, LOW);
      }
      else
      {
        digitalWrite(yellow, HIGH);
        delay(200);
        digitalWrite(yellow, LOW);
        delay(200);
        digitalWrite(yellow, HIGH);
        delay(200);
        digitalWrite(yellow, LOW);
        delay(200);
        digitalWrite(yellow, HIGH);
        delay(200);
        digitalWrite(yellow, LOW);
        lcd.clear();
        lcd.print("    Student ID: ");
        lcd.setCursor(0, 1);
        lcd.print("      A00");
        lcd.print(finger.fingerID);
        delay (2000);

        for (int i = 0; i < 5; i++)
        {
          lcd.clear();
          lcd.print("Place your card");
          delay(1000);

          if (RC522.isCard()) {

            /* If so then get its serial number */
            RC522.readCardSerial();
            yellowLed();

            lcd.setCursor(0, 1);
            lcd.print(" Card detected");
            Serial.println("Card detected:");
            yellowLed();



            lcd.clear();
            lcd.setCursor(4, 0);
            lcd.print("Writing and ");
            lcd.setCursor(2, 1);
            lcd.print("submitting your");
            lcd.setCursor(5, 2);
            lcd.print("attendance");
            lcd.setCursor(4, 3);
            lcd.print("please wait");
            digitalWrite(red, HIGH);
            Datos = SD.open("Datos.csv", FILE_WRITE);       //Will open and write according to
            Datos.print("A00");
            Datos.print(finger.fingerID);
            Datos.print(",");

            /* Output the serial number to the UART */
            for (i = 0; i <= 4; i++)
            {
              Datos.print(RC522.serNum[i], HEX);

              Serial.print(RC522.serNum[i], HEX);
              Serial.print(" ");
            }

            Datos.print(",");    //Print mesage(not saved yet)

            Datos.print(myRTC.dayofmonth); // Element 1
            Datos.print("/");
            Datos.print(myRTC.month); // Element 2
            Datos.print("/");
            Datos.print(myRTC.year); // Element 3
            Datos.print(",");

            Datos.print(myRTC.hours); // Element 4
            Datos.print(":");
            Datos.print(myRTC.minutes); // Element 5
            Datos.print(":");
            Datos.println(myRTC.seconds); // Element 6


            Serial.print("Current Date / Time: ");
            Serial.print(myRTC.dayofmonth); // Element 1
            Serial.print("/");
            Serial.print(myRTC.month); // Element 2
            Serial.print("/");
            Serial.print(myRTC.year); // Element 3
            Serial.print(" ");
            Serial.print(myRTC.hours); // Element 4
            Serial.print(":");
            Serial.print(myRTC.minutes); // Element 5
            Serial.print(":");
            Serial.println(myRTC.seconds); // Element 6
            delay( 2000);




            // Datos.println("Hello World1, Hello World2");
            // Datos.println();
            Datos.close();                                  //Mesage saved
            delay(1000);
            digitalWrite(red, LOW);

            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print(" Submitted");
            lcd.setCursor(0, 1);
            lcd.print(" Successfully");
            greenLed();
            delay(2000);

          }
          else {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print(" Card reading");
            lcd.setCursor(0, 1);
            lcd.print("   Error");
            redLed();

            return;
          }
        }

        //***********************************************/

      }
      return;
    }
  }

  checkKeys();
  delay(1000);
}

void checkKeys()
{
  if (digitalRead(enroll) == 0)
  {
    lcd.clear();
    lcd.print("Please Wait");
    delay(1000);
    while (digitalRead(enroll) == 0);
    Enroll();
  }

  else if (digitalRead(del) == 0)
  {
    lcd.clear();
    lcd.print("Please Wait");
    delay(1000);
    delet();
  }
}

void Enroll()
{
  int count = 0;
  lcd.clear();
  lcd.print("Enroll Finger    ");
  lcd.setCursor(0, 1);
  lcd.print("Location:");
  while (1)
  {
    lcd.setCursor(9, 1);
    lcd.print(count);
    if (digitalRead(up) == 0)
    {
      count++;
      if (count > 25)
        count = 0;
      delay(500);
    }

    else if (digitalRead(down) == 0)
    {
      count--;
      if (count < 0)
        count = 25;
      delay(500);
    }
    else if (digitalRead(del) == 0)
    {
      id = count;
      getFingerprintEnroll();
      return;
    }

    else if (digitalRead(enroll) == 0)
    {
      return;
    }
  }
}

void delet()
{
  int count = 0;
  lcd.clear();
  lcd.print("Delete Finger    ");
  lcd.setCursor(0, 1);
  lcd.print("Location:");
  while (1)
  {
    lcd.setCursor(9, 1);
    lcd.print(count);
    if (digitalRead(up) == 0)
    {
      count++;
      if (count > 25)
        count = 0;
      delay(500);
    }

    else if (digitalRead(down) == 0)
    {
      count--;
      if (count < 0)
        count = 25;
      delay(500);
    }
    else if (digitalRead(del) == 0)
    {
      id = count;
      deleteFingerprint(id);
      return;
    }

    else if (digitalRead(enroll) == 0)
    {
      return;
    }
  }
}

uint8_t getFingerprintEnroll()
{
  int p = -1;
  lcd.clear();
  lcd.print("finger ID:");
  lcd.print(id);
  lcd.setCursor(0, 1);
  lcd.print("Place Finger");
  delay(2000);
  while (p != FINGERPRINT_OK)
  {
    p = finger.getImage();
    switch (p)
    {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        lcd.clear();
        lcd.print("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.println("No Finger");
        lcd.clear();
        lcd.print("No Finger");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        lcd.clear();
        lcd.print("Comm Error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        lcd.clear();
        lcd.print("Imaging Error");
        break;
      default:
        Serial.println("Unknown error");
        lcd.clear();
        lcd.print("Unknown Error");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      lcd.clear();
      lcd.print("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      lcd.clear();
      lcd.print("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      lcd.clear();
      lcd.print("Comm Error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      lcd.clear();
      lcd.print("Feature Not Found");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      lcd.clear();
      lcd.print("Feature Not Found");
      return p;
    default:
      Serial.println("Unknown error");
      lcd.clear();
      lcd.print("Unknown Error");
      return p;
  }

  Serial.println("Remove finger");
  lcd.clear();
  lcd.print("Remove Finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
  lcd.clear();
  lcd.print("Place Finger");
  lcd.setCursor(0, 1);
  lcd.print("   Again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.print(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        break;
      default:
        Serial.println("Unknown error");
        return;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  Serial.print("Creating model for #");  Serial.println(id);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
    lcd.clear();
    lcd.print("Stored!");
    delay(2000);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  }
  else {
    Serial.println("Unknown error");
    return p;
  }
}

int getFingerprintIDez()
{
  uint8_t p = finger.getImage();

  if (p != FINGERPRINT_OK)
    return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)
    return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)
  {
    lcd.clear();
    lcd.print("Finger Not Found");
    lcd.setCursor(0, 1);
    lcd.print("Try Later");
    delay(2000);
    return -1;
  }
  // found a match!
  Serial.print("Found ID #");
  Serial.print(finger.fingerID);
  return finger.fingerID;
}

uint8_t deleteFingerprint(uint8_t id)
{
  uint8_t p = -1;
  lcd.clear();
  lcd.print("Please wait");
  p = finger.deleteModel(id);
  if (p == FINGERPRINT_OK)
  {
    Serial.println("Deleted!");
    lcd.clear();
    lcd.print("Figer Deleted");
    lcd.setCursor(0, 1);
    lcd.print("Successfully");
    delay(1000);
  }

  else
  {
    Serial.print("Something Wrong");
    lcd.clear();
    lcd.print("Something Wrong");
    lcd.setCursor(0, 1);
    lcd.print("Try Again Later");
    delay(2000);
    return p;
  }
}

void greenLed() {
  digitalWrite(green, HIGH);
  delay(200);
  digitalWrite(green, LOW);
  delay(200);
  digitalWrite(green, HIGH);
  delay(200);
  digitalWrite(green, LOW);
  delay(200);
  digitalWrite(green, HIGH);
  delay(200);
  digitalWrite(green, LOW);
}

void redLed() {
  digitalWrite(red, HIGH);
  delay(500);
  digitalWrite(red, LOW);
  delay(500);
  digitalWrite(red, HIGH);
  delay(500);
  digitalWrite(red, LOW);
  delay(500);
  digitalWrite(red, HIGH);
  delay(500);
  digitalWrite(red, LOW);
}

void yellowLed() {
  digitalWrite(yellow, HIGH);
  delay(200);
  digitalWrite(yellow, LOW);
  delay(200);
  digitalWrite(yellow, HIGH);
  delay(200);
  digitalWrite(yellow, LOW);
  delay(200);
  digitalWrite(yellow, HIGH);
  delay(200);
  digitalWrite(yellow, LOW);
}

