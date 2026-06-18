import mqtt, { MqttClient, type IClientSubscribeOptions } from "mqtt"
import {
  subscribeToTopics,
  type TopicCallbacksMap,
  type TopicSubscriptionOptions,
} from "../../mqtt/sub.js"
import { WaterMonitor } from "../../core/water.js"
import { SystemState, SystemStateManager } from "../../core/state.js"

function defineCallbacks(
  unconnectedTimeoutMs: number,
  waterMonitor: WaterMonitor,
  systemStateManager: SystemStateManager,
): [() => void, TopicCallbacksMap] {
  let unconnectedTimeout: NodeJS.Timeout | null = null
  let beforeUnconnectedState: SystemState | null = null

  systemStateManager.on(SystemState.Unconnected, (prev) => {
    beforeUnconnectedState = prev
  })

  function resetTimeout() {
    if (unconnectedTimeout !== null) {
      clearTimeout(unconnectedTimeout)
      if (beforeUnconnectedState !== null) {
        systemStateManager.registerSystemState(beforeUnconnectedState)
        beforeUnconnectedState = null
      }
    }
    unconnectedTimeout = setTimeout(() => {
      systemStateManager.registerSystemState(SystemState.Unconnected)
      unconnectedTimeout = null
    }, unconnectedTimeoutMs)
  }

  return [
    resetTimeout,
    {
      "water/level": (_, payload, __) => {
        resetTimeout()
        try {
          const waterLevel = parseFloat(payload.toString())
          waterMonitor.registerWaterLevel(waterLevel)
        } catch (e) {
          console.error("Error parsing MQTT water level payload:", e)
        }
      },
    },
  ]
}

export interface MQTTClientStartOptions {
  topicSubscription?: TopicSubscriptionOptions
}

export async function startMQTTClient(
  brokerURL: string,
  unconnectedTimeoutMs: number,
  waterMonitor: WaterMonitor,
  systemStateManager: SystemStateManager,
  subscribeOptions: IClientSubscribeOptions,
  options: MQTTClientStartOptions = {},
): Promise<MqttClient> {
  const endpoint = `mqtt://${brokerURL}`
  console.debug("Connecting to:", endpoint)
  const client = await mqtt.connectAsync(endpoint)
  console.debug(`MQTT client connected`)
  const [resetTimeout, callbacks] = defineCallbacks(
    unconnectedTimeoutMs,
    waterMonitor,
    systemStateManager,
  )
  await subscribeToTopics(
    client,
    subscribeOptions,
    callbacks,
    options.topicSubscription,
  )
  resetTimeout()
  return client
}
