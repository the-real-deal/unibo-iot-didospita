import { env } from "bun"
import express from "express"

const PORT = env.CUS_PORT || 3000

const app = express()

app.get("/{:name}", (req, res) => {
  const name = req.params.name || "world"
  res.send(`Hello, ${name}!`)
})

app.listen(PORT, () => {
  console.log(`Example app listening on port http://localhost:${PORT}`)
})
