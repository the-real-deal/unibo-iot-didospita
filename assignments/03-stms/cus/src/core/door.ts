import { EventsManager, EventNames } from "./events.js"

export interface DoorEvent {
  percentage: number
}

export interface DoorEventsMap {
  changed: [e: DoorEvent]
}

export class DoorManager extends EventsManager<DoorEventsMap> {
  private percentage: number

  constructor(initialPercentage: number) {
    super()
    this.percentage = initialPercentage
  }

  registerDoorPercentage(percentage: number) {
    if (this.percentage === percentage) {
      return
    }
    this.percentage = percentage
    this.emit("changed", { percentage })
  }
}
