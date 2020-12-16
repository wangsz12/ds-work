import Vue from 'vue'
import Vuex from 'vuex'
import $api from '../api/index'

Vue.use(Vuex)

export default new Vuex.Store({
  state: {
    showRightMenu: false,
    disableRightMenu: false,
    lastStart: {},
    lastFinish: {},
    cellAssignment: '',
    mazeSize: {
      height: 5,
      width: 5
    }
  },
  mutations: {
    setShowRightMenu(state, newShowRightMenu) {
      if (state.disableRightMenu) return
      state.showRightMenu = newShowRightMenu
    },
    setDisableRightMenu(state, newDisableRightMenu) {
      state.disableRightMenu = newDisableRightMenu
    },
    setLastStart(state, newLastStart) {
      state.lastStart = newLastStart
    },
    setLastFinish(state, newLastFinish) {
      state.lastFinish = newLastFinish
    },
    setCellAssignment(state, newCellAssignment) {
      state.cellAssignment = newCellAssignment
    },
    setMazeSize(state, newMazeSize) {
      state.mazeSize = Object.assign({}, newMazeSize)
    },
  },
  actions: {
    setShowRightMenu({ commit }, newSetRightMenu) {
      commit('setShowRightMenu', newSetRightMenu)
    },
    setDisableRightMenu({ commit }, newPreventRightMenu) {
      commit('setDisableRightMenu', newPreventRightMenu)
    },
    setLastStart({ commit }, newLastStart) {
      commit('setLastStart', newLastStart)
    },
    setLastFinish({ commit }, newLastFinish) {
      commit('setLastFinish', newLastFinish)
    },
    setCellAssignment({ commit }, newCellAssignment) {
      commit('setCellAssignment', newCellAssignment)
    },
    setMazeSize({ commit }, newMazeSize) {
      commit('setMazeSize', newMazeSize)
    },
    solveMaze(ctx, payload) {
      return $api.solveMaze(payload)
        .then((res) => {
          if (res.data.status === false) {
            return Promise.reject(res.data.msg)
          }
          return Promise.resolve(res)
        })
        .catch((err) => {
          return Promise.reject(err)
        })
    },
    getAllRecords() {
      return $api.getAllRecords()
        .then((res) => {
          if (res.data.status === false) {
            return Promise.reject(res.data.msg)
          }
          return Promise.resolve(res)
        })
        .catch((err) => {
          return Promise.reject(err)
        })
    }
  }
})