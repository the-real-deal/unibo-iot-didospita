import {
  MqttClient,
  type IClientSubscribeOptions,
  type OnMessageCallback,
} from "mqtt"
import { joinTopics, sanitizeTopic } from "./topic"

export type TopicCallbacksMap = Dict<OnMessageCallback>

export interface TopicSubscriptionOptions {
  baseTopic?: string
}

export async function subscribeToTopics(
  client: MqttClient,
  subscribeOptions: IClientSubscribeOptions,
  callbacks: TopicCallbacksMap,
  options: TopicSubscriptionOptions = {},
) {
  const callbacksMap = sanitizeCallbacksMap(callbacks, options.baseTopic)
  const topics = Object.keys(callbacksMap)
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

function sanitizeCallbacksMap(
  handlers: TopicCallbacksMap,
  baseTopic?: string,
): TopicCallbacksMap {
  return Object.fromEntries(
    Object.entries(handlers).map(([topic, handler]) => [
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

function attachCallbacks(client: MqttClient, handlers: TopicCallbacksMap) {
  client.on("message", (topic, payload, packet) => {
    const handler = handlers[topic]
    if (handler !== undefined) {
      handler(topic, payload, packet)
    }
  })
}
