import express from "express"
import http, { Server } from "http"

const app = express()

app.get("/{:name}", (req, res) => {
  const name = req.params.name || "world"
  res.send(`Hello, ${name}!`)
})

export async function startHTTPServer(hostname: string, port: number): Promise<Server> {
  return new Promise((resolve, reject) => {
    try {
      const server = http.createServer(app)
      server.listen(port, hostname)
      resolve(server)
    } catch (err) {
      reject(err)
    }
  })
}
