import type { AddressInfo } from "net"

export function serverAddressString(
  address: AddressInfo | string | null,
): string | undefined {
  switch (typeof address) {
    case "object":
      return address === null
        ? undefined
        : `http://${address.address}:${address.port}`
    case "string":
      return address
    default:
      return undefined
  }
}
