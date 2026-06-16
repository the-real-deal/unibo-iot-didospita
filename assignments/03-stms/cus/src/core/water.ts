import { EventsManager, EventNames } from "./events.js"

export interface WaterLevelEvent {
  level: number
}

export interface WaterEventsMap {
  safe: [e: WaterLevelEvent]
  danger: [e: WaterLevelEvent]
  critical: [e: WaterLevelEvent]
}

export class WaterMonitor extends EventsManager<WaterEventsMap> {
  private levelState: EventNames<WaterEventsMap>
  private dangerEvent: {
    timeoutId: NodeJS.Timeout
    level: number
  } | null

  constructor(
    private dangerLevel: number,
    private criticalLevel: number,
    private dangerTimeoutMs: number,
  ) {
    super()
    this.levelState = "safe"
    this.dangerEvent = null
  }

  registerWaterLevel(level: number) {
    console.debug("Water level:", level)
    if (level < this.dangerLevel) {
      if (this.levelState !== "safe") {
        this.emit("safe", { level })
      }
      this.levelState = "safe"
      if (this.dangerEvent === null) {
        return
      }
      clearTimeout(this.dangerEvent.timeoutId)
      this.dangerEvent = null
    } else if (level < this.criticalLevel) {
      if (this.levelState === "critical") {
        return
      }
      this.levelState = "danger"
      if (this.dangerEvent === null) {
        const timeoutId = setTimeout(() => {
          this.emit("danger", {
            level: this.dangerEvent!.level,
          })
        }, this.dangerTimeoutMs)
        this.dangerEvent = { timeoutId, level }
      } else {
        this.dangerEvent.level = level
      }
    } else {
      if (this.levelState !== "critical") {
        this.emit("critical", { level })
      }
      this.levelState = "critical"
    }
  }
}
