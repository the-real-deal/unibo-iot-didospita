import express, { Express } from "express"
import http, { Server } from "http"

function setup(app: Express) {
  app.use(express.json())

  app.get("/{:name}", (req, res) => {
    const name = req.params.name || "world"
    console.log("HTTP hello:", name)
    res.send(`Hello, ${name}!`)
  })
}

export async function startHTTPServer(
  hostname: string,
  port: number,
): Promise<Server> {
  return new Promise((resolve, reject) => {
    try {
      const app = express()
      setup(app)
      const server = http.createServer(app)
      server.listen(port, hostname)
      resolve(server)
    } catch (err) {
      reject(err)
    }
  })
}
