import { env } from "bun"
import express from "express"

const PORT = (() => {
  try {
    return parseInt(env.CUS_PORT || "")
  } catch {
    return 3000
  }
})()

const app = express()

app.get("/{:name}", (req, res) => {
  const name = req.params.name || "world"
  res.send(`Hello, ${name}!`)
})

export default function startHTTPServer(port: number = PORT) {
  app.listen(PORT, () => {
    console.log(`Example app listening on port http://localhost:${PORT}`)
  })
}
