export function parseEnum<T extends Record<string, string>>(
  enumObj: T,
  value: string,
): T[keyof T] | null {
  return Object.values(enumObj).includes(value)
    ? (value as T[keyof T])
    : null
}
