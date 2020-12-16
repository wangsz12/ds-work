<template>
  <div :key="mazeKey">
    <div
      :class="['maze--container', disableRightMenu ? 'disabled' : '']"
      :style="maze__style"
      @contextmenu.prevent="rightClickPrevent($event)"
    >
      <Cell
        v-for="i in size"
        :key="i"
        :index="i"
        :ref="`cell${i}`"
        @getIndex="getCellIndex"
      />
    </div>
    <RightMenu
      :x="rightMenu.x"
      :y="rightMenu.y"
      @select="getRightMenuItem"
    />
  </div>
</template>

<script>
import Cell from './Cell'
import RightMenu from './RightMenu'
import { mapActions, mapState } from 'vuex'

export default {
  data() {
    return {
      size: 25,
      rightMenu: {
        x: 0,
        y: 0
      },
      nowCell: {
        index: 0,
        assignment: ''
      },
      mazeKey: 0,
      disabled: false
    }
  },
  components: {
    Cell,
    RightMenu
  },
  methods: {
    ...mapActions(['setShowRightMenu', 'setMazeSize', 'setLastStart', 'setLastFinish', 'setDisableRightMenu']),

    rightClickPrevent(e) {
      this.setShowRightMenu(true)
      const { layerX, layerY } = e
      
      this.rightMenu = {
        x: layerY,
        y: layerX
      }
    },
    getRightMenuItem(assignment) {
      switch(assignment) {
        case 's':
        case 'f':
          this.nowCell.assignment = assignment
          break
        default:
          break
      }
      this.setCell()
    },
    getCellIndex(index) {
      this.nowCell.index = index
    },
    setCell() {
      for (let item in this.$refs) {
        const cellIndex = Number(item.substring(4))
        if (cellIndex === this.nowCell.index) {
          switch(this.nowCell.assignment) {
            case 's':
              this.$refs[item][0].setAsStart()
              break
            case 'f':
              this.$refs[item][0].setAsFinish()
              break
            default:
              break
          }
          break
        }
      }
    },
    getMazeArray() {
      if (!this.lastStart.index) {
        throw new Error('请选择起点！')
      }
      if (!this.lastFinish.index) {
        throw new Error('请选择终点！')
      }

      let mazeData = []
      for (let item in this.$refs) {
        const index = parseInt(item.substring(4))
        if (index > this.size) break

        switch(this.$refs[item][0].which) {
          case 'wall':
            mazeData.push(1)
            break
          case 'space':
          default:
            mazeData.push(0)
            break
        }
      }
      return mazeData
    }
  },
  computed: {
    ...mapState(['mazeSize', 'lastStart', 'lastFinish', 'disableRightMenu']),

    maze__style() {
      return `width: ${2.5 * this.mazeSize.width}rem;`
    },
    listenMazeSize() {
      const { mazeSize } = this
      return { mazeSize }
    }
  },
  watch: {
    listenMazeSize(newValue) {
      this.mazeKey++
      this.size = newValue.mazeSize.height * newValue.mazeSize.width
    }
  },
  mounted() {
    this.size = this.mazeSize.height * this.mazeSize.width
  }
}
</script>

<style lang="scss" scoped>
  .maze--container {
    display: flex;
    flex-wrap: wrap;
    border: rgb(0, 0, 0) solid 2.5px;
    box-sizing: bor;
  }

  .disabled {
    cursor: not-allowed;
  }
</style>