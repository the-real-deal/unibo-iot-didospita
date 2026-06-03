const ENV_PREFIX = "IOT_ASSIGNMENT_O3_CUS"

export enum EnvVarType {
  String,
  Number,
  Boolean,
}

export function getEnv(key: string, type: EnvVarType.String): string | undefined
export function getEnv(key: string, type: EnvVarType.Number): number | undefined
export function getEnv(key: string, type: EnvVarType.Boolean): boolean | undefined
export function getEnv(
  key: string,
  type: EnvVarType,
): string | number | boolean | undefined {
  key = `${ENV_PREFIX}_${key}`

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
