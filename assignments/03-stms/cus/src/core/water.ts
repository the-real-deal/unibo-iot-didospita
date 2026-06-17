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
    timeout: NodeJS.Timeout
    level: number
  } | null

  constructor(
    initialState: EventNames<WaterEventsMap>,
    private dangerLevel: number,
    private criticalLevel: number,
    private dangerTimeoutMs: number,
  ) {
    super()
    this.levelState = initialState
    this.dangerEvent = null
  }

  private destroyDangerEvent() {
    if (this.dangerEvent === null) {
      return
    }
    clearTimeout(this.dangerEvent.timeout)
    this.dangerEvent = null
  }

  registerWaterLevel(level: number) {
    console.debug("WATER LEVEL:", level)
    if (level < this.dangerLevel) {
      if (this.levelState !== "safe") {
        this.emit("safe", { level })
      }
      this.levelState = "safe"
      this.destroyDangerEvent()
    } else if (level < this.criticalLevel) {
      if (this.levelState === "critical") {
        this.emit("danger", { level })
        this.levelState = "danger"
      } else if (this.levelState === "safe") {
        if (this.dangerEvent === null) {
          const timeout = setTimeout(() => {
            this.emit("danger", { level: this.dangerEvent!.level })
            this.levelState = "danger"
          }, this.dangerTimeoutMs)
          this.dangerEvent = { timeout, level }
        } else {
          this.dangerEvent.level = level
        }
      }
    } else {
      if (this.levelState !== "critical") {
        this.destroyDangerEvent()
        this.emit("critical", { level })
      }
      this.levelState = "critical"
    }
  }
}
