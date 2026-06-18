import { EventsManager, EventNames } from "./events.js"
import { SystemState, SystemStateManager } from "./state.js"

export interface WaterLevelEvent {
  level: number
}

export interface WaterEventsMap {
  safe: [e: WaterLevelEvent]
  danger: [e: WaterLevelEvent]
  critical: [e: WaterLevelEvent]
  new: [e: WaterLevelEvent]
}

export class WaterMonitor extends EventsManager<WaterEventsMap> {
  private level: number
  private levelState: EventNames<WaterEventsMap>
  private dangerTimeout: NodeJS.Timeout | null

  constructor(
    initialLevel: number,
    private dangerLevel: number,
    private criticalLevel: number,
    private dangerTimeoutMs: number,
  ) {
    super()
    this.level = initialLevel
    if (initialLevel < this.dangerLevel) {
      this.levelState = "safe"
    } else if (initialLevel < this.criticalLevel) {
      this.levelState = "danger"
    } else {
      this.levelState = "critical"
    }
    this.dangerTimeout = null
  }

  getLevel() {
    return this.level
  }

  private destroyDangerTimeout() {
    if (this.dangerTimeout === null) {
      return
    }
    clearTimeout(this.dangerTimeout)
    this.dangerTimeout = null
  }

  registerWaterLevel(level: number) {
    console.debug("WATER LEVEL:", level)
    const e = { level }
    this.level = level
    this.emit("new", e)

    if (level < this.dangerLevel) {
      if (this.levelState !== "safe") {
        this.emit("safe", e)
      }
      this.levelState = "safe"
      this.destroyDangerTimeout()
    } else if (level < this.criticalLevel) {
      if (this.levelState === "critical") {
        this.emit("danger", e)
        this.levelState = "danger"
      } else if (this.levelState === "safe") {
        if (this.dangerTimeout !== null) {
          return
        }
        const timeout = setTimeout(() => {
          this.emit("danger", {
            level: this.level,
          })
          this.levelState = "danger"
        }, this.dangerTimeoutMs)
        this.dangerTimeout = timeout
      }
    } else {
      if (this.levelState !== "critical") {
        this.destroyDangerTimeout()
        this.emit("critical", e)
      }
      this.levelState = "critical"
    }
  }
}
