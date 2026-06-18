export function successRes(value: object) {
  return { success: true, data: value }
}

export function errorRes(error: string) {
  return { success: false, data: error }
}
