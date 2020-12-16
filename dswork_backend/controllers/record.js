const MazeRecord = require('../models/MazeRecord')
const logger = require('../plugins/logger')
const { trueReturn, falseReturn } = require('../plugins/result')

const prefix = '/dswork'

const algos = [
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
  },
  {
    value: 'oldbfs',
    name: '有问题的BFS'
  }
]

/**
 * GET /dswork/getAllRecords
 */

async function getAllRecords(ctx, next) {
  await MazeRecord.getAllRecord()
    .then((res) => {
      ctx.response.body = trueReturn({
        records: res.map((item) => {
          const date = new Date(item.time)
          let hour = date.getHours()
          if (hour < 10) hour = `0${hour}`
          let minute = date.getMinutes()
          if (minute < 10) minute = `0${minute}`
          let second = date.getSeconds()
          if (second < 10) second = `0${second}`
          const time = `${date.toISOString().substr(0, 10)} ${hour}:${minute}:${second}`
          return {
            id: item._id,
            size: `${item.size.height} × ${item.size.width}`,
            algorithm: algos.filter((_item) => { return _item.value === item.algorithm })[0].name,
            time
          }
        })
      })
    })
  await next()
}

module.exports = {
  prefix,
  handlers: {
    'GET /getAllRecords': getAllRecords
  }
}