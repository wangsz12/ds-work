const { solveMazeByAlgo } = require("../build/Release/dswork.node");
const MazeRecord = require("../models/MazeRecord");
const logger = require("../plugins/logger")
const { trueReturn, falseReturn } = require("../plugins/result")

const prefix = '/dswork'

const algorithmMap = {
  'dfs': 1,
  'bfs': 2,
  'greedy': 3,
  'ucs': 4,
  'A*euc': 5,
  'A*man': 6,
  'A*che': 7,
  'test': 8,
  'oldbfs': 9
}

/**
 * POST /dswork/solveMaze
 */

async function solveMaze(ctx, next) {
  const { algorithm, mazeData, size, start, finish } = ctx.request.body

  let row = [], maze = []
  mazeData.forEach((item, index) => {
    row.push(item)
    if ((index + 1) % size.width === 0) {
      maze.push(row)
      row = []
    }
  })

  let res = {}
  try {
    res = solveMazeByAlgo(algorithmMap[algorithm], maze, size, start, finish)
    MazeRecord.createMazeRecord(mazeData, size, start, finish, algorithm)
    ctx.response.body = trueReturn(res)
  }
  catch(e) {
    ctx.response.body = falseReturn({}, '暂不支持该算法！')
  }
  finally {
    await next()
  }
}

module.exports = {
  prefix,
  handlers: {
    'POST /solveMaze': solveMaze
  }
}