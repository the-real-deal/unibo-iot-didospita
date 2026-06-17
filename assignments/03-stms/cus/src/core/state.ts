import { EventsManager, EventNames } from "./events.js"

export enum SystemState {
  Manual = "MANUAL",
  Automatic = "AUTOMATIC",
  Unconnected = "UNCONNECTED",
}

export type SystemStateEventsMap = Record<SystemState, []> & {
  changed: [state: SystemState]
}

export class SystemStateManager extends EventsManager<SystemStateEventsMap> {
  private state: SystemState

  constructor(initialState: SystemState) {
    super()
    this.state = initialState
  }

  registerSystemState(state: SystemState) {
    if (this.state === state) {
      return
    }
    this.state = state
    this.emit(state)
    this.emit("changed", state)
  }
}
