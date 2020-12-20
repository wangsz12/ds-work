const moment = require('moment')
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
  const result = await MazeRecord.getAllRecord()
  ctx.response.body = trueReturn({
    records: result.map((item) => {
      return {
        id: item._id,
        size: `${item.size.height} × ${item.size.width}`,
        algorithm: algos.filter((_item) => { return _item.value === item.algorithm })[0].name,
        time: moment.parseZone(item.time).format('YYYY[-]MM[-]DD HH[:]mm[:]ss')
      }
    })
  })
  await next()
}

async function getRecord(ctx, next) {
  const { id } = ctx.request.body
  const result = await MazeRecord.getRecord({ id })
  ctx.response.body = trueReturn({
    records: result
  })
  await next()
}

module.exports = {
  prefix,
  handlers: {
    'GET /getAllRecords': getAllRecords,
    'POST /getRecord': getRecord
  }
}