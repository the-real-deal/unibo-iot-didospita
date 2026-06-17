import { DoorManager } from "../core/door.js"
import { SystemStateManager } from "../core/state.js"
import { WaterMonitor } from "../core/water.js"
import { QoS } from "../mqtt/qos.js"
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
    unconnectedTimeoutMs: number
  }
  serial: {
    path?: string
    baudRate: number
  }
}

export async function startAllServers(
  waterMonitor: WaterMonitor,
  systemStateManager: SystemStateManager,
  doorManager: DoorManager,
  options: ServersOptions,
) {
  try {
    const { hostname, port } = options.http
    const httpServer = await startHTTPServer(
      hostname,
      port,
      systemStateManager,
      doorManager,
    )
    console.info(`HTTP server started at: http://${hostname}:${port}`)
  } catch (err) {
    console.error("Failed to start http server")
  }

  try {
    await startMQTTClient(
      options.mqtt.brokerURL,
      options.mqtt.unconnectedTimeoutMs,
      waterMonitor,
      systemStateManager,
      {
        qos: options.mqtt.qos,
      },
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
      systemStateManager,
      doorManager,

      {
        path: options.serial.path,
      },
    )
    console.info("Serial server started at:", serialServer.path())
  } catch (err) {
    console.error("Failed to start serial server")
  }
}
