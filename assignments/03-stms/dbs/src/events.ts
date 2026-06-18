// The NodeJS events api doesn't expose all types
export type EventsMap<T> = Record<keyof T, any[]>
export type EventNames<T extends EventsMap<T>> = keyof T
export type EventListener<T extends EventsMap<T>, K extends EventNames<T>> = (
  ...args: T[K]
) => void

export class EventsManager<T extends EventsMap<T>> {
  private listeners: {
    [K in keyof T]?: EventListener<T, K>[]
  }

  constructor() {
    this.listeners = {}
  }

  protected emit<K extends EventNames<T>>(event: K, ...args: T[K]) {
    const listeners = this.listeners[event]
    if (listeners === undefined) {
      return
    }
    listeners.forEach(listener => listener(...args))
  }

  on<K extends EventNames<T>>(event: K, listener: EventListener<T, K>) {
    if (this.listeners[event] === undefined) {
      this.listeners[event] = []
    }
    this.listeners[event].push(listener)
  }
}
