import { startHTTPServer } from "./api/http/index.js"
import { startMQTTClient } from "./api/mqtt/index.js"
import { startSerialServer } from "./api/serial/index.js"
import { sanitizeQoS } from "./mqtt/qos.js"
import { getEnvNumber, getEnvString, setEnvPrefix } from "./utils/env.js"
import { serverAddressString } from "./utils/http.js"

setEnvPrefix("IOT_ASSIGNMENT_O3")

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
const SERIAL_BAUD_RATE = getEnvNumber("SERIAL_BAUD_RATE") ?? 115200

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
  const [serialServer, path] = await startSerialServer(SERIAL_BAUD_RATE, {
    path: SERIAL_PORT,
  })
  console.info("Serial server started at:", path)
} catch (err) {
  console.error("Failed to start serial server")
}
