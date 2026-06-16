import { startAllServers } from "./api/index.js"
import { WaterMonitor } from "./core/water.js"
import { sanitizeQoS } from "./mqtt/qos.js"
import { getEnvNumber, getEnvString, setEnvPrefix } from "./utils/env.js"

setEnvPrefix("IOT_ASSIGNMENT_O3")

const DANGER_WATER_LEVEL = getEnvNumber("DANGER_WATER_LEVEL") ?? 0.4 // L1
const CRITICAL_WATER_LEVEL = getEnvNumber("CRITICAL_WATER_LEVEL") ?? 0.7 // L2
const DANGER_TIMEOUT_MS = getEnvNumber("DANGER_TIMEOUT_MS") ?? 3000 // T1

const HTTP_HOSTNAME = getEnvString("HTTP_HOSTNAME") ?? "0.0.0.0"
const HTTP_PORT = getEnvNumber("HTTP_PORT") ?? 3000

const MQTT_BROKER_URL =
  getEnvString("MQTT_BROKER_URL") ?? "broker.mqtt-dashboard.com"
const MQTT_BASE_TOPIC =
  getEnvString("MQTT_BASE_TOPIC") ?? "unibo-iot-didospita/assignment03"
const MQTT_QOS = (() => {
  const envQoS = getEnvNumber("MQTT_QOS")
  if (envQoS === undefined) {
    return 0
  }
  const qos = sanitizeQoS(envQoS)
  if (qos === null) {
    throw new Error(`Invalid QoS value: ${envQoS}`)
  }
  return qos
})()

const SERIAL_PORT = getEnvString("SERIAL_PORT")
const SERIAL_BAUD_RATE = getEnvNumber("SERIAL_BAUD_RATE") ?? 9600

const waterMonitor = new WaterMonitor(
  DANGER_WATER_LEVEL,
  CRITICAL_WATER_LEVEL,
  DANGER_TIMEOUT_MS,
)

await startAllServers(waterMonitor, {
  http: {
    hostname: HTTP_HOSTNAME,
    port: HTTP_PORT,
  },
  mqtt: {
    brokerURL: MQTT_BROKER_URL,
    qos: MQTT_QOS,
    baseTopic: MQTT_BASE_TOPIC,
  },
  serial: {
    path: SERIAL_PORT,
    baudRate: SERIAL_BAUD_RATE,
  },
})
