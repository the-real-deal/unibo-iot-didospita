import { type AddressInfo } from "net"

export function serverAddressString(
  address: AddressInfo | string | null,
): string | null {
  switch (typeof address) {
    case "object":
      return address === null
        ? null
        : `http://${address.address}:${address.port}`
    case "string":
      return address
    default:
      return null
  }
}
