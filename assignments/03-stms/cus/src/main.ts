import { startHTTPServer } from "./api/http"
import { startMQTTClient } from "./api/mqtt"
import { startSerialPort } from "./api/serial"
import { sanitizeQoS } from "./mqtt/qos"
import { EnvVarType, getEnv, setEnvPrefix } from "./utils/env"
import { serverAddressString } from "./utils/http"

setEnvPrefix("IOT_ASSIGNMENT_O3")

const HTTP_HOSTNAME = getEnv("HTTP_HOSTNAME", EnvVarType.String) ?? "0.0.0.0"
const HTTP_PORT = getEnv("HTTP_PORT", EnvVarType.Number) ?? 3000

const MQTT_BROKER_URL =
  getEnv("MQTT_BROKER_URL", EnvVarType.String) ?? "broker.mqtt-dashboard.com"
const MQTT_BASE_TOPIC =
  getEnv("MQTT_BASE_TOPIC", EnvVarType.String) ?? "unibo-iot-didospita"
const MQTT_QOS = (() => {
  const qos = getEnv("MQTT_QOS", EnvVarType.Number)
  return qos === undefined ? 0 : (sanitizeQoS(qos) ?? 0)
})()

const SERIAL_PORT = getEnv("SERIAL_PORT", EnvVarType.String)
const SERIAL_BAUD_RATE = getEnv("SERIAL_BAUD_RATE", EnvVarType.Number) ?? 9600

try {
  const httpServer = await startHTTPServer(HTTP_HOSTNAME, HTTP_PORT)
  const httpAddress = serverAddressString(httpServer.address())
  if (httpAddress === null) {
    console.warn("Failed to get http server address")
  } else {
    console.info("HTTP server started at:", httpAddress)
  }
} catch (err) {
  console.error("Failed to start http server")
}

try {
  await startMQTTClient(
    MQTT_BROKER_URL,
    {
      qos: MQTT_QOS,
    },
    {
      topicSubscription: {
        baseTopic: MQTT_BASE_TOPIC,
      },
    },
  )
  console.info("MQTT client started")
} catch (err) {
  console.error("Failed to start MQTT client")
}

try {
  const serialPort = await startSerialPort(SERIAL_BAUD_RATE, {
    path: SERIAL_PORT,
  })
  console.info("Serial port attached at:", serialPort.path)
} catch (err) {
  console.error("Failed to attach serial port")
}
