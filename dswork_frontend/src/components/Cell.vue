<template>
  <div
    class="cell--container" 
    :style="cell__style"
    @click="clickHdl"
    @contextmenu.prevent="sendIndex"
    @dragenter="draggingHdl"
  >
    <div class="content" :style="contentStyle">
      <canvas 
        class="canvas"
        ref="canvas"
        width="40"
        height="40"
      ></canvas>
      <span class="actual-cost">{{ cost.actualCost === -1 ? '' : cost.actualCost }}</span>
      <span class="step-index">{{ stepIndex === -1 ? '' : stepIndex }}</span>
    </div>
  </div>
</template>

<script>
const colorMap = {
  'wall': 'black',
  'start': 'rgb(34, 206, 43)',
  'finish': 'red',
  'space': 'white',
  'open': 'rgb(96, 226, 64)',
  'close': 'rgb(175, 238, 238)'
}

import { mapState, mapActions } from 'vuex'

export default {
  data() {
    return {
      which: 'space',
      contentStyle: 'display: none;',
      cost: {
        actualCost: -1,
        estimateCost: -1,
        totalCost: -1
      },
      stepIndex: -1
    }
  },
  props: {
    index: {
      type: Number,
      default: 0
    }
  },
  methods: {
    ...mapActions(['setLastStart', 'setLastFinish', 'setDisableRightMenu']),

    init(ifSetAsSpace = true) {
      if (ifSetAsSpace) {
        this.setAsSpace()
      }
      this.$refs['canvas'].getContext("2d").clearRect(0, 0, 40, 40)
      this.contentStyle = 'display: none;'
    },
    clickHdl() {
      if (this.disableRightMenu) {
        return
      }

      switch(this.which) {
        case 'space':
          this.setAsWall()
          break
        case 'start':
          this.setLastStart({})
          this.setAsSpace()
          break
        case 'finish':
          this.setLastFinish({})
          this.setAsSpace()
          break
        case 'wall':
          this.setAsSpace()
          break
      }
    },
    setAsWall() {
      this.which = 'wall'
    },
    setAsStart() {
      if (this.lastStart.setAsSpace) {
        this.lastStart.setAsSpace()
      }
      this.setLastStart(this)
      this.which = 'start'
    },
    setAsFinish() {
      if (this.lastFinish.setAsSpace) {
        this.lastFinish.setAsSpace()
      }
      this.setLastFinish(this)
      this.which = 'finish'
    },
    setAsSpace() {
      this.which = 'space'
    },
    setAsProcess(from, next, cost, index = -1,) {
      this.cost = cost
      if (index > 0) {
        this.stepIndex = index
      }

      this.contentStyle = 'display: block;'

      let ctx = this.$refs['canvas'].getContext("2d")
      ctx.beginPath()
      ctx.lineWidth = 4
      ctx.strokeStyle = 'yellow'
      
      ctx.moveTo(20, 20)
      ctx.lineTo(20 - from.dy * 20, 20 - from.dx * 20)
      ctx.moveTo(20, 20)
      ctx.lineTo(20 + next.dy * 20, 20 + next.dx * 20)

      ctx.stroke()
      if (this.which !== 'space') {
        return
      }
      this.which = 'close'
    },
    setAsOpenNode() {
      this.which = 'open'
    },
    setAsCloseNode() {
      this.which = 'close'
    },
    sendIndex() {
      this.$emit('getIndex', this.index)
    },
    draggingHdl() {
      if (this.which !== 'space') {
        this.setAsSpace()
      }
      else {
        this.setAsWall()
      }
    }
  },
  computed: {
    ...mapState(['lastStart', 'lastFinish', 'cellAssignment', 'disableRightMenu']),

    cell__style() {
      return `background-color: ${colorMap[this.which]}`
    }
  },
  mounted() {
    this.init()
  }
}
</script>

<style lang="scss" scoped>
  $cell-size: 2.5rem;

  .cell--container {
    height: $cell-size;
    width: $cell-size;
    border: rgb(180, 180, 180) solid 1px;
    box-sizing: border-box;

    .content {
      position: relative;

      .canvas {
        height: 100%;
        width: 100%;
        z-index: 999;
      }

      .actual-cost {
        position: absolute;
        left: 5%;
        bottom: 10%;
        font-size: .4rem;
        user-select: none;
        z-index: 998;
      }

      .step-index {
        position: absolute;
        right: 5%;
        bottom: 35%;
        font-size: .4rem;
        user-select: none;
        z-index: 998;
      }
    }
  }
</style>