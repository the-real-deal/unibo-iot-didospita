export function sanitizeTopic(topic: string): string {
  return topic
    .trim()
    .split("/")
    .filter((s) => s.length > 0)
    .join("/")
}

export function joinTopics(...topics: string[]): string {
  return topics.map(sanitizeTopic).join("/")
}
