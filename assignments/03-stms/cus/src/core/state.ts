import { EventsManager, EventNames } from "./events.js"

export enum SystemState {
  Manual = "MANUAL",
  Automatic = "AUTOMATIC",
  Unconnected = "UNCONNECTED",
}

export type SystemStateEventsMap = Record<SystemState, [prev: SystemState]> & {
  changed: [state: SystemState, prev: SystemState]
}

export class SystemStateManager extends EventsManager<SystemStateEventsMap> {
  private state: SystemState

  constructor(initialState: SystemState) {
    super()
    this.state = initialState
  }

  getState(): SystemState {
    return this.state
  }

  registerSystemState(eventsSource: string, state: SystemState) {
    if (this.state === state) {
      return
    }
    const prev = this.state
    this.state = state
    this.emit(state, eventsSource, prev)
    this.emit("changed", eventsSource, state, prev)
  }
}
