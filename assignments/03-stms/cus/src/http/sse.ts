import { Request, Response } from "express"

interface EventStreamEvent {
  id?: string
  type?: string
  data: any
}

export class EventStream {
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
  }

  sendEvent(event: EventStreamEvent) {
    const { id, type, data } = event
    this.registeredClients.forEach(({ res }) => {
      if (id !== undefined) {
        res.write(`id: ${id}\n`)
      }
      if (type !== undefined) {
        res.write(`type: ${type}\n`)
      }
      res.write(`data: ${JSON.stringify(data)}\n\n`)
    })
  }
}
