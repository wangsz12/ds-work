import api from '@/api/index'

export default {
  install(Vue) {
    Object.defineProperty(Vue.prototype, '$api', { value: api })
  }
}