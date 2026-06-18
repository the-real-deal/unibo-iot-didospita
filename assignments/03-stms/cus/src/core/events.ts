// The NodeJS events api doesn't expose all types

import { EventEmitterOptions, EventEmitter as NodeEventEmitter } from "events"
import { EventEmitter } from "stream"

export type EventsMap<T> = Record<keyof T, any[]>
export type EventNames<T extends EventsMap<T>> = keyof T
export type EventListener<T extends EventsMap<T>, K extends EventNames<T>> = (
  ...args: T[K]
) => void

type Emitter<T extends EventsMap<T>> = NodeEventEmitter<T>

export class EventsManager<T extends EventsMap<T>> {
  private eventEmitter: Emitter<T>
  private enabled: boolean

  constructor(options?: EventEmitterOptions) {
    this.eventEmitter = new NodeEventEmitter(options)
    this.enabled = true
  }

  enable() {
    this.enabled = true
  }

  disable() {
    this.enabled = false
  }

  isEnabled() {
    return this.enabled
  }

  protected emit<K extends EventNames<T>>(event: K, ...args: T[K]) {
    if (!this.enabled) {
      return
    }
    this.eventEmitter.emit(event as any, ...args as any)
  }

  on<K extends EventNames<T>>(event: K, listener: EventListener<T, K>) {
    this.eventEmitter.on(event as any, (...args: T[K]) => {
      if (!this.isEnabled()) {
        return
      }
      listener(...args)
    })
  }
}
