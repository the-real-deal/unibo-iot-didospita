import mqtt, { MqttClient, type IClientSubscribeOptions } from "mqtt"
import {
  subscribeToTopics,
  type TopicCallbacksMap,
  type TopicSubscriptionOptions,
} from "../../mqtt/sub.js"
import { WaterMonitor } from "../../core/water.js"

function defineCallbacks(waterMonitor: WaterMonitor): TopicCallbacksMap {
  return {
    "water/level": (_, payload, __) => {
      try {
        const waterLevel = parseFloat(payload.toString())
        waterMonitor.registerWaterLevel(waterLevel)
      } catch (e) {
        console.error("Error parsing MQTT water level payload:", e)
      }
    },
  }
}

export interface MQTTClientStartOptions {
  topicSubscription?: TopicSubscriptionOptions
}

export async function startMQTTClient(
  brokerURL: string,
  waterMonitor: WaterMonitor,
  subscribeOptions: IClientSubscribeOptions,
  options: MQTTClientStartOptions = {},
): Promise<MqttClient> {
  const endpoint = `mqtt://${brokerURL}`
  console.debug(`Connecting to ${endpoint}`)
  const client = await mqtt.connectAsync(endpoint)
  console.debug(`MQTT client connected`)
  const callbacks = defineCallbacks(waterMonitor)
  await subscribeToTopics(
    client,
    subscribeOptions,
    callbacks,
    options.topicSubscription,
  )
  return client
}
