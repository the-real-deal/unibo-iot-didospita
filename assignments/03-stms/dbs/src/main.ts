import Chart from "chart.js/auto"
import { CUSApi, type SystemState } from "./cus"

// configuration can't come from env since this is pure client
const CUS_ENDPOINT = "http://localhost:3000"
const CUS_FETCH_TIMEOUT_MS = 3000
const CUS_PING_INTERVAL_MS = 1000
const WATER_CHART_MAX_MEASUREMENTS = 20

const stateDisplay = document.getElementById("state-display") as HTMLElement
const valveDisplay = document.getElementById("valve-display") as HTMLElement
const waterLevelDisplay = document.getElementById(
  "water-level-display",
) as HTMLElement
const btnStateSwitch = document.getElementById(
  "btn-state-switch",
) as HTMLButtonElement
const valveSlider = document.getElementById("valve-slider") as HTMLInputElement
const waterChartCanvas = document.getElementById(
  "waterChart",
) as HTMLCanvasElement

const waterChart = new Chart(waterChartCanvas, {
  type: "line",
  data: {
    labels: [] as string[],
    datasets: [
      {
        label: "Water Level",
        data: [] as number[],
        fill: true,
      },
    ],
  },
  options: {
    responsive: true,
    scales: {
      y: { min: 0, max: 100 },
    },
  },
})

const cusApi = new CUSApi(
  CUS_ENDPOINT,
  CUS_FETCH_TIMEOUT_MS,
  CUS_PING_INTERVAL_MS,
)

function percentageDisplay(percentage: number): number {
  return Math.round(Math.min(Math.max(0, percentage), 1) * 100)
}

function percentageString(percentage: number): string {
  return `${percentageDisplay(percentage)}%`
}

cusApi.on("badResponse", (e) => alert(e))

let prevState: SystemState | null = null
let btnStateSwitchPrevDisabled = true
let valveSliderPrevDisabled = true

function setState(state: SystemState) {
  prevState = state
  stateDisplay.textContent = state
}

function setBtnSwitchEnabled(enabled: boolean) {
  const disabled = !enabled
  btnStateSwitchPrevDisabled = btnStateSwitch.disabled
  btnStateSwitch.disabled = disabled
}

function setValveSliderEnabled(enabled: boolean) {
  const disabled = !enabled
  valveSliderPrevDisabled = valveSlider.disabled
  valveSlider.disabled = disabled
}

btnStateSwitch.addEventListener("click", () => {
  if (prevState !== "AUTOMATIC" && prevState !== "MANUAL") {
    return
  }
  cusApi.setState(prevState === "MANUAL" ? "AUTOMATIC" : "MANUAL")
})

valveSlider.addEventListener("change", () => {
  const percentage = valveSlider.valueAsNumber / Number(valveSlider.max)
  cusApi.setDoorOpening(percentage)
})

cusApi.on("notreachable", () => {
  stateDisplay.textContent = "NOT REACHABLE"
  setBtnSwitchEnabled(false)
  setValveSliderEnabled(false)
})

cusApi.on("reachable", () => {
  stateDisplay.textContent = prevState ?? "REACHABLE"
  btnStateSwitch.disabled = btnStateSwitchPrevDisabled
  valveSlider.disabled = valveSliderPrevDisabled
})

cusApi.on("state", (state) => {
  setState(state)
  switch (state) {
    case "UNCONNECTED":
      setBtnSwitchEnabled(false)
      setValveSliderEnabled(false)
      break
    case "MANUAL":
      setBtnSwitchEnabled(true)
      setValveSliderEnabled(true)
      break
    case "AUTOMATIC":
      setBtnSwitchEnabled(true)
      setValveSliderEnabled(false)
      break
  }
})

cusApi.on("doorOpening", (percentage) => {
  valveDisplay.textContent = percentageString(percentage)
  valveSlider.value = percentageDisplay(percentage).toString()
})

cusApi.on("waterLevel", (level) => {
  waterLevelDisplay.textContent = percentageString(level)
  const now = new Date()

  waterChart.data.labels?.push(now.toLocaleTimeString())
  waterChart.data.datasets[0].data.push(percentageDisplay(level))

  if ((waterChart.data.labels?.length || 0) > WATER_CHART_MAX_MEASUREMENTS) {
    waterChart.data.labels?.shift()
    waterChart.data.datasets[0].data.shift()
  }
  waterChart.update()
})

await cusApi.beginListening()
