let envPrefix: string | null = null

export function setEnvPrefix(prefix: string) {
  envPrefix = prefix
}

function sanitizeEnvKey(key: string): string {
  return envPrefix === null ? key : `${envPrefix}_${key}`
}

function getEnv(key: string): string | undefined {
  key = sanitizeEnvKey(key)

  const value = process.env[key]
  if (value === undefined) {
    console.debug(`Env var ${key} not defined`)
    return undefined
  }
  console.debug(`Found env var ${key}=${value}`)
  return value
}

export function getEnvString(key: string): string | undefined {
  return getEnv(key)
}

export function getEnvNumber(key: string): number | undefined {
  const value = getEnv(key)
  if (value === undefined) {
    return undefined
  }
  const num = Number(value)
  if (isNaN(num)) {
    console.debug(`Env var ${key} is not a valid number`)
    return undefined
  } else {
    return num
  }
}

export function getEnvBool(key: string): boolean | undefined {
  const value = getEnv(key)
  if (value === undefined) {
    return undefined
  }
  return value == "" || value == "1" || value.toLowerCase() == "true"
}
