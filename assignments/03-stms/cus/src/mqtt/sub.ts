import {
  MqttClient,
  type IClientSubscribeOptions,
  type OnMessageCallback,
} from "mqtt"
import { joinTopics, sanitizeTopic } from "./topic.js"

export type TopicCallbacksMap = {
  [topic: string]: OnMessageCallback
}

export interface TopicSubscriptionOptions {
  baseTopic?: string
}

export async function subscribeToTopics(
  client: MqttClient,
  subscribeOptions: IClientSubscribeOptions,
  callbacks: TopicCallbacksMap,
  options: TopicSubscriptionOptions = {},
) {
  callbacks = sanitizeCallbacksMap(callbacks, options.baseTopic)
  const topics = Object.keys(callbacks)
  console.debug("Subscribing to topics:", topics)

  const rejectedTopics = await subscribeClient(client, topics, subscribeOptions)
  if (rejectedTopics.length > 0) {
    throw new Error(
      `Subscription to some topics was rejected: ${rejectedTopics}`,
    )
  }
  console.debug("Subscribed successfully to all requested topics")
  attachCallbacks(client, callbacks)
  console.debug("Topics handlers attached")
}

export function sanitizeCallbacksMap(
  callbacks: TopicCallbacksMap,
  baseTopic?: string,
): TopicCallbacksMap {
  return Object.fromEntries(
    Object.entries(callbacks).map(([topic, handler]) => [
      baseTopic === undefined
        ? sanitizeTopic(topic)
        : joinTopics(baseTopic, topic),
      handler,
    ]),
  )
}

async function subscribeClient(
  client: MqttClient,
  topics: string[],
  subscribeOptions: IClientSubscribeOptions,
): Promise<string[]> {
  const subscriptionGrants = await client.subscribeAsync(
    topics,
    subscribeOptions,
  )
  const grantedTopics = new Set(subscriptionGrants.map((g) => g.topic))
  const rejectedTopics = topics.filter((t) => !grantedTopics.has(t))
  return rejectedTopics
}

function attachCallbacks(client: MqttClient, callbacks: TopicCallbacksMap) {
  client.on("message", (topic, payload, packet) => {
    console.info(`Received mqtt message at ${topic}:`, {
      cmd: packet.cmd,
      qos: packet.qos,
      payloadSize: payload.length,
    })
    const callback = callbacks[topic]
    if (callback !== undefined) {
      callback(topic, payload, packet)
    }
  })
}
