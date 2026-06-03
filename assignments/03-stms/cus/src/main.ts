import { startHTTPServer } from "./api/http"
import { connectToSerial, findSerialDevice } from "./api/serial"
import { serverAddressString } from "./utils/http"

const httpServer = await startHTTPServer()
const httpAddress = serverAddressString(httpServer.address())
if (httpAddress === undefined) {
  console.warn("Failed to get http server address")
} else {
  console.log("HTTP server started at:", httpAddress)
}

const portInfo = await findSerialDevice()
if (portInfo === undefined) {
  console.warn("No serial device found")
} else {
  console.info("Serial device found at:", portInfo.path)
  connectToSerial(portInfo.path)
  console.info("Listening on serial port:", portInfo.path)
}
