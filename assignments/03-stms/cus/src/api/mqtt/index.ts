import mqtt, { MqttClient, type IClientSubscribeOptions } from "mqtt"
import {
  subscribeToTopics,
  type TopicCallbacksMap,
  type TopicSubscriptionOptions,
} from "../../mqtt/subscription.js"

const callbacks: TopicCallbacksMap = {
  hello: (topic, payload, packet) => {
    console.log("Message arrived:", payload.toString())
  },
}

export interface MQTTClientStartOptions {
  topicSubscription?: TopicSubscriptionOptions
}

export async function startMQTTClient(
  brokerURL: string,
  subscribeOptions: IClientSubscribeOptions,
  options: MQTTClientStartOptions = {},
): Promise<MqttClient> {
  const endpoint = `mqtt://${brokerURL}`
  console.debug(`Connecting to ${endpoint}`)
  const client = await mqtt.connectAsync(endpoint)
  console.debug(`MQTT client connected`)
  await subscribeToTopics(
    client,
    subscribeOptions,
    callbacks,
    options.topicSubscription,
  )
  return client
}
