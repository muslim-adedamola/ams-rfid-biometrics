# ams-rfid-biometrics
This repo contains the code that runs the attendance monitoring system I built for my bachelor's thesis in 2020.
The system integrates an Arduino Mega as the controller, a fingerprint sensor, and an RFID module that scans the RFID card of the attendee.
If the fingerprint and RFID of the attendee matches the copy on the databse, attendance is marked. If not, attendance is rejected. An SD card was integrated to serve as the local database.
