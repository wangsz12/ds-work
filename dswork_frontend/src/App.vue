<template>
  <div class="container">
    <div class="main-box">
      <div class="title">
        <span>数据结构课设 — 基于搜索技术的迷宫寻路系统</span>
        <span>组员：金成洋 刘桢谋 滕邱鹏 王盛泽 张昕瑶 张伊男</span>
      </div>
      <div class="maze-box">
        <Maze ref="maze"></Maze>
      </div>
      <div class="control-panel">
        <div class="left">
          <div class="maze-panel">
            <el-divider content-position="center">设置迷宫参数</el-divider>
            <div class="input-box">
              <span>设置高度：</span>
              <el-input
                v-model="tmp.height"
                class="input"
                size="medium"
                maxlength="2"
                type="number"
              >
                <template slot="append">格</template>
              </el-input>
            </div>
            <div class="input-box">
              <span>设置宽度：</span>
              <el-input
                v-model="tmp.width"
                class="input"
                size="medium"
                maxlength="2"
                type="number"
              >
                <template slot="append">格</template>
              </el-input>
            </div>
            <div class="btn-box">
              <el-button
                @click="changeMazeSize"
                type="primary"
                :round="true"
              >确定</el-button>
              <el-button
                @click="clearMaze(true)"
                type="danger"
                :round="true"
              >清空迷宫</el-button>
              <el-button
                @click="randomMaze"
                type="primary"
                :round="true"
                disabled
              >随机</el-button>
            </div>
          </div>
        </div>
        <div class="right">
          <div class="algo-panel">
            <el-divider content-position="center">选择求解算法</el-divider>
            <el-select v-model="algoSelected" placeholder="请选择求解算法" style="max-width: 60%;" :disabled="algoSelectDisabled">
              <el-option
                v-for="(item, index) in algos"
                :key="index"
                :label="item.name"
                :value="item.value">
              </el-option>
            </el-select>
            <el-button
              @click="solveHdl"
              type="primary"
              :round="true"
              v-if="!algoSelectDisabled"
            >确定</el-button>
            <el-button
              @click="reselect"
              type="danger"
              :round="true"
              v-if="algoSelectDisabled"
            >重新选择</el-button>
          </div>
          <div class="history-panel">
            <el-divider content-position="center">查看搜索历史</el-divider>
            <el-button
              @click="showHistory"
              type="primary"
              :round="true"
            >查看搜索历史</el-button>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
import { mapActions, mapState } from 'vuex'
import Maze from './components/Maze'
import History from './components/History'
import NProgress from 'nprogress'
import 'nprogress/nprogress.css'

export default {
  data() {
    return {
      maze: {},
      tmp: {
        height: 5,
        width: 5
      },
      algos: [
        {
          value: 'dfs',
          name: '深度优先搜索'
        },
        {
          value: 'bfs',
          name: '广度优先搜索'
        },
        {
          value: 'greedy',
          name: '贪婪搜索'
        },
        {
          value: 'ucs',
          name: '等代价搜索'
        },
        {
          value: 'A*euc',
          name: 'A*搜索(欧氏距离)'
        },
        {
          value: 'A*man',
          name: 'A*搜索(曼哈顿距离)'
        },
        {
          value: 'A*che',
          name: 'A*搜索(切比雪夫距离)'
        },
        {
          value: 'test',
          name: '测试用'
        }
      ],
      algoSelected: '',
      algoSelectDisabled: false,
      steps: [],
      process: [],
      draw: {},
      startCode: -1,
      finishCode: -1
    }
  },
  components: {
    Maze,
    // eslint-disable-next-line vue/no-unused-components
    History
  },
  methods: {
    ...mapActions([
      'setMazeSize',
      'solveMaze',
      'setLastStart',
      'setLastFinish',
      'setDisableRightMenu',
      'getAllRecords'
    ]),

    changeMazeSize() {
      NProgress.start()

      const data = {
        height: Number(this.tmp.height),
        width: Number(this.tmp.width)
      }

      if (data.height <= 0 || data.width <= 0) {
        this.$toast.error('迷宫的宽高必须大于0！')
        NProgress.done()
        return
      }
      else {
        this.maze = data
        localStorage.setItem('maze', JSON.stringify(this.maze))
        this.clearMaze(false)
        this.setMazeSize(this.maze)
        this.$toast.success('设置成功！')
        NProgress.done()
      }
    },
    solveHdl() {
      if (this.algoSelected === '') {
        this.$toast.error('请选择求解算法！')
        return
      }

      let mazeData = []
      try {
        mazeData = this.$refs['maze'].getMazeArray()
      }
      catch(e) {
        this.$toast.error(e.message)
        return
      }

      const start = {
        x: parseInt((this.lastStart.index - 1) / this.mazeSize.width),
        y: parseInt((this.lastStart.index - 1) % this.mazeSize.width)
      }
      this.startCode = start.x * this.mazeSize.width + start.y + 1;

      const finish = {
        x: parseInt((this.lastFinish.index - 1) / this.mazeSize.width),
        y: parseInt((this.lastFinish.index - 1) % this.mazeSize.width)
      }
      this.finishCode = finish.x * this.mazeSize.width + finish.y + 1;

      this.$api.solveMaze({
        algorithm: this.algoSelected,
        mazeData,
        size: this.mazeSize,
        start,
        finish
      }).then((res) => {
        if (!res.data.status) {
          return Promise.reject(res.data.msg)
        }

        this.algoSelectDisabled = true

        const { steps, process } = res.data.data

        if (steps.length === 1 && steps[0].code === -1) {
          return Promise.reject(new Error('该迷宫无解'))
        }

        this.setDisableRightMenu(true)

        const newSteps = steps.map((item, index) => {
          return {
            actualCost: index === 0 ? -1 : item.actualCost,
            x: parseInt((item.code - 1) / this.mazeSize.width),
            y: parseInt((item.code - 1) % this.mazeSize.width)
          }
        })

        const newProcess = process.map((item, index) => {
          return {
            actualCost: index === 0 ? -1 : item.actualCost,
            x: parseInt((item.code - 1) / this.mazeSize.width),
            y: parseInt((item.code - 1) % this.mazeSize.width),
            time: item.time
          }
        })

        this.steps = newSteps
        console.log('newSteps: ', newSteps);
        this.process = newProcess
        console.log('newProcess: ', newProcess);

        if (['dfs', 'greedy', 'test'].includes(this.algoSelected)) {
          this.drawDFS(start, finish, this.algoSelected === 'greedy' ? 150 : 1);
        }
        else {
          this.drawBFS(start, finish);
        }

        this.$toast.success('迷宫寻路成功！')
      }).catch((err) => {
        this.$toast.error(err.message)
      })
    },
    drawDFS(start, finish, drawInterval) {
      let index = 0
      this.draw = setInterval(() => {
        const item = this.process[index]
        const refStr = `cell${item.x * this.mazeSize.width + item.y + 1}`

        if (index) {
          if ((this.process[index - 1].x !== start.x || this.process[index - 1].y !== start.y) && (this.process[index - 1].x !== finish.x || this.process[index - 1].y !== finish.y)) {
            this.$refs['maze'].$refs[`cell${this.process[index - 1].x * this.mazeSize.width + this.process[index - 1].y + 1}`][0].setAsCloseNode()
          }
          if (item.time <= this.process[index - 1].time) {
            if (this.process[index - 1].x !== finish.x || this.process[index - 1].y !== finish.y) {
              this.$refs['maze'].$refs[`cell${this.process[index - 1].x * this.mazeSize.width + this.process[index - 1].y + 1}`][0].setAsSpace()
            }
          }
        }
          
        if (index && (this.process[index].x !== start.x || this.process[index].y !== start.y) && (this.process[index].x !== finish.x || this.process[index].y !== finish.y)) {
          this.$refs['maze'].$refs[refStr][0].setAsOpenNode()
        }
        if (index === this.process.length - 1) {
          clearInterval(this.draw)
          this.drawRoutes()
        }
        index++
      }, drawInterval)
    },
    drawBFS(start, finish) {
      const maxTime = this.process.sort((a, b) => b.time - a.time)[0].time

      let newProcess = new Array(maxTime + 1)
      for (let i = 0; i < newProcess.length; i++) {
        newProcess[i] = new Array()
      }

      this.process.forEach((item) => newProcess[item.time].push(item))

      let timeIndex = 1
      this.draw = setInterval(() => {
        const now = newProcess[timeIndex]

        if (timeIndex > 1) {
          newProcess[timeIndex - 1].forEach((item) => {
            const preRef = `cell${item.x * this.mazeSize.width + item.y + 1}`
            if ((item.x !== start.x || item.y !== start.y) && (item.x !== finish.x || item.y !== finish.y)) {
              this.$refs['maze'].$refs[preRef][0].setAsCloseNode()
            }
          })
        }

        now.forEach((item) => {
          const refStr = `cell${item.x * this.mazeSize.width + item.y + 1}`
          if ((item.x !== start.x || item.y !== start.y) && (item.x !== finish.x || item.y !== finish.y)) {
            this.$refs['maze'].$refs[refStr][0].setAsOpenNode()
          }
        })
          
        if (timeIndex === newProcess.length - 1) {
          newProcess[timeIndex].forEach((item) => {
            const preRef = `cell${item.x * this.mazeSize.width + item.y + 1}`
            if ((item.x !== start.x || item.y !== start.y) && (item.x !== finish.x || item.y !== finish.y)) {
              this.$refs['maze'].$refs[preRef][0].setAsCloseNode()
            }
          })
          clearInterval(this.draw)
          this.drawRoutes()
        }
        timeIndex++
      }, 400)
    },
    drawRoutes() {
      let index = 0
      this.draw = setInterval(() => {
        const item = this.steps[index]
        let from = {}, next = {}

        if (index !== 0) {
          from = {
            dx: item.x - this.steps[index - 1].x,
            dy: item.y - this.steps[index - 1].y
          }
        }
        if (index !== this.steps.length - 1) {
          next = {
            dx: this.steps[index + 1].x - item.x,
            dy: this.steps[index + 1].y - item.y
          }
        }

        const refStr = `cell${item.x * this.mazeSize.width + item.y + 1}`
        const cost = {
          actualCost: item.actualCost
        }
        this.$refs['maze'].$refs[refStr][0].setAsProcess(from, next, cost, index, false)
        if (index === this.steps.length - 1) {
          clearInterval(this.draw)
        }
        index++
      }, 100)
    },
    clearMaze(ifShowToast) {
      this.setDisableRightMenu(false)
      this.setLastStart({})
      this.setLastFinish({})
      this.algoSelectDisabled = false
      for (let item in this.$refs['maze'].$refs) {
        if (this.$refs['maze'].$refs[item][0].init) {
          this.$refs['maze'].$refs[item][0].init()
        }
      }

      if (ifShowToast) {
        this.$toast.info('迷宫已清空')
      }
    },
    reselect() {
      clearInterval(this.draw)
      this.$toast.info('寻路已停止')
      this.setDisableRightMenu(false)
      this.algoSelectDisabled = false
      this.draw = {}
      
      this.steps.forEach((item) => {
        const code = item.x * this.mazeSize.width + item.y + 1
        const refStr = `cell${code}`
        if (this.$refs['maze'].$refs[refStr][0].init) {
          this.$refs['maze'].$refs[refStr][0].init(([this.startCode, this.finishCode].includes(code) ? false : true))
        }
      })
      
      this.process.forEach((item) => {
        const code = item.x * this.mazeSize.width + item.y + 1
        const refStr = `cell${code}`
        if (this.$refs['maze'].$refs[refStr][0].init) {
          this.$refs['maze'].$refs[refStr][0].init(([this.startCode, this.finishCode].includes(code) ? false : true))
        }
      })
    },
    randomMaze() {
      for (let i = 1; i <= this.mazeSize.height * this.mazeSize.width; ++i) {
        const refStr = `cell${i}`
        this.$refs['maze'].$refs[refStr][0].setAsSpace()
        if (Math.random() * 10 > 6) {
          this.$refs['maze'].$refs[refStr][0].setAsWall()
        }
      }
    },
    showHistory() {
      const that = this

      this.$api.getAllRecords()
        .then((res) => {
          if (!res.data.status) {
            return Promise.reject(res.data.msg)
          }

          this.$MsgBox({
            title: '搜索历史',
            message: this.$createElement('History',
              {
                props: {
                  tableData: res.data.data.records
                },
                on: {
                  closeMsgBox(feedback) {
                    feedback.then((res) => {
                      console.log('res: ', res);
                      that.tmp = res.size
                      that.changeMazeSize()

                      res.maze.forEach((item, index) => {
                        const refStr = `cell${index + 1}`
                        if (item) {
                          that.$refs.maze.$refs[refStr][0].setAsWall()
                        }
                        else {
                          if (index === (res.start.x * that.mazeSize.width + res.start.y)) {
                            that.$refs.maze.$refs[refStr][0].setAsStart()
                          }
                          else if (index === (res.finish.x * that.mazeSize.width + res.finish.y)) {
                            that.$refs.maze.$refs[refStr][0].setAsFinish()
                          }
                          else {
                            that.$refs.maze.$refs[refStr][0].setAsSpace()
                          }
                        }
                      })
                    })
                  }
                }
              }),
            customClass: 'message-box'
          })
          .then(() => {})
          .catch(() => {})
        })
        .catch((err) => {
          this.$toast.error(err.message)
        })
    }
  },
  computed: {
    ...mapState(['mazeSize', 'lastStart', 'lastFinish'])
  },
  mounted() {
    let maze = localStorage.getItem('maze')
    if (maze) {
      maze = JSON.parse(maze)
      this.tmp = maze
      this.setMazeSize(maze)
    }
  }
}
</script>

<style lang="scss" scoped>
  .container {
    min-height: 100vh;
    display: flex;
    justify-content: center;
    align-items: center;
    background: linear-gradient(-135deg, #C850C0, #4158D0);

    .main-box {
      min-width: 55rem;
      box-sizing: border-box;
      background-color: #fff;
      border-radius: 10px;
      box-shadow: 1px 3px 15px -5px rgba(0,0,0,.4);
      padding: .5rem 2rem;
      margin: 3rem auto;

      .title {
        text-align: center;
        
        span {
          display: block;
          margin: 1rem 0;

          &:nth-child(1) {
            font-weight: bold;
            font-size: 1.5rem;
          }
        }
      }

      .maze-box {
        height: 70%;
        width: auto;
        margin-top: 2rem;
        margin-bottom: 1rem;
        display: flex;
        justify-content: center;
        align-items: center;
      }

      .control-panel {
        display: flex;
        justify-content: space-between;

        .left {
          width: 50%;
          padding: 0 2rem;
          margin-bottom: 1rem;
          box-sizing: border-box;
          border-right: 2px rgb(220, 223, 230) solid;

          .maze-panel {
            width: 100%;

            .input-box {
              text-align: center;
              margin-bottom: 1rem;

              .input {
                width: 35%;
                margin-left: 1rem;
              }
            }

            .btn-box {
              text-align: center;
              
              button {
                &:nth-child(1),
                &:nth-child(3) {
                  width: 25%;
                }

                &:nth-child(2) {
                  width: 30%;
                }
              }
            }
          }
        }
        
        .right {
          width: 50%;
          padding: 0 2rem;
          margin-bottom: 1rem;
          box-sizing: border-box;

          .algo-panel {
            width: 100%;

            button {
              width: 30%;
              margin-left: 2rem;
            }
          }
          
          .history-panel {
            width: 100%;
            text-align: center;
          }
        }
      }
    }
  }
</style>

<style>
  .message-box {
    width: 50rem;
  }
</style>