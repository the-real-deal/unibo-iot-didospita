import { WaterMonitor } from "../core/water.js"
import { QoS } from "../mqtt/qos.js"
import { serverAddressString } from "../utils/http.js"
import { startHTTPServer } from "./http/index.js"
import { startMQTTClient } from "./mqtt/index.js"
import { startSerialServer } from "./serial/index.js"

export interface ServersOptions {
  http: {
    hostname: string
    port: number
  }
  mqtt: {
    brokerURL: string
    qos: QoS
    baseTopic: string
  }
  serial: {
    path?: string
    baudRate: number
  }
}

export async function startAllServers(
  waterMonitor: WaterMonitor,
  options: ServersOptions,
) {
  try {
    const httpServer = await startHTTPServer(
      options.http.hostname,
      options.http.port,
    )
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
      options.mqtt.brokerURL,
      {
        qos: options.mqtt.qos,
      },
      waterMonitor,
      {
        topicSubscription: {
          baseTopic: options.mqtt.baseTopic,
        },
      },
    )
    console.info("MQTT client started")
  } catch (err) {
    console.error("Failed to start MQTT client")
  }

  try {
    const serialServer = await startSerialServer(
      options.serial.baudRate,
      waterMonitor,
      {
        path: options.serial.path,
      },
    )
    console.info("Serial server started at:", serialServer.path())
  } catch (err) {
    console.error("Failed to start serial server")
  }
}
