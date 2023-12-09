# PY32F002A Remote Control

Remote control basing on PY32F002A, XL2400(or XN297LBW), 74HC165, 74HC595

# Schematics And PCB

OSHWHub: [https://oshwhub.com/iosetting/py32f002a-remote-control](https://oshwhub.com/iosetting/py32f002a-remote-control)

# Build

## Makefile

Wireless part selection
```
# Wireless part:
#   XL2400, XN297L
WIRELESS_PART	?= XN297L
```

## Remote Controller

```bash
make -C Apps/RemoteController clean
make -C Apps/RemoteController
```

## Servo Driver Board

```bash
make -C Apps/ServoDriverBoard clean
make -C Apps/ServoDriverBoard
```
