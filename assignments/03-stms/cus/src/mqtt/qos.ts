import { type QoS } from "mqtt-packet";

export { type QoS } from "mqtt-packet";

export function sanitizeQoS(qos: number): QoS | null {
  switch (qos) {
    case 0:
    case 1:
    case 2:
      return qos;
    default:
      return null;
  }
}
