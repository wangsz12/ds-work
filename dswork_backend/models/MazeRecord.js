const logger = require('../plugins/logger')
const mongo = require('../plugins/mongodb')

class MazeRecord {
  constructor(maze, size, start, finish, algorithm) {
    this.maze = maze
    this.size = size
    this.start = start
    this.finish = finish
    this.algorithm = algorithm
    this.time = new Date()
  }

  static async createMazeRecord(maze, size, start, finish, algorithm) {
    const newMaze = new MazeRecord(maze, size, start, finish, algorithm)
    await mongo.insert('record', newMaze)
  }

  static async getAllRecord() {
    return mongo.find('record')
  }
}

module.exports = MazeRecord