import express from "express"
import { EnvVarType, getEnv } from "../../utils/env"
import http, { Server } from "http"

const HOSTNAME = getEnv("HTTP_HOSTNAME", EnvVarType.String) ?? "localhost"
const PORT = getEnv("HTTP_PORT", EnvVarType.Number) ?? 3000

const app = express()

app.get("/{:name}", (req, res) => {
  const name = req.params.name || "world"
  res.send(`Hello, ${name}!`)
})

interface HTTPServerStartOptions {
  hostname?: string
  port?: number
}
export async function startHTTPServer(
  options: HTTPServerStartOptions = {},
): Promise<Server> {
  const { port, hostname } = {
    ...{ port: PORT, hostname: HOSTNAME },
    ...options,
  }

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
