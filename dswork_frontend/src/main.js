import Vue from 'vue'
import App from './App.vue'
import store from './store/index'

import axiosConfig from './plugins/axios'
import APIConfig from './plugins/api'
Vue.use(axiosConfig).use(APIConfig)

// nprogress
import NProgress from 'nprogress'

// vue-toast
import Toast from 'vue-toastification'
import 'vue-toastification/dist/index.css'
Vue.use(Toast, {
  transition: 'Vue-Toastification__bounce',
  maxToasts: 3,
  newestOnTop: true,
  position: 'top-right',
  timeout: 3000,
  closeOnClick: true,
  pauseOnFocusLoss: false,
  pauseOnHover: false,
  draggable: true,
  draggablePercent: 0.7,
  showCloseButtonOnHover: false,
  hideProgressBar: true,
  closeButton: 'button',
  icon: true,
  rtl: false
})

// element-ui
import { Input, Divider, Button, Select, Option, Table, MessageBox, TableColumn } from 'element-ui'
Vue.use(Input).use(Divider).use(Button).use(Select).use(Option).use(Table).use(TableColumn)

Vue.component(MessageBox.name, MessageBox)
Vue.prototype.$MsgBox = MessageBox

Vue.use(NProgress)

Vue.config.productionTip = false

new Vue({
  store,
  render: h => h(App)
}).$mount('#app')
