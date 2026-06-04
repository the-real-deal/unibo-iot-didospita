export enum EnvVarType {
  String,
  Number,
  Boolean,
}

let envPrefix: string | null = null

export function setEnvPrefix(prefix: string) {
  envPrefix = prefix
}

function sanitizeEnvKey(key: string): string {
  return envPrefix === null ? key : `${envPrefix}_${key}`
}

export function getEnv(key: string, type: EnvVarType.String): string | undefined
export function getEnv(key: string, type: EnvVarType.Number): number | undefined
export function getEnv(
  key: string,
  type: EnvVarType.Boolean,
): boolean | undefined
export function getEnv(
  key: string,
  type: EnvVarType,
): string | number | boolean | undefined {
  key = sanitizeEnvKey(key)

  const envValue = process.env[key]
  if (envValue === undefined) {
    console.debug(`Env var ${key} not defined`)
    return undefined
  }
  console.debug(`Found env var ${key}=${envValue}`)

  switch (type) {
    case EnvVarType.String:
      return envValue
    case EnvVarType.Number:
      const num = Number(envValue)
      if (isNaN(num)) {
        console.debug(`Env var ${key} is not a valid number`)
        return undefined
      } else {
        return num
      }
    case EnvVarType.Boolean:
      return (
        envValue == "" || envValue == "1" || envValue.toLowerCase() == "true"
      )
  }
}
