import express, { Express } from "express"
import http, { Server } from "http"
import { SystemState, SystemStateManager } from "../../core/state.js"
import { parseEnum } from "../../utils/enum.js"
import { DoorManager } from "../../core/door.js"

function setup(
  app: Express,
  systemStateManager: SystemStateManager,
  doorManager: DoorManager,
) {
  app.use(express.json())

  app.get("/state/:state", (req, res) => {
    const state = parseEnum(SystemState, req.params["state"])
    if (state === null) {
      return res.status(400).send("Invalid state")
    }

    console.log("HTTP State:", state)
    systemStateManager.registerSystemState(state)
    return res.status(200).send("ok")
  })

  app.get("/door/:percentage", (req, res) => {
    const percentage = Number(req.params["percentage"])
    if (isNaN(percentage)) {
      return res.status(400).send("Invalid perc")
    }

    console.log("HTTP Door:", percentage)
    doorManager.registerDoorPercentage(percentage)
    return res.status(200).send("ok")
  })
}

export async function startHTTPServer(
  hostname: string,
  port: number,
  systemStateManager: SystemStateManager,
  doorManager: DoorManager,
): Promise<Server> {
  return new Promise((resolve, reject) => {
    try {
      const app = express()
      setup(app, systemStateManager, doorManager)
      const server = http.createServer(app)
      server.listen(port, hostname)
      resolve(server)
    } catch (err) {
      reject(err)
    }
  })
}
