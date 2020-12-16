import axios from 'axios'

const baseURL = 'http://localhost:3000'
// const baseURL = 'http://39.99.134.43:3000'

export const Axios = axios.create({
  baseURL: baseURL,
  timeout: 600000,
  headers: {
    'Content-Type': 'application/json'
  }
})

export default {
  install(Vue) {
    Object.defineProperty(Vue.prototype, '$axios', { value: Axios })
  }
}