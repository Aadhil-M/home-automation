# MQTT Topics

This document describes the MQTT topics used by the Smart Home Dashboard project.

---

## Command Topics

### Bedroom Light Control

**Topic**

```text
addyhome/bedroom/light
```

**Payload**

```json
{
  "state": 1
}
```

### State Values

| Value | Description |
|---------|-------------|
| 1 | Turn ON the light |
| 0 | Turn OFF the light |

---

## Status Topics

### Bedroom Light Status

**Topic**

```text
addyhome/bedroom/light/status
```

**Payload**

```text
ON
```

or

```text
OFF
```

### Status Values

| Payload | Description |
|----------|-------------|
| ON | Light is ON |
| OFF | Light is OFF |

---

## Heartbeat Topic

### Device Availability

**Topic**

```text
addyhome/status
```

**Payload**

```text
ONLINE
```

### Status Values

| Payload | Description |
|----------|-------------|
| ONLINE | Device is connected and running |

---

## MQTT Topic Structure

```text
addyhome/
├── status
└── bedroom/
    └── light
        └── status
```

---

## Example MQTT Messages

### Turn ON Bedroom Light

**Topic**

```text
addyhome/bedroom/light
```

**Payload**

```json
{
  "state": 1
}
```

### Turn OFF Bedroom Light

**Topic**

```text
addyhome/bedroom/light
```

**Payload**

```json
{
  "state": 0
}
```

### Light Status Update

**Topic**

```text
addyhome/bedroom/light/status
```

**Payload**

```text
ON
```

### Device Heartbeat

**Topic**

```text
addyhome/status
```

**Payload**

```text
ONLINE
```

---

## Notes

- MQTT Broker Port: `1883`
- Payload format for commands: `JSON`
- Payload format for status updates: `Plain Text`
- Heartbeat topic is used to monitor device availability.
