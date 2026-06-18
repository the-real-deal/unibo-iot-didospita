import { Request, Response } from "express"
import { successRes } from "./utils.js"

interface EventStreamEvent {
  id?: string
  event?: string
  data: any
}

export class EventsStream {
  private registeredClients: Set<{ req: Request, res: Response }>

  constructor() {
    this.registeredClients = new Set()
  }

  registerClient(req: Request, res: Response) {
    const client = { req, res }
    this.registeredClients.add(client)
    req.on("close", () => {
      this.registeredClients.delete(client)
    })

    res.setHeader("Content-Type", "text/event-stream")
    res.setHeader("Cache-Control", "no-cache")
    res.setHeader("Connection", "keep-alive")
    res.status(200).flushHeaders()
  }

  sendEvent(streamEvent: EventStreamEvent) {
    const { id, event, data } = streamEvent
    this.registeredClients.forEach(({ res }) => {
      if (id !== undefined) {
        res.write(`id: ${id}\n`)
      }
      if (event !== undefined) {
        res.write(`event: ${event}\n`)
      }
      res.write(`data: ${JSON.stringify(data)}\n\n`)
    })
  }
}
