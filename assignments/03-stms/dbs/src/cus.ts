import { EventsManager } from "./events"

export type SystemState = "MANUAL" | "AUTOMATIC" | "UNCONNECTED"

export type CUSApiEventsMap = {
  [K in SystemState]: []
} & {
  waterLevel: [level: number]
  state: [state: SystemState]
  doorOpening: [percentage: number]
  badResponse: [message: string]
  notreachable: []
  reachable: []
}

export class CUSApi extends EventsManager<CUSApiEventsMap> {
  private endpoint: string
  private fetchTimeoutMs: number
  private pingIntervalMs: number
  private reachable: boolean
  private eventSource: EventSource | null

  constructor(
    endpoint: string,
    fetchTimeoutMs: number,
    pingIntervalMs: number,
  ) {
    super()
    this.endpoint = endpoint
    this.fetchTimeoutMs = fetchTimeoutMs
    this.pingIntervalMs = pingIntervalMs
    this.reachable = false
    this.eventSource = null
  }

  private emitReachable() {
    this.emit("reachable")
    this.reachable = true
  }

  private emitNotReachable() {
    this.emit("notreachable")
    this.reachable = false
  }

  private registerNotReachable() {
    if (!this.reachable) {
      return
    }
    this.emitNotReachable()
  }

  private registerReachable() {
    if (this.reachable) {
      return
    }
    this.emitReachable()
  }

  isReachable(): boolean {
    return this.reachable
  }

  private makePath(path: string) {
    path = path
      .split("/")
      .filter((p) => p.length > 0)
      .join("/")
    return `${this.endpoint}/${path}`
  }

  private async fetchApi(
    path: string,
    init?: RequestInit,
  ): Promise<object | "aborted" | null> {
    try {
      const abortSignal = AbortSignal.timeout(this.fetchTimeoutMs)
      const response = await fetch(this.makePath(path), {
        ...{
          signal: abortSignal,
        },
        ...init,
      })

      this.registerReachable()
      if (response.body === null) {
        return null
      }

      const body = (await response.json()) as {
        success: boolean
        data: object | string
      }
      if (response.ok && body.success) {
        return body.data as object
      } else {
        this.emit("badResponse", body.data as string)
        return null
      }
    } catch {
      this.registerNotReachable()
      return "aborted"
    }
  }

  private async pingEndpoint(): Promise<boolean> {
    return (await this.fetchApi("/", { method: "HEAD" })) != "aborted"
  }

  private async postJSON(
    path: string,
    data: object,
  ): Promise<object | "aborted" | null> {
    return this.fetchApi(path, {
      method: "POST",
      body: JSON.stringify(data),
      headers: { "Content-Type": "application/json" },
    })
  }

  async setState(state: Exclude<SystemState, "UNCONNECTED">): Promise<boolean> {
    const res = await this.postJSON("/state", { state })
    return res !== null
  }

  async setDoorOpening(percentage: number): Promise<boolean> {
    const res = await this.postJSON("/door/opening", { percentage })
    return res !== null
  }

  private resIsOk(res: object | "aborted" | null): res is object {
    return res !== null && res !== "aborted"
  }

  private emitState(state: SystemState) {
    this.emit("state", state)
    this.emit(state)
  }

  private emitWaterLevel(waterLevel: number) {
    this.emit("waterLevel", waterLevel)
  }

  private emitDoorOpening(percentage: number) {
    this.emit("doorOpening", percentage)
  }

  private async resetConnection() {
    const res = await this.fetchApi("/snapshot")
    if (this.resIsOk(res)) {
      const snapshot = res as any
      this.emitState(snapshot.state)
      this.emitWaterLevel(snapshot.waterLevel)
      this.emitDoorOpening(snapshot.doorOpening)
    }
    if (this.eventSource) {
      this.eventSource.close()
    }
    this.eventSource = new EventSource(this.makePath("events"))
    this.eventSource.addEventListener("water/level", (e) => {
      this.registerReachable()
      const waterLevel = JSON.parse(e.data) as number
      this.emitWaterLevel(waterLevel)
    })

    this.eventSource.addEventListener("state", (e) => {
      this.registerReachable()
      const state = JSON.parse(e.data) as SystemState
      this.emitState(state)
    })

    this.eventSource.addEventListener("door/opening", (e) => {
      this.registerReachable()
      const percentage = JSON.parse(e.data) as number
      this.emitDoorOpening(percentage)
    })
  }

  async beginListening() {
    this.on("reachable", () => this.resetConnection())

    this.emitNotReachable()
    setInterval(() => this.pingEndpoint(), this.pingIntervalMs)
    await this.pingEndpoint()
  }
}
