// The NodeJS events api doesn't expose all types

import { EventEmitterOptions, EventEmitter as NodeEventEmitter } from "events"
import { EventEmitter } from "stream"

export type EventsMap<T> = Record<keyof T, any[]>
export type EventNames<T extends EventsMap<T>> = keyof T
export type EventListener<T extends EventsMap<T>, K extends EventNames<T>> = (
  ...args: T[K]
) => void

type EventSourceData<T extends EventsMap<T>, K extends EventNames<T>> = {
  source: string
  eventArgs: T[K]
}

type EventsSourceMap<T extends EventsMap<T>> = {
  [K in keyof T]: [e: EventSourceData<T, K>]
}

type Emitter<T extends EventsMap<T>> = NodeEventEmitter<EventsSourceMap<T>>

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

  protected emit<K extends EventNames<T>>(event: K, source: string, ...args: T[K]) {
    if (!this.enabled) {
      return
    }
    const e = {
      source: source,
      eventArgs: args,
    }
    const emitter = this.eventEmitter as EventEmitter<any>
    emitter.emit(event as any, e)
  }

  on<K extends EventNames<T>>(event: K, source: string, listener: EventListener<T, K>) {
    this.eventEmitter.on(event as any, (e: EventSourceData<T, K>) => {
      if (!this.isEnabled() || e.source === source) {
        return
      }
      listener(...e.eventArgs)
    })
  }
}
