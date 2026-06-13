# Smart Mailbox

A 3-node Arduino system for secure mailbox access and real-time mail/door notifications.

The Smart Mailbox system employs three Arduino devices to ensure secure access to a mailbox and offers real-time notifications about the presence of new mail. The project includes a default pin that users can choose to share or keep private. When accessed safely, an LCD display notifies the user about the mailbox access or the arrival of new mail. This notification also occurs if the user has previously unlocked their mailbox. Manual lock and unlock functions can be controlled through pushbuttons connected to one of the Arduinos. In case of unauthorized access, the LCD screen displays a warning message to alert the user.

## Architecture

```
┌─────────────────────┐     serial      ┌─────────────────────┐
│  Arduino 2          │ ──────────────► │  Arduino 1          │
│  Sensor Node        │   '0'/'1' mail  │  House Controller   │
│  (HC-SR04, reed)    │   '2'/'3' door  │  (LCD, button)      │
└─────────────────────┘                 └──────────┬──────────┘
                                                   │ '4' unlock
                                                   ▼
                                        ┌─────────────────────┐
                                        │  Arduino 3          │
                                        │  Access Control     │
                                        │  (keypad, servo)    │
                                        └─────────────────────┘
```

## Sketches

| Folder | Role | Hardware |
|--------|------|----------|
| [`arduino1-house-controller/`](arduino1-house-controller/) | Central hub + LCD UI | 16×2 LCD, unlock button |
| [`arduino2-sensor-node/`](arduino2-sensor-node/) | Mail & door sensing | HC-SR04 ultrasonic, magnetic reed switch |
| [`arduino3-access-control/`](arduino3-access-control/) | PIN auth + lock | 4×4 keypad, servo, LED |

Open each folder as a separate sketch in the Arduino IDE and flash to its board.

## Serial Protocol

| Command | Direction | Meaning |
|---------|-----------|---------|
| `'0'` | Sensor → Controller | No mail |
| `'1'` | Sensor → Controller | Mail detected |
| `'2'` | Sensor → Controller | Door closed |
| `'3'` | Sensor → Controller | Door open |
| `'4'` | Controller → Access | Remote unlock |

Baud rate: **9600**

## Original Source

The combined reference file [`arduino.c`](arduino.c) contains all three sketches in one file.
